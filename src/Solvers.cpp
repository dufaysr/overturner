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

using namespace parameters;
/*-------------- Base Class Solver -----------------------*/
Solver::Solver(int Nloc, double yStart, double zStart):
	mParticles(Nloc,yStart,zStart), 
	// seed(std::chrono::system_clock::now().time_since_epoch().count()),
	seed(1),
	generator(seed),
	wiener(0.0,1.0)
{}

Solver::Solver(int Nloc, double* yStart, double* zStart, int n):
	mParticles(Nloc,yStart,zStart,n), 
	seed(std::chrono::system_clock::now().time_since_epoch().count()),
	generator(seed),
	wiener(0.0,1.0)
{}

Solver::Solver(int Nloc, double* yStart, double* zStart, int ny, int nz):
	mParticles(Nloc,yStart,zStart,ny,nz),
	seed(std::chrono::system_clock::now().time_since_epoch().count()),
	generator(seed),
	wiener(0.0,1.0)
{}

Solver::Solver(const Particles2D& particles):
	mParticles(particles),
	seed(std::chrono::system_clock::now().time_since_epoch().count()),
	generator(seed),
	wiener(0.0,1.0)
{}

Particles2D& Solver::Run()
{
	for (int i=0; i*dt < T; i++)
	{
		UpdatePosition();
	}
	return mParticles;
}

Particles2D& Solver::Run(std::string model, int nPrint)
{
	std::ofstream fT = openOutputFile("out/" + model + "/time.out");
	std::ofstream fY = openOutputFile("out/" + model + "/Y.out");
	fY.setf(std::ios::scientific); fY.precision(10);
	std::ofstream fZ = openOutputFile("out/" + model + "/Z.out");
	fZ.setf(std::ios::scientific); fZ.precision(10);

	PrintParticles(fT, fY, fZ);
	int i=0;
	while (i*dt < T)
	{
		for (int j=0; j<nPrint; j++)
		{
			UpdatePosition();
			i++;
		}
		PrintParticles(fT,fY,fZ); // print particles every 5 time steps
	}

	fT.close(); fY.close(); fZ.close();

	return mParticles;
}

Particles2D& Solver::RunAdim()
{
	for (int i=0; i*dtPrime < TPrime; i++)
	{
		UpdatePositionAdim();
	}
	return mParticles;
}

Particles2D& Solver::RunAdim(std::string model, int nPrint)
{
	std::ofstream fT = openOutputFile("out/" + model + "/time.out");
	std::ofstream fY = openOutputFile("out/" + model + "/Y.out");
	fY.setf(std::ios::scientific); fY.precision(10);
	std::ofstream fZ = openOutputFile("out/" + model + "/Z.out");
	fZ.setf(std::ios::scientific); fZ.precision(10);

	PrintParticles(fT, fY, fZ);
	int i=0;
	while (i*dt < T)
	{
		for (int j=0; j<nPrint; j++)
		{
			UpdatePositionAdim();
			i++;
		}
		PrintParticles(fT,fY,fZ); // print particles every 5 time steps
	}

	fT.close(); fY.close(); fZ.close();

	return mParticles;
}

void Solver::DisplayParticles() const
{
	for (int i=0; i<mParticles.mN; i++)
	{
		std::cout << i << " : (" << mParticles.mY[i] << ", " << mParticles.mZ[i] << ")" << std::endl;
	}
}

