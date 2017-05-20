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
#include "Field.hpp"
#include "Utilities.hpp"

class Estimator
{
	protected:
		int mDimy, mDimz;
		Field mEstimator;

	public:
		Estimator(int dimy, int dimz);
		virtual ~Estimator() {}
		virtual void Estimate(const Particles2D& particles) =0;
		virtual void EstimateAdim(const Particles2D& particles) =0;
		void Print(std::string filename, bool binary=false) const;
};

class KernelEstimator : public Estimator
{
	private:
		double mLambda;
		double (*mKernel)(double y, double z);

	public:
		KernelEstimator(int dimy, int dimz, double lambda, std::string kernelFunction);
		KernelEstimator(int dimy, int dimz, double lambda, double (*kernelFunction)(double y, double z));
		void Estimate(const Particles2D& particles);
		void EstimateAdim(const Particles2D& particles);
};

class BoxEstimator : public Estimator
{
	public:
		BoxEstimator(int dimy, int dimz);
		void Estimate(const Particles2D& particles);
		void EstimateAdim(const Particles2D& particles);
};

class GlobalEstimator
{
	protected:
		int mDimy, mDimz, mNbox;
		Field mEstimator;

	public:
		GlobalEstimator(int dimy, int dimz, int Nbox);
		virtual ~GlobalEstimator() {}
		virtual void Estimate(const Particles2D& particles) =0;
		virtual void EstimateAdim(const Particles2D& particles) =0;
		void Print(std::string filename, bool binary=false) const;
};

class GlobalKernelEstimator : public GlobalEstimator
{
	private:
		double mLambda;
		double (*mKernel)(double y, double z);

	public:
		GlobalKernelEstimator(int dimy, int dimz, int Nbox, double lambda, std::string kernelFunction);
		GlobalKernelEstimator(int dimy, int dimz, int Nbox, double lambda, double (*kernelFunction)(double y, double z));
		void Estimate(const Particles2D& particles);
		void EstimateAdim(const Particles2D& particles);
};

class GlobalBoxEstimator : public GlobalEstimator
{
	public:
		GlobalBoxEstimator(int dimy, int dimz, int Nbox);
		void Estimate(const Particles2D& particles);
		void EstimateAdim(const Particles2D& particles);
};

double Gaussian(double y, double z);
double Epanechnikov(double y, double z);
#endif