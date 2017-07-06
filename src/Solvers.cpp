/*
  Solvers.cpp
  "overturner"

  Created by Renaud Dufays on 17/03/17.
  Copyright © 2017. All rights reserved.
*/

#include <chrono>
#include <iostream>
#include "Solvers.hpp"
#include "workingdirectory.hpp"

/*-------------- Base Class Solver -----------------------*/
Solver::Solver(const Particles2D& particles, double dt):
	mParticles(particles),
	seed(std::chrono::system_clock::now().time_since_epoch().count()),
	generator(seed),
	wiener(0.0,1.0),
	mdt(dt)
{}

Particles2D Solver::Run(const AbstractAdvDiffProblem& prob, double Time)
{
	double Tend = Time - mParticles.mTime;
	for (int i=0; i*mdt < Tend; i++)
	{
		UpdatePosition(prob);
	}
	return mParticles;
}

Particles2D Solver::Run(const AbstractAdvDiffProblem& prob, double Time,
				std::string model, int nPrint, bool printInit, bool binary)
{
	std::ofstream fT = openOutputFile(wd::root + "out/" + model + "/time.out",binary);
	std::ofstream fY = openOutputFile(wd::root + "out/" + model + "/Y.out",binary);
	fY.setf(std::ios::scientific); fY.precision(10);
	std::ofstream fZ = openOutputFile(wd::root + "out/" + model + "/Z.out",binary);
	fZ.setf(std::ios::scientific); fZ.precision(10);
	double Tend = Time - mParticles.mTime;

	if (!binary){
		if (printInit)
			PrintParticles(fT, fY, fZ);

		int i=0;
		while (i*mdt < Tend)
		{
			for (int j=0; j<nPrint; j++)
			{
				UpdatePosition(prob);
				i++;
			}
			PrintParticles(fT,fY,fZ); // print particles every nPrint time steps
		}
	}
	else{
		if (printInit)
			PrintParticlesBinary(fT, fY, fZ);

		int i=0;
		while (i*mdt < Tend)
		{
			for (int j=0; j<nPrint; j++)
			{
				UpdatePosition(prob);
				i++;
			}
			PrintParticlesBinary(fT,fY,fZ); // print particles every nPrint time steps
		}
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

void Solver::PrintParticlesBinary(std::ofstream& fT, std::ofstream& fY, std::ofstream& fZ) const
{
	fT.write((char*) &(mParticles.mTime), sizeof(double));
	fY.write((char*) &(mParticles.mY), mParticles.mN*sizeof(double));
	fZ.write((char*) &(mParticles.mZ), mParticles.mN*sizeof(double));
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
/*--- /!\ Does not take the derivative of the diffusivities (i.e. the gradient dirft term) into account /!\ ---*/

EMSolver::EMSolver(const Particles2D& particles, double dt):
	Solver(particles,dt)
{}

void EMSolver::UpdatePosition(const AbstractAdvDiffProblem& prob)
{
	// construct a trivial random generator engine from a time-based seed:
	LowerTriMatrix B(0,0,0);
	Vec2 U(0,0);
	double sqrt_dt = sqrt(mdt);
	double R1, R2;
	for (int i=0; i<mParticles.mN; i++)
	{
		B = prob.getB(mParticles.mY[i],mParticles.mZ[i]);
		U = prob.getU(mParticles.mY[i],mParticles.mZ[i]);

		R1 = wiener(generator);
		R2 = wiener(generator);

		mParticles.mY[i] += U[0]*mdt + B(1,1)*sqrt_dt*R1;
		mParticles.mZ[i] += U[1]*mdt + B(2,1)*sqrt_dt*R1 + B(2,2)*sqrt_dt*R2;
	}
	mParticles.mTime += mdt;
}

/*----------- Derived class from Solver : Backward Ito (BI) Solver ------------------*/

BISolver::BISolver(const Particles2D& particles, double dt):
	Solver(particles, dt)
{}

void BISolver::UpdatePosition(const AbstractAdvDiffProblem& prob)
{
	LowerTriMatrix B(0,0,0);
	Vec2 U(0,0);
	double R1, R2, dY, dZ, y, z, ypred, zpred;
	double sqrt_dt = sqrt(mdt);
	for (int i=0; i<mParticles.mN; i++)
	{
		// position and speed of particle i at time t
		y = mParticles.mY[i];
		z = mParticles.mZ[i];
		U = prob.getU(y,z);
		B = prob.getB(y,z);
		// realisations of the noises
		R1 = wiener(generator);
		R2 = wiener(generator);
		// prediction step of the backward-Ito scheme
		dY = B(1,1)*sqrt_dt*R1;
		dZ = B(2,1)*sqrt_dt*R1 + B(2,2)*sqrt_dt*R2;
		// No-through BC also applies on the predictions -> boucing on the walls
		ypred = y+dY;
		zpred = z+dZ;
		ypred = (ypred < prob.getL0()) ? 2*prob.getL0()-ypred : (ypred > prob.getL1()) ? 2*prob.getL1()-ypred : ypred;
		zpred = (zpred < prob.getH0()) ? 2*prob.getH0()-zpred : (zpred > prob.getH1()) ? 2*prob.getH1()-zpred : zpred;
		// amplitude of the noises
		B = prob.getB(ypred,zpred);
		/* update particles positions using backward-Ito scheme
		* 2 options for the no-flux BC : either stick to the wall or bounce on it.
		* Uncomment the one of your choice and comment the other.
		*/
		// 1. Stick to the wall
		// mParticles.mY[i] = std::min(prob.getL1(), std::max(mParticles.mY[i] + v*mdt + B(1,1)*sqrt_dt*R1, prob.getL0()));
		// mParticles.mZ[i] = std::min(prob.getH1(), std::max(mParticles.mZ[i] + w*mdt + B(2,1)*sqrt_dt*R1 + B(2,2)*sqrt_dt*R2, prob.getH0()));
		// 2. Bounce on the wall
		ypred = mParticles.mY[i] + U[0]*mdt + B(1,1)*sqrt_dt*R1;
		zpred = mParticles.mZ[i] + U[1]*mdt + B(2,1)*sqrt_dt*R1 + B(2,2)*sqrt_dt*R2;
		mParticles.mY[i] = (ypred < prob.getL0()) ? 2*prob.getL0()-ypred : (ypred > prob.getL1()) ? 2*prob.getL1()-ypred : ypred;
		mParticles.mZ[i] = (zpred < prob.getH0()) ? 2*prob.getH0()-zpred : (zpred > prob.getH1()) ? 2*prob.getH1()-zpred : zpred;
	}
	mParticles.mTime += mdt;
}