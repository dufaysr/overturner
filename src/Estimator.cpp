/*
  KernelEstimator.cpp
  "overturner"

  Created by Renaud Dufays on 04/04/17
  Copyright © 2017. All rights reserved.
*/

#include "Estimator.hpp"

Estimator::Estimator(int dimy, int dimz):
mDimy(dimy), mDimz(dimz), mEstimator(dimy,dimz)
{}

void Estimator::Print(std::string filename) const
{
	mEstimator.Print(filename);
}

KernelEstimator::KernelEstimator(int dimy, int dimz, double lambda, std::string kernelFunction):
Estimator(dimy,dimz), mLambda(lambda)
{
	transform(kernelFunction.begin(), kernelFunction.end(), kernelFunction.begin(), ::tolower);
	if (kernelFunction == "gaussian")
	{
		mKernel = Gaussian;
	}
	else if (kernelFunction == "epanechnikov")
	{
		mKernel = Epanechnikov;
	}
	else
	{
		std::cerr << "Unvalid kernel function name. Name bust be \"Epanechnikov\" or \"Gaussian\", no matter the case" << "\n";
		abort();
	}
}

KernelEstimator::KernelEstimator(int dimy, int dimz, double lambda, double (*kernelFunction)(double y, double z)):
Estimator(dimy, dimz), mLambda(lambda), mKernel(kernelFunction)
{}

void KernelEstimator::Estimate(const Particles2D& particles)
{
	double dy = 1./mDimy;
	double dz = 1./mDimz;
	int N = particles.mN;
	for (int i=0; i<mDimy; i++)
	{
		for (int j=0; j<mDimz; j++)
		{
			for (int n=0; n<N; n++)
			{
				mEstimator(i,j) += mKernel((particles.mY[n]-(i+.5)*dy)/mLambda, (particles.mZ[n]-(j+.5)*dz)/mLambda);
			}
			mEstimator(i,j) /= (N*pow(mLambda,2));
		}
	}
}

BoxEstimator::BoxEstimator(int dimy, int dimz):
Estimator(dimy,dimz)
{}

void BoxEstimator::Estimate(const Particles2D& particles)
{
	int i, j;
	int N = particles.mN;
	for (int n=0; n<N; n++)
	{
		i = std::min(int(particles.mY[n]*mDimy),mDimy-1);
		j = std::min(int(particles.mZ[n]*mDimz),mDimz-1);
		mEstimator(i,j) += 1.;
	}
	mEstimator /= N;
}

GlobalEstimator::GlobalEstimator(int dimy, int dimz):
mDimy(dimy), mDimz(dimz), mEstimator(dimy*dimz, dimy*dimz)
{} 

void GlobalEstimator::Print(std::string filename) const
{
	mEstimator.Print(filename);
}

GlobalKernelEstimator::GlobalKernelEstimator(int dimy, int dimz, double lambda, std::string kernelFunction):
GlobalEstimator(dimy,dimz), mLambda(lambda)
{
	transform(kernelFunction.begin(), kernelFunction.end(), kernelFunction.begin(), ::tolower);
	if (kernelFunction == "gaussian")
	{
		mKernel = Gaussian;
	}
	else if (kernelFunction == "epanechnikov")
	{
		mKernel = Epanechnikov;
	}
	else
	{
		std::cerr << "Unvalid kernel function name. Name bust be \"Epanechnikov\" or \"Gaussian\", no matter the case" << "\n";
		abort();
	}
}

GlobalKernelEstimator::GlobalKernelEstimator(int dimy, int dimz, double lambda, double (*kernelFunction)(double y, double z)):
GlobalEstimator(dimy, dimz), mLambda(lambda), mKernel(kernelFunction)
{}

void GlobalKernelEstimator::Estimate(const Particles2D& particles)
{
	double dy = 1./mDimy;
	double dz = 1./mDimz;
	int N = particles.mN;
	for (int iyStart=0; iyStart<mDimy; iyStart++)
	{
		for (int izStart=0; izStart<mDimz; izStart++)
		{
			for (int iyEnd=0; iyEnd<mDimy; iyEnd++)
			{
				for (int izEnd=0; izEnd<mDimz; izEnd++)
				{	
					for (int n=0; n<N; n++)
					{
						mEstimator(iyStart*mDimz+izStart,iyEnd*mDimz+izEnd) += 
							mKernel((particles.mY[iyStart*mDimz*N+izStart*N+n]-(iyEnd+.5)*dy)/mLambda,
							 		(particles.mZ[iyStart*mDimz*N+izStart*N+n]-(iZEnd+.5)*dz)/mLambda);
					}
					mEstimator(iyStart*mDimz+izStart,iyEnd*mDimz+izEnd) /= (N*pow(mLambda,2));
				}
			}
		}
	}
}

GlobalBoxEstimator::GlobalBoxEstimator(int dimy, int dimz):
GlobalEstimator(dimy,dimz)
{}

void GlobalBoxEstimator::Estimate(const Particles2D& particles)
{
	int iyStart, izStart, iyEnd, izEnd;
	int N = particles.mN;
	for (iyStart=0; iyStart<mDimy; iyStart++)
	{
		for (izStart=0; izStart<mDimz; izStart++)
		{
			for (int n=0; n<N; n++)
			{
				iyEnd = std::min(int(particles.mY[iyStart*mDimz*N+izStart*N+n]*mDimy),mDimy-1);
				izEnd = std::min(int(particles.mZ[iyStart*mDimz*N+izStart*N+n]*mDimz),mDimz-1);
				mEstimator(iyStart*mDimz+izStart,iyEnd*mDimz+izEnd) += 1.;
			}
		}
	}
	mEstimator /= N;
}

double Gaussian(double y, double z)
{
	return exp(-.5*(y*y+z*z))/(2*M_PI);
}

double Epanechnikov(double y, double z)
{
	int indicator = (y*y + z*z <= 1.);
	return indicator*2*(1-y*y-z*z)/M_PI;
}