/*
  Particles2D.cpp
  "overturner"

  Created by Renaud Dufays on 17/03/17.
  Copyright © 2017. All rights reserved.
*/

#include <cmath>
#include <cassert>
#include "Particles2D.hpp"

using namespace parameters;

Particles2D::Particles2D(int N, double yStart, double zStart):
mN(N), mTime(0.)
{
	mY = new double [mN];
	mZ = new double [mN];

	for (int i=0; i<N; i++)
	{
		mY[i] = yStart;
		mZ[i] = zStart;
	}
}

Particles2D::Particles2D(int N, double* yStart, double* zStart):
mN(N), mTime(0.)
{
	mY = new double [mN];
	mZ = new double [mN];

	for (int i=0; i<N; i++)
	{
		mY[i] = yStart[i];
		mZ[i] = zStart[i];
	}
}

Particles2D::Particles2D(int Nloc, double* yStart, double* zStart, int n):
mN(Nloc*n), mTime(0.)
{
	/*
		Nloc particles at every couple (yStart[i], zStart[i]).
		n is the length of yStart and zStart.
	*/
	mY = new double [mN];
	mZ = new double [mN];
	for (int i=0; i<n; i++)
	{
		for (int j=0; j<Nloc; j++)
		{
			mY[i*Nloc+j] = yStart[i];
			mZ[i*Nloc+j] = zStart[i];
		}
	}
}

Particles2D::Particles2D(int Nloc, double* yStart, double* zStart, int ny, int nz):
mN(Nloc*ny*nz), mTime(0.)
{
	/*
		Nloc particles at every couple (yStart[i], zStart[j]).
		ny is the length of yStart and nz is the length of zStart.
	*/
	mY = new double [mN];
	mZ = new double [mN];
	for (int iy=0; iy<ny; iy++)
	{
		for (int iz=0; iz<nz; iz++)
		{
			for (int k=0; k<Nloc; k++)
			{
				mY[iy*nz*Nloc+iz*Nloc+k] = yStart[iy];
				mZ[iy*nz*Nloc+iz*Nloc+k] = zStart[iz];
			}
		}
	}
}

Particles2D::Particles2D(const Particles2D& otherParticles):
mN(otherParticles.mN), mTime(otherParticles.mTime)
{
	/*
		Copy constructor
	*/
	mY = new double[mN];
	mZ = new double[mN];
	for (int i=0; i<mN; i++)
	{
		mY[i] = otherParticles.mY[i];
		mZ[i] = otherParticles.mZ[i];
	}
}

Particles2D::~Particles2D()
{
	/*
		Destructor
	*/
	delete[] mY;
	delete[] mZ;
}