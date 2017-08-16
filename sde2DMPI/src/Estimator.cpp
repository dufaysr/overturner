/*
  KernelEstimator.cpp
  "overturner"

  Created by Renaud Dufays on 04/04/17
  Copyright © 2017. All rights reserved.
*/

#include "Estimator.hpp"

Estimator::Estimator(int ncelly, int ncellz, double H, double L):
mNcelly(ncelly), mNcellz(ncellz), mH(H), mL(L), mEstimator(ncelly,ncellz)
{}

void Estimator::Print(std::ofstream& f, bool binary) const
{
	f.setf(std::ios::scientific); f.precision(10);
	if (binary)
		mEstimator.PrintBinary(f);
	else
		mEstimator.Print(f);
    f.close();
}

BoxEstimator::BoxEstimator(int ncelly, int ncellz, double H, double L):
Estimator(ncelly,ncellz,H,L)
{}

void BoxEstimator::Estimate(const Particles2D& particles)
{
	int i, j;
	int N = particles.mN;
	double dy = mL/mNcelly;
	double dz = mH/mNcellz;
	for (int n=0; n<N; n++)
	{
		i = std::min(int(particles.mY[n]/dy),mNcelly-1);
		j = std::min(int(particles.mZ[n]/dz),mNcellz-1);
		mEstimator(i,j) += 1.;
	}
	mEstimator /= N;
}