void Solver::PrintParticles(std::ofstream& fT, std::ofstream& fY, std::ofstream& fZ) const
{
	fT << mParticles.mTime << "\n";

    for(int i=0; i<mParticles.mN; i++)
    {
    	fY << mParticles.mY[i] << " ";
  	}
  	fY << "\n";

  	for(int i=0; i<mParticles.mN; i++)
    {
    	fZ << mParticles.mZ[i] << " ";
  	}
  	fZ << "\n";
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

/*----------------- Derived class from Solver : EMSolver -----------------------*/
/*--- /!\ Does not take the derivative of the diffusivities into account /!\ ---*/

EMSolver::EMSolver(int Nloc, double yStart, double zStart):
	Solver(Nloc,yStart,zStart)
{}

EMSolver::EMSolver(int Nloc, double* yStart, double* zStart, int n):
	Solver(Nloc,yStart,zStart,n)
{}

EMSolver::EMSolver(int Nloc, double* yStart, double* zStart, int ny, int nz):
	Solver(Nloc,yStart,zStart,ny,nz)
{}

EMSolver::EMSolver(const Particles2D& particles):
	Solver(particles)
{}

void EMSolver::UpdatePosition()
{
	using parameters::Kh;
	// construct a trivial random generator engine from a time-based seed:
	for (int i=0; i<mParticles.mN; i++)
	{
		mParticles.mY[i] += GetV(mParticles.mY[i],mParticles.mZ[i])*dt + sqrt(2*Kh*dt)*wiener(generator);
		mParticles.mZ[i] += GetW(mParticles.mY[i],mParticles.mZ[i])*dt 
							+ sqrt(2*Kv(mParticles.mY[i],mParticles.mZ[i])*dt)*wiener(generator);
	}
	mParticles.mTime += dt;
}

void EMSolver::UpdatePositionAdim()
{
	// construct a trivial random generator engine from a time-based seed:
	for (int i=0; i<mParticles.mN; i++)
	{

		mParticles.mY[i] += GetVPrime(mParticles.mY[i],mParticles.mZ[i])*dtPrime
							+ sqrt(2*PehInv(mParticles.mY[i],mParticles.mZ[i])*dtPrime)*wiener(generator);
		mParticles.mZ[i] += GetWPrime(mParticles.mY[i],mParticles.mZ[i])*dtPrime 
							+ sqrt(2*PevInv(mParticles.mY[i],mParticles.mZ[i])*dtPrime)*wiener(generator);
	}
	mParticles.mTime += dtPrime;
}

/*----------- Derived class from Solver : Backward Ito (BI) Solver ------------------*/

BISolver::BISolver(int N, double yStart, double zStart):
	Solver(N,yStart,zStart)
{}

BISolver::BISolver(int N, double* yStart, double* zStart, int n):
	Solver(N,yStart,zStart,n)
{}

BISolver::BISolver(int N, double* yStart, double* zStart, int ny, int nz):
	Solver(N,yStart,zStart,ny,nz)
{}

BISolver::BISolver(const Particles2D& particles):
	Solver(particles)
{}

void BISolver::UpdatePosition()
{
	using namespace parameters;
	
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
		dY = sqrt(2*Kh*dt)*R1;
		dZ = sqrt(2*Kv(y,z)*dt)*R2;
		// amplitude of the noises
		N1 = sqrt(2*Kh*dt);
		N2 = sqrt(2*Kv(y+dY,z+dZ)*dt);
		// update particles positions using backward-Ito scheme
		mParticles.mY[i] = std::min(L, std::max(mParticles.mY[i] + v*dt + N1*R1, 0.));
		mParticles.mZ[i] = std::min(H, std::max(mParticles.mZ[i] + w*dt + N2*R2, 0.));
	}
	mParticles.mTime += dt;
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
		dY = sqrt(2*PehInv(y,z)*dtPrime)*R1;
		dZ = sqrt(2*PevInv(y,z)*dtPrime)*R2;
		// amplitude of the noises
		N1 = sqrt(2*PehInv(y+dY,z+dZ)*dtPrime);
		N2 = sqrt(2*PevInv(y+dY,z+dZ)*dtPrime);
		// update particles positions using backward-Ito scheme
		mParticles.mY[i] = std::min(1., std::max(mParticles.mY[i] + v*dtPrime + N1*R1, 0.));
		mParticles.mZ[i] = std::min(1., std::max(mParticles.mZ[i] + w*dtPrime + N2*R2, 0.));
	}
	mParticles.mTime += dtPrime;
}