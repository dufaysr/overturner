/*
  KernelEstimator.hpp
  "sde2D"

  Created by Renaud Dufays on 04/04/17
  Copyright © 2017. All rights reserved.
*/

#ifndef KERNELESTIMATORHEADERDEF
#define KERNELESTIMATORHEADERDEF

#include <cmath>
#include <string>
#include <algorithm>
#include "Particles2D.hpp"
#include "Matrix.hpp"
#include "Utilities.hpp"

/*
	Abstract base class for the concentration estimators.
*/
class Estimator
{
	protected:
		int mNcelly, mNcellz; // ncelly, ncellz
		double mH; // total size of the domain in in the z direction
		double mL; // total size of the domain in in the y direction
		Matrix mEstimator;

	public:
		Estimator(int ncelly, int ncellz, double H, double L);
		virtual ~Estimator() {}
		// estimate the concentration from particles. The result is stored in mEstimator
		virtual void Estimate(const Particles2D& particles) =0;
		void Print(std::string filename, bool binary=false) const;
		void Print(std::ofstream& f, bool binary=false) const;
};

/*
	Kernel estimator
*/
class KernelEstimator : public Estimator
{
	protected:
		double mLambda;
		double (*mKernel)(double y, double z);

	public:
		KernelEstimator(int ncelly, int ncellz, double H, double L, double lambda, std::string kernelFunction);
		KernelEstimator(int ncelly, int ncellz, double H, double L, double lambda, double (*kernelFunction)(double y, double z));
		void Estimate(const Particles2D& particles);
};

/*
	Box-counting estimator
*/
class BoxEstimator : public Estimator
{
	protected:
		double mH0; // inferior boundary in the z direction : mH0 <= z
		double mL0; // inferior boundary in the y direction : mL0 <= y

	public:
		BoxEstimator(int ncelly, int ncellz, double H0, double H, double L0, double L);
		void Estimate(const Particles2D& particles);
};


/*
	Abstract base class for the estimator of the transition probability matrix
*/
class TPMatrixEstimator
{
	protected:
		int mNcelly, mNcellz, mP0; // ncelly, ncellz, P0
		double mH, mL; // total size of the domain in in the z and y directions
		Matrix mEstimator; // ncelly*ncellz x ncelly*ncellz matrix to store the results

	public:
		TPMatrixEstimator(int ncelly, int ncellz, double H, double L, int P0);
		virtual ~TPMatrixEstimator() {}
		virtual void Estimate(const Particles2D& particles) =0;
		void Print(std::string filename, bool binary=false) const;
};

/*
	Kernel estimator
*/
class TPMatrixKernelEstimator : public TPMatrixEstimator
{
	protected:
		double mLambda;
		double (*mKernel)(double y, double z);

	public:
		TPMatrixKernelEstimator(int ncelly, int ncellz, double H, double L, int P0, double lambda, std::string kernelFunction);
		TPMatrixKernelEstimator(int ncelly, int ncellz, double H, double L, int P0, double lambda, double (*kernelFunction)(double y, double z));
		void Estimate(const Particles2D& particles);
};

/*
	Box counting estimator
*/
class TPMatrixBoxEstimator : public TPMatrixEstimator
{
	protected:
		double mH0; // inferior boundary in the z direction : mH0 <= z
		double mL0; // inferior boundary in the y direction : mL0 <= y

	public:
		TPMatrixBoxEstimator(int ncelly, int ncellz, double H0, double H, double L0, double L, int P0);
		void Estimate(const Particles2D& particles);
		virtual Matrix Count(const Particles2D& particles);
};

/*
	Special estimator taking into account the symmetry of the bi-overturner problems.
	Only used in ComputeP2BTransitionProbabilities
*/
class TPMatrixBoxEstimatorP2B : public TPMatrixBoxEstimator
{
	public:
		TPMatrixBoxEstimatorP2B(int ncelly, int ncellz, double H0, double H, double L0, double L, int P0);
		Matrix Count(const Particles2D& particles);
};

// Gaussian Kernel
double Gaussian(double y, double z);
// Epanechnikov kernel
double Epanechnikov(double y, double z);
#endif