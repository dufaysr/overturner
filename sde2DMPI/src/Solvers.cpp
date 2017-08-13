/*
  Solvers.cpp
  "overturner"

  Created by Renaud Dufays on 17/03/17.
  Copyright © 2017. All rights reserved.
*/

#include "Solvers.hpp"

/*-------------- Base Class Solver -----------------------*/
Solver::Solver(const Particles2D& particles):
	mParticles(particles),
	seed(std::chrono::system_clock::now().time_since_epoch().count()),
	generator(seed),
	wiener(0.0,1.0)
{}

Particles2D Solver::Run(const AbstractAdvDiffProblem& prob, double Time)
{
	double Tend = Time - mParticles.mTime;
	for (int i=0; i*prob.getdt() < Tend; i++)
	{
		UpdatePosition(prob);
	}
	return mParticles;
}

/*----------- Derived class from Solver : Backward Ito (BI) Solver ------------------*/

BISolver::BISolver(const Particles2D& particles):
	Solver(particles)
{}

void BISolver::UpdatePosition(const AbstractAdvDiffProblem& prob)
{
	double N1, N2, R1, R2, dY, dZ, y, z, v, w;
	for (int i=0; i<mParticles.mN; i++)
	{
		// position and speed of particle i at time t
		y = mParticles.mY[i];
		z = mParticles.mZ[i];
		v = prob.getV(y,z);
		w = prob.getW(y,z);
		// realisations of the noises
		R1 = wiener(generator);
		R2 = wiener(generator);
		// prediction step of the backward-Ito scheme
		dY = sqrt(2*prob.getKh(y,z)*prob.getdt())*R1;
		dZ = sqrt(2*prob.getKv(y,z)*prob.getdt())*R2;
		// amplitude of the noises
		double ypred = y+dY;
		double zpred = z+dZ;
		ypred = (ypred < prob.getL0()) ? 2*prob.getL0()-ypred : (ypred > prob.getL1()) ? 2*prob.getL1()-ypred : ypred;
		zpred = (zpred < prob.getH0()) ? 2*prob.getH0()-zpred : (zpred > prob.getH1()) ? 2*prob.getH1()-zpred : zpred;
		// amplitude of the noises
		N1 = sqrt(2*prob.getKh(y+dY,z+dZ)*prob.getdt());
		N2 = sqrt(2*prob.getKv(y+dY,z+dZ)*prob.getdt());
		/* update particles positions using backward-Ito scheme
		* 2 options for the no-flux BC : either stick to the wall or bounce on it.
		* Uncomment the one of your choice and comment the other.
		*/
		// 1. Stick to the wall
		// mParticles.mY[i] = std::min(L, std::max(mParticles.mY[i] + v*dt + N1*R1, 0.));
		// mParticles.mZ[i] = std::min(H, std::max(mParticles.mZ[i] + w*dt + N2*R2, 0.));
		// 2. Bounce on the wall
		double ynew = mParticles.mY[i] + v*prob.getdt() + N1*R1;
		double znew = mParticles.mZ[i] + w*prob.getdt() + N2*R2;
		mParticles.mY[i] = (ynew < prob.getL0()) ? 2*prob.getL0()-ynew : (ynew > prob.getL1()) ? 2*prob.getL1()-ynew : ynew;
		mParticles.mZ[i] = (znew < prob.getH0()) ? 2*prob.getH0()-znew : (znew > prob.getH1()) ? 2*prob.getH1()-znew : znew;
	}
	mParticles.mTime += prob.getdt();
}