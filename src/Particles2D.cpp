/*
  Particles2D.cpp
  "overturner"

  Created by Renaud Dufays on 17/03/17.
  Copyright © 2017. All rights reserved.
*/

#include <cmath>
#include <cassert>
#include "Particles2D.hpp"

Particles2D::Particles2D(int nParticles, double yStart, double zStart)
{
	mTime = 0.;
	mN = nParticles;
	mY = new double [nParticles];
	mZ = new double [nParticles];

	for (int i=0; i<nParticles; i++)
	{
		mY[i] = yStart;
		mZ[i] = zStart;
	}
}

Particles2D::Particles2D(int nParticles, double* yStart, double* zStart, int nPos)
{
	/*
		nParticles particles at every couple (yStart[i], zStart[i]).
		nPos is the length of yStart and zStart.
	*/
	mTime = 0.;
	mN = nParticles;
	mY = new double [nPos*nParticles];
	mZ = new double [nPos*nParticles];
	for (int i=0; i<nPos; i++)
	{
		for (int j=0; j<nParticles; j++)
		{
			mY[i*nParticles+j] = yStart[i];
			mZ[i*nParticles+j] = zStart[i];
		}
	}
}

Particles2D::Particles2D(int nParticles, double* yStart, double* zStart, int ny, int nz)
{
	/*
		nParticles particles at every couple (yStart[i], zStart[j]).
		ny is the length of yStart and nz is the length of zStart.
	*/
	mTime = 0.;
	mN = nParticles;
	mY = new double [ny*nz*nParticles];
	mZ = new double [ny*nz*nParticles];
	for (int i=0; i<ny; i++)
	{
		for (int j=0; j<nz; j++)
		{
			for (int k=0; k<nParticles; k++)
			{
				mY[i*nz*nParticles+j*nParticles+k] = yStart[i];
				mZ[i*nz*nParticles+j*nParticles+k] = zStart[j];
			}
		}
	}
}

Particles2D::Particles2D(const Particles2D& otherParticles)
{
	mN = otherParticles.mN;
	mTime = otherParticles.mTime;
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
	delete[] mY;
	delete[] mZ;
}

double Particles2D::GetY(int ithParticle) const
{
	// 0-based indexing
	assert (ithParticle > -1);
	assert (ithParticle < mN);
	return mY[ithParticle];
}

double Particles2D::GetZ(int ithParticle) const
{
	// 0-based indexing
	assert (ithParticle > -1);
	assert (ithParticle < mN);
	return mZ[ithParticle];
}

double Particles2D::GetN() const
{
	return mN;
}

double& Particles2D::SetY(int ithParticle)
{
	// 0-based indexing
	assert (ithParticle > -1);
	assert (ithParticle < mN);
	return mY[ithParticle];
}

double& Particles2D::SetZ(int ithParticle)
{
	// 0-based indexing
	assert (ithParticle > -1);
	assert (ithParticle < mN);
	return mZ[ithParticle];
}

double& Particles2D::SetTime(double T)
{
	return mTime;
}


void Particles2D::Print(std::string model) const
{
  	std::ofstream myfileT ("out/" + model + "time.out", std::ios::out | std::ios::app);
  	if (myfileT.is_open())
  	{
  		myfileT << mTime << "\n";
  		myfileT.close();
  	}
  	else
  	{
  		std::cout << "Unable to open file Out/" << model << "time.out\n";
  		abort();
  	}

	std::ofstream myfileY ("out/" + model + "Y.out", std::ios::out | std::ios::app);
	if (myfileY.is_open())
  	{
  		myfileY.setf(std::ios::scientific);
        myfileY.precision(10);
    	for(int i=0; i<mN; i++)
    	{
    		myfileY << mY[i] << " ";
  		}
  		myfileY << "\n";
  		myfileY.close();
  	}
  	else
  	{
  		std::cerr << "Unable to open file Out" << model << "/Y.out\n";
  		abort();
  	}

  	std::ofstream myfileZ ("out/" + model + "Z.out", std::ios::out | std::ios::app);
	if (myfileZ.is_open())
  	{	
  		myfileZ.setf(std::ios::scientific);
        myfileZ.precision(10);
  		for(int i=0; i<mN; i++)
    	{
    		myfileZ << mZ[i] << " ";
  		}
  		myfileZ << "\n";
  		myfileZ.close();
  	}
  	else
  	{
  		std::cerr << "Unable to open file Out/" + model + "Z.out\n";
  		abort();
  	}
}