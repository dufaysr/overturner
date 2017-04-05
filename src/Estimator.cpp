/*
  KernelEstimator.cpp
  "overturner"

  Created by Renaud Dufays on 04/04/17
  Copyright © 2017. All rights reserved.
*/

#include "Estimator.hpp"

Estimator::Estimator(int dim1, int dim2):
mDim1(dim1), mDim2(dim2), mEstimator(dim1,dim2)
{}

void Estimator::Print(std::string filename) const
{
	mEstimator.Print(filename);
}

KernelEstimator::KernelEstimator(int dim1, int dim2, double lambda, std::string kernelFunction):
Estimator(dim1,dim2), mLambda(lambda)
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
		std::cout << "Unvalid kernel function name. Name bust be \"Epanechnikov\" or \"Gaussian\", no matter the case" << "\n";
		std::abort();
	}
}

KernelEstimator::KernelEstimator(int dim1, int dim2, double lambda, double (*kernelFunction)(double y, double z)):
Estimator(dim1, dim2), mLambda(lambda), mKernel(kernelFunction)
{}

void KernelEstimator::Estimate(Particles2D particles)
{
	double dy = 1./mDim1;
	double dz = 1./mDim2;
	for (int i=0; i<mDim1; i++)
	{
		for (int j=0; j<mDim2; j++)
		{
			for (int n=0; n<particles.GetN(); n++)
			{
				mEstimator(i,j) += mKernel((particles.GetY(n)-(i+.5)*dy)/mLambda, (particles.GetZ(n)-(j+.5)*dz)/mLambda);
			}
			mEstimator(i,j) /= (particles.GetN()*pow(mLambda,2));
		}
	}
}

BoxEstimator::BoxEstimator(int dim1, int dim2):
Estimator(dim1,dim2)
{}

void BoxEstimator::Estimate(Particles2D particles)
{
	int i, j;
	for (int n=0; n<particles.GetN(); n++)
	{
		i = std::min(int(particles.GetY(n)*mDim1),mDim1-1);
		j = std::min(int(particles.GetZ(n)*mDim2),mDim2-1);
		mEstimator(i,j) += 1.;
	}
	mEstimator /= particles.GetN();
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