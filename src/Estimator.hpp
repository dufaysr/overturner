/*
  KernelEstimator.hpp
  "overturner"

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

class Estimator
{
	protected:
		int mNboxy, mNboxz;
		double mH, mL;
		Matrix mEstimator;

	public:
		Estimator(int nboxy, int nboxz, double H, double L);
		virtual ~Estimator() {}
		virtual void Estimate(const Particles2D& particles) =0;
		void Print(std::string filename, bool binary=false) const;
		void Print(std::ofstream& f, bool binary=false) const;
};

class KernelEstimator : public Estimator
{
	private:
		double mLambda;
		double (*mKernel)(double y, double z);

	public:
		KernelEstimator(int nboxy, int nboxz, double H, double L, double lambda, std::string kernelFunction);
		KernelEstimator(int nboxy, int nboxz, double H, double L, double lambda, double (*kernelFunction)(double y, double z));
		void Estimate(const Particles2D& particles);
};

class BoxEstimator : public Estimator
{
	private:
		double mH0, mL0;

	public:
		BoxEstimator(int nboxy, int nboxz, double H0, double H, double L0, double L);
		void Estimate(const Particles2D& particles);
};

class TPMatrixEstimator
{
	protected:
		int mNboxy, mNboxz, mNloc;
		double mH, mL;
		Matrix mEstimator;

	public:
		TPMatrixEstimator(int nboxy, int nboxz, double H, double L, int Nloc);
		virtual ~TPMatrixEstimator() {}
		virtual void Estimate(const Particles2D& particles) =0;
		void Print(std::string filename, bool binary=false) const;
};

class TPMatrixKernelEstimator : public TPMatrixEstimator
{
	private:
		double mLambda;
		double (*mKernel)(double y, double z);

	public:
		TPMatrixKernelEstimator(int nboxy, int nboxz, double H, double L, int Nloc, double lambda, std::string kernelFunction);
		TPMatrixKernelEstimator(int nboxy, int nboxz, double H, double L, int Nloc, double lambda, double (*kernelFunction)(double y, double z));
		void Estimate(const Particles2D& particles);
};

class TPMatrixBoxEstimator : public TPMatrixEstimator
{
	private:
		double mH0, mL0;

	public:
		TPMatrixBoxEstimator(int nboxy, int nboxz, double H0, double H, double L0, double L, int Nloc);
		void Estimate(const Particles2D& particles);
		Matrix Count(const Particles2D& particles);
};

double Gaussian(double y, double z);
double Epanechnikov(double y, double z);
#endif