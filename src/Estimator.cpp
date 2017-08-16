/*
  KernelEstimator.cpp
  "sde2D"

  Created by Renaud Dufays on 04/04/17
  Copyright © 2017. All rights reserved.
*/

#include "Estimator.hpp"

Estimator::Estimator(int ncelly, int ncellz, double H, double L):
mNcelly(ncelly), mNcellz(ncellz), mH(H), mL(L), mEstimator(ncelly,ncellz)
{}

void Estimator::Print(std::string filename, bool binary) const
{
	std::ofstream f = openOutputFile(filename,binary);
	f.setf(std::ios::scientific); f.precision(10);

	if (binary)
		mEstimator.PrintBinary(f);
	else
		mEstimator.Print(f);
    f.close();
}

void Estimator::Print(std::ofstream& f, bool binary) const
{
	f.setf(std::ios::scientific); f.precision(10);

	if (binary)
		mEstimator.PrintBinary(f);
	else
		mEstimator.Print(f);
    f.close();
}

KernelEstimator::KernelEstimator(int ncelly, int ncellz, double H, double L, double lambda, std::string kernelFunction):
Estimator(ncelly,ncellz,H,L), mLambda(lambda)
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

KernelEstimator::KernelEstimator(int ncelly, int ncellz, double H, double L, double lambda, double (*kernelFunction)(double y, double z)):
Estimator(ncelly, ncellz,H,L), mLambda(lambda), mKernel(kernelFunction)
{}

void KernelEstimator::Estimate(const Particles2D& particles)
{
	double dy = mL/mNcelly;
	double dz = mH/mNcellz;
	int N = particles.mN;
	for (int i=0; i<mNcelly; i++)
	{
		for (int j=0; j<mNcellz; j++)
		{
			for (int n=0; n<N; n++)
			{
				mEstimator(i,j) += mKernel((particles.mY[n]-(i+.5)*dy)/mLambda, (particles.mZ[n]-(j+.5)*dz)/mLambda);
			}
			mEstimator(i,j) /= (N*pow(mLambda,2));
		}
	}
}

BoxEstimator::BoxEstimator(int ncelly, int ncellz, double H0, double H, double L0, double L):
Estimator(ncelly,ncellz,H,L),
mH0(H0),
mL0(L0)
{}

void BoxEstimator::Estimate(const Particles2D& particles)
{
	int i, j;
	int N = particles.mN;
	double dy = mL/mNcelly;
	double dz = mH/mNcellz;
	for (int n=0; n<N; n++)
	{
		i = std::min(int((particles.mY[n]-mL0)/dy),mNcelly-1);
		j = std::min(int((particles.mZ[n]-mH0)/dz),mNcellz-1);
		mEstimator(i,j) += 1.;
	}
	mEstimator /= N;
}

TPMatrixEstimator::TPMatrixEstimator(int ncelly, int ncellz, double H, double L, int P0):
mNcelly(ncelly), mNcellz(ncellz), mP0(P0), mH(H), mL(L), mEstimator(ncelly*ncellz, ncelly*ncellz)
{} 

void TPMatrixEstimator::Print(std::string filename, bool binary) const
{
	std::ofstream f = openOutputFile(filename, binary);
	f.setf(std::ios::scientific); f.precision(10);

	if (binary)
		mEstimator.PrintBinary(f);
	else
		mEstimator.Print(f);
    f.close();
}

TPMatrixKernelEstimator::TPMatrixKernelEstimator(int ncelly, int ncellz, double H, double L, int P0, double lambda, std::string kernelFunction):
TPMatrixEstimator(ncelly,ncellz,H,L,P0), mLambda(lambda)
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

TPMatrixKernelEstimator::TPMatrixKernelEstimator(int ncelly, int ncellz, double H, double L, int P0, double lambda, double (*kernelFunction)(double y, double z)):
TPMatrixEstimator(ncelly, ncellz, H, L, P0), mLambda(lambda), mKernel(kernelFunction)
{}

void TPMatrixKernelEstimator::Estimate(const Particles2D& particles)
{
	double dy = mL/mNcelly;
	double dz = mH/mNcellz;

	for (int iyStart=0; iyStart<mNcelly; iyStart++)
	{
		for (int izStart=0; izStart<mNcellz; izStart++)
		{
			for (int iyEnd=0; iyEnd<mNcelly; iyEnd++)
			{
				for (int izEnd=0; izEnd<mNcellz; izEnd++)
				{	
					for (int n=0; n<mP0; n++)
					{
						mEstimator(iyStart*mNcellz+izStart,iyEnd*mNcellz+izEnd) += 
							mKernel((particles.mY[iyStart*mNcellz*mP0+izStart*mP0+n]-(iyEnd+.5)*dy)/mLambda,
							 		(particles.mZ[iyStart*mNcellz*mP0+izStart*mP0+n]-(izEnd+.5)*dz)/mLambda);
					}
					mEstimator(iyStart*mNcellz+izStart,iyEnd*mNcellz+izEnd) *= (dy*dz)/(mP0*pow(mLambda,2));
				}
			}
		}
	}
}

TPMatrixBoxEstimator::TPMatrixBoxEstimator(int ncelly, int ncellz, double H0, double H, double L0, double L, int P0):
	TPMatrixEstimator(ncelly,ncellz,H,L,P0),
	mH0(H0), 
	mL0(L0)
{}

void TPMatrixBoxEstimator::Estimate(const Particles2D& particles)
{
	Count(particles);
	mEstimator /= mP0;
}

Matrix TPMatrixBoxEstimator::Count(const Particles2D& particles)
{
	int iyStart, izStart, iyEnd, izEnd;
	double dy = mL/mNcelly;
	double dz = mH/mNcellz;
	for (iyStart=0; iyStart<mNcelly; iyStart++)
	{
		for (izStart=0; izStart<mNcellz; izStart++)
		{
			for (int n=0; n<mP0; n++)
			{
				iyEnd = std::min(int((particles.mY[iyStart*mNcellz*mP0+izStart*mP0+n]-mL0)/dy),mNcelly-1);
				izEnd = std::min(int((particles.mZ[iyStart*mNcellz*mP0+izStart*mP0+n]-mH0)/dz),mNcellz-1);
				mEstimator(iyStart*mNcellz+izStart,iyEnd*mNcellz+izEnd) += 1.;
			}
		}
	}
	return mEstimator;
}


TPMatrixBoxEstimatorP2B::TPMatrixBoxEstimatorP2B(int ncelly, int ncellz, double H0, double H, double L0, double L, int P0):
	TPMatrixBoxEstimator(ncelly,ncellz,H0,H,L0,L,P0)
{}

Matrix TPMatrixBoxEstimatorP2B::Count(const Particles2D& particles)
{
	int iyStart, izStart, iyEnd, izEnd;
	int ncelly_left = mNcelly/2;
	double dy = mL/mNcelly;
	double dz = mH/mNcellz;
	for (iyStart=0; iyStart<ncelly_left; iyStart++){
		for (izStart=0; izStart<mNcellz; izStart++){
			for (int n=0; n<mP0; n++){
				iyEnd = std::min(int((particles.mY[iyStart*mNcellz*mP0+izStart*mP0+n]-mL0)/dy),mNcelly-1);
				izEnd = std::min(int((particles.mZ[iyStart*mNcellz*mP0+izStart*mP0+n]-mH0)/dz),mNcellz-1);
				mEstimator(iyStart*mNcellz+izStart,iyEnd*mNcellz+izEnd) += 1.;
			}
		}
	}
	int iystartmap, iyendmap;
	for (iyStart=ncelly_left; iyStart<mNcelly; iyStart++){
		for (izStart=0; izStart<mNcellz; izStart++){
			for (iyEnd=0; iyEnd<mNcelly; iyEnd++){
				for (izEnd=0; izEnd<mNcellz; izEnd++){
					iystartmap = mNcelly-1-iyStart;
					iyendmap = mNcelly-1-iyEnd;
					mEstimator(iyStart*mNcellz+izStart,iyEnd*mNcellz+izEnd) = mEstimator(iystartmap*mNcellz+izStart,iyendmap*mNcellz+izEnd);
				}
			}
		}
	}
	return mEstimator;
}
/*
	Utility functions for the Kernel estimators
*/

double Gaussian(double y, double z)
{
	return exp(-.5*(y*y+z*z))/(2*M_PI);
}

double Epanechnikov(double y, double z)
{
	int indicator = (y*y + z*z <= 1.);
	return indicator*2*(1-y*y-z*z)/M_PI;
}