/*
  Solvers.cpp
  "overturner"

  Created by Renaud Dufays on 17/03/17.
  Copyright © 2017. All rights reserved.
*/

#include <chrono>
#include <random>
#include <iostream>
#include "Solvers.hpp"

/*-------------- Base Class Solver -----------------------*/
Solver::Solver(int N, double yStart, double zStart, double dt, double T):
	mParticles(N,yStart,zStart), mFinalTime(T), mDt(dt), 
	seed(std::chrono::system_clock::now().time_since_epoch().count()),
	generator(seed),
	wiener(0.0,1.0)
{}

Solver::Solver(int N, double* yStart, double* zStart, double dt, double T):
	mParticles(N,yStart,zStart), mFinalTime(T), mDt(dt), 
	seed(std::chrono::system_clock::now().time_since_epoch().count()),
	generator(seed),
	wiener(0.0,1.0)
{}

Solver::Solver(Particles2D particles, double dt, double T):
	mParticles(particles), mFinalTime(T), mDt(dt), 
	seed(std::chrono::system_clock::now().time_since_epoch().count()),
	generator(seed),
	wiener(0.0,1.0)
{}

Particles2D Solver::Run()
{
	PrintParticles();
	for(int i=0; i*mDt < mFinalTime; i++)
	{
		UpdatePosition();
		PrintParticles();		
	}
	return mParticles;
}

Particles2D Solver::RunAdim()
{
	PrintParticles();
	for(int i=0; i*mDt < mFinalTime; i++)
	{
		UpdatePositionAdim();
		PrintParticles();		
	}
	return mParticles;
}

void Solver::DisplayParticles() const
{
	for (int i=0; i<mParticles.mN; i++)
	{
		std::cout << i << " : (" << mParticles.mY[i] << ", " << mParticles.mZ[i] << ")" << std::endl;
	}
}

void Solver::PrintParticles() const
{
	mParticles.Print(); 
}

void Solver::PrintParameters() const
{

}

void Solver::TestWiener()
{
	const int nrolls=10000;  // number of experiments
  	const int nstars=100;    // maximum number of stars to distribute

  	int width = 7;
  	int p[width]={};

  	for (int i=0; i<nrolls; ++i) 
  	{
    	double number = wiener(generator);
    	if ((number>-double(width)/2)&&(number<=double(width)/2))
    	{
    		++p[int(round(number))+width/2];
    	}
  	}

  	std::cout << "normal_distribution (0.0,1.0):" << std::endl;

  	for (int i=0; i<width; ++i) 
  	{
    	std::cout << std::setw(4) << i-double(width)/2 << " -> " << std::setw(4) << (i+1)-double(width)/2 << ": ";
    	std::cout << std::string(p[i]*nstars/nrolls,'*') << std::endl;
  	}
}

/*----------- Derived class from Solver : EMSolver ------------------*/
/*------------ WRONG ! TO BE MODIFIED -------------------------------*/

EMSolver::EMSolver(int N, double yStart, double zStart, double dt, double T):
	Solver(N,yStart,zStart,dt,T)
{}

EMSolver::EMSolver(int N, double* yStart, double* zStart, double dt, double T):
	Solver(N,yStart,zStart,dt,T)
{}

EMSolver::EMSolver(Particles2D particles, double dt, double T):
	Solver(particles,dt,T)
{}

void EMSolver::UpdatePosition()
{
	using constants::Kh;
	// construct a trivial random generator engine from a time-based seed:
	for (int i=0; i<mParticles.mN; i++)
	{
		mParticles.mY[i] += GetV(mParticles.mY[i],mParticles.mZ[i])*mDt + sqrt(2*Kh*mDt)*wiener(generator);
		mParticles.mZ[i] += GetW(mParticles.mY[i],mParticles.mZ[i])*mDt 
							+ sqrt(2*Kv(mParticles.mY[i],mParticles.mZ[i])*mDt)*wiener(generator);
	}
	mParticles.mTime += mDt;
}

void EMSolver::UpdatePositionAdim()
{
	// construct a trivial random generator engine from a time-based seed:
	for (int i=0; i<mParticles.mN; i++)
	{

		mParticles.mY[i] += GetVPrime(mParticles.mY[i],mParticles.mZ[i])*mDt
							+ sqrt(2*PehInv(mParticles.mY[i],mParticles.mZ[i])*mDt)*wiener(generator);
		mParticles.mZ[i] += GetWPrime(mParticles.mY[i],mParticles.mZ[i])*mDt 
							+ sqrt(2*PevInv(mParticles.mY[i],mParticles.mZ[i])*mDt)*wiener(generator);
	}
	mParticles.mTime += mDt;
}

/*----------- Derived class from Solver : Backward Ito (BI) Solver ------------------*/

BISolver::BISolver(int N, double yStart, double zStart, double dt, double T):
	Solver(N,yStart,zStart,dt,T)
{}

BISolver::BISolver(int N, double* yStart, double* zStart, double dt, double T):
	Solver(N,yStart,zStart,dt,T)
{}

BISolver::BISolver(Particles2D particles, double dt, double T):
	Solver(particles,dt,T)
{}

void BISolver::UpdatePosition()
{
	using namespace constants;
	
	double N1, N2, R1, R2, dY, dZ, y, z, v, w;
	for (int i=0; i<mParticles.mN; i++)
	{
		// position and speed of particle i at time t
		y = mParticles.mY[i];
		z = mParticles.mZ[i];
		v = GetV(y,z);
		w = GetW(y,z);
		// realisations of the noises
		R1 = wiener(generator);
		R2 = wiener(generator);
		// prediction step of the backward-Ito scheme
		dY = sqrt(2*Kh*mDt)*R1;
		dZ = sqrt(2*Kv(y,z)*mDt)*R2;
		// amplitude of the noises
		N1 = sqrt(2*Kh*mDt);
		N2 = sqrt(2*Kv(y+dY,z+dZ)*mDt);
		// update particles positions using backward-Ito scheme
		mParticles.mY[i] = std::min(L, std::max(mParticles.mY[i] + v*mDt + N1*R1, 0.));
		mParticles.mZ[i] = std::min(H, std::max(mParticles.mZ[i] + w*mDt + N2*R2, 0.));
	}
	mParticles.mTime += mDt;
}

void BISolver::UpdatePositionAdim()
{
	double N1, N2, R1, R2, dY, dZ, y, z, v, w;
	for (int i=0; i<mParticles.mN; i++)
	{
		// position and speed of particle i at time t
		y = mParticles.mY[i];
		z = mParticles.mZ[i];
		v = GetVPrime(y,z);
		w = GetWPrime(y,z);
		// realisations of the noises
		R1 = wiener(generator);
		R2 = wiener(generator);
		// prediction step of the backward-Ito scheme
		dY = sqrt(2*PehInv(y,z)*mDt)*R1;
		dZ = sqrt(2*PevInv(y,z)*mDt)*R2;
		// amplitude of the noises
		N1 = sqrt(2*PehInv(y+dY,z+dZ)*mDt);
		N2 = sqrt(2*PevInv(y+dY,z+dZ)*mDt);
		// update particles positions using backward-Ito scheme
		mParticles.mY[i] = std::min(1., std::max(mParticles.mY[i] + v*mDt + N1*R1, 0.));
		mParticles.mZ[i] = std::min(1., std::max(mParticles.mZ[i] + w*mDt + N2*R2, 0.));
	}
	mParticles.mTime += mDt;
}