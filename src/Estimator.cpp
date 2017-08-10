/*
  KernelEstimator.cpp
  "overturner"

  Created by Renaud Dufays on 04/04/17
  Copyright © 2017. All rights reserved.
*/

#include "Estimator.hpp"

Estimator::Estimator(int nboxy, int nboxz, double H, double L):
mNboxy(nboxy), mNboxz(nboxz), mH(H), mL(L), mEstimator(nboxy,nboxz)
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

KernelEstimator::KernelEstimator(int nboxy, int nboxz, double H, double L, double lambda, std::string kernelFunction):
Estimator(nboxy,nboxz,H,L), mLambda(lambda)
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

KernelEstimator::KernelEstimator(int nboxy, int nboxz, double H, double L, double lambda, double (*kernelFunction)(double y, double z)):
Estimator(nboxy, nboxz,H,L), mLambda(lambda), mKernel(kernelFunction)
{}

void KernelEstimator::Estimate(const Particles2D& particles)
{
	double dy = mL/mNboxy;
	double dz = mH/mNboxz;
	int N = particles.mN;
	for (int i=0; i<mNboxy; i++)
	{
		for (int j=0; j<mNboxz; j++)
		{
			for (int n=0; n<N; n++)
			{
				mEstimator(i,j) += mKernel((particles.mY[n]-(i+.5)*dy)/mLambda, (particles.mZ[n]-(j+.5)*dz)/mLambda);
			}
			mEstimator(i,j) /= (N*pow(mLambda,2));
		}
	}
}

BoxEstimator::BoxEstimator(int nboxy, int nboxz, double H0, double H, double L0, double L):
Estimator(nboxy,nboxz,H,L),
mH0(H0),
mL0(L0)
{}

void BoxEstimator::Estimate(const Particles2D& particles)
{
	int i, j;
	int N = particles.mN;
	double dy = mL/mNboxy;
	double dz = mH/mNboxz;
	for (int n=0; n<N; n++)
	{
		i = std::min(int((particles.mY[n]-mL0)/dy),mNboxy-1);
		j = std::min(int((particles.mZ[n]-mH0)/dz),mNboxz-1);
		mEstimator(i,j) += 1.;
	}
	mEstimator /= N;
}

TPMatrixEstimator::TPMatrixEstimator(int nboxy, int nboxz, double H, double L, int Nloc):
mNboxy(nboxy), mNboxz(nboxz), mNloc(Nloc), mH(H), mL(L), mEstimator(nboxy*nboxz, nboxy*nboxz)
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

TPMatrixKernelEstimator::TPMatrixKernelEstimator(int nboxy, int nboxz, double H, double L, int Nloc, double lambda, std::string kernelFunction):
TPMatrixEstimator(nboxy,nboxz,H,L,Nloc), mLambda(lambda)
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

TPMatrixKernelEstimator::TPMatrixKernelEstimator(int nboxy, int nboxz, double H, double L, int Nloc, double lambda, double (*kernelFunction)(double y, double z)):
TPMatrixEstimator(nboxy, nboxz, H, L, Nloc), mLambda(lambda), mKernel(kernelFunction)
{}

void TPMatrixKernelEstimator::Estimate(const Particles2D& particles)
{
	double dy = mL/mNboxy;
	double dz = mH/mNboxz;

	for (int iyStart=0; iyStart<mNboxy; iyStart++)
	{
		for (int izStart=0; izStart<mNboxz; izStart++)
		{
			for (int iyEnd=0; iyEnd<mNboxy; iyEnd++)
			{
				for (int izEnd=0; izEnd<mNboxz; izEnd++)
				{	
					for (int n=0; n<mNloc; n++)
					{
						mEstimator(iyStart*mNboxz+izStart,iyEnd*mNboxz+izEnd) += 
							mKernel((particles.mY[iyStart*mNboxz*mNloc+izStart*mNloc+n]-(iyEnd+.5)*dy)/mLambda,
							 		(particles.mZ[iyStart*mNboxz*mNloc+izStart*mNloc+n]-(izEnd+.5)*dz)/mLambda);
					}
					mEstimator(iyStart*mNboxz+izStart,iyEnd*mNboxz+izEnd) *= (dy*dz)/(mNloc*pow(mLambda,2));
				}
			}
		}
	}
}

TPMatrixBoxEstimator::TPMatrixBoxEstimator(int nboxy, int nboxz, double H0, double H, double L0, double L, int Nloc):
	TPMatrixEstimator(nboxy,nboxz,H,L,Nloc),
	mH0(H0), 
	mL0(L0)
{}

void TPMatrixBoxEstimator::Estimate(const Particles2D& particles)
{
	Count(particles);
	mEstimator /= mNloc;
}

Matrix TPMatrixBoxEstimator::Count(const Particles2D& particles)
{
	int iyStart, izStart, iyEnd, izEnd;
	double dy = mL/mNboxy;
	double dz = mH/mNboxz;
	for (iyStart=0; iyStart<mNboxy; iyStart++)
	{
		for (izStart=0; izStart<mNboxz; izStart++)
		{
			for (int n=0; n<mNloc; n++)
			{
				iyEnd = std::min(int((particles.mY[iyStart*mNboxz*mNloc+izStart*mNloc+n]-mL0)/dy),mNboxy-1);
				izEnd = std::min(int((particles.mZ[iyStart*mNboxz*mNloc+izStart*mNloc+n]-mH0)/dz),mNboxz-1);
				mEstimator(iyStart*mNboxz+izStart,iyEnd*mNboxz+izEnd) += 1.;
			}
		}
	}
	return mEstimator;
}


TPMatrixBoxEstimatorP2B::TPMatrixBoxEstimatorP2B(int nboxy, int nboxz, double H0, double H, double L0, double L, int Nloc):
	TPMatrixBoxEstimator(nboxy,nboxz,H0,H,L0,L,Nloc)
{}

Matrix TPMatrixBoxEstimatorP2B::Count(const Particles2D& particles)
{
	int iyStart, izStart, iyEnd, izEnd;
	int nboxy_left = mNboxy/2;
	double dy = mL/mNboxy;
	double dz = mH/mNboxz;
	for (iyStart=0; iyStart<nboxy_left; iyStart++){
		for (izStart=0; izStart<mNboxz; izStart++){
			for (int n=0; n<mNloc; n++){
				iyEnd = std::min(int((particles.mY[iyStart*mNboxz*mNloc+izStart*mNloc+n]-mL0)/dy),mNboxy-1);
				izEnd = std::min(int((particles.mZ[iyStart*mNboxz*mNloc+izStart*mNloc+n]-mH0)/dz),mNboxz-1);
				mEstimator(iyStart*mNboxz+izStart,iyEnd*mNboxz+izEnd) += 1.;
			}
		}
	}
	int iystartmap, iyendmap;
	for (iyStart=nboxy_left; iyStart<mNboxy; iyStart++){
		for (izStart=0; izStart<mNboxz; izStart++){
			for (iyEnd=0; iyEnd<mNboxy; iyEnd++){
				for (izEnd=0; izEnd<mNboxz; izEnd++){
					iystartmap = mNboxy-1-iyStart;
					iyendmap = mNboxy-1-iyEnd;
					mEstimator(iyStart*mNboxz+izStart,iyEnd*mNboxz+izEnd) = mEstimator(iystartmap*mNboxz+izStart,iyendmap*mNboxz+izEnd);
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