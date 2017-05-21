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
		int mNboxy, mNboxz;
		double mH, mL;
		Field mEstimator;

	public:
		Estimator(int nboxy, int nboxz, double H, double L);
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
		KernelEstimator(int nboxy, int nboxz, double H, double L, double lambda, std::string kernelFunction);
		KernelEstimator(int nboxy, int nboxz, double H, double L, double lambda, double (*kernelFunction)(double y, double z));
		void Estimate(const Particles2D& particles);
		void EstimateAdim(const Particles2D& particles);
};

class BoxEstimator : public Estimator
{
	public:
		BoxEstimator(int nboxy, int nboxz, double H, double L);
		void Estimate(const Particles2D& particles);
		void EstimateAdim(const Particles2D& particles);
};

class GlobalEstimator
{
	protected:
		int mNboxy, mNboxz, mNbox;
		double mH, mL;
		Field mEstimator;

	public:
		GlobalEstimator(int nboxy, int nboxz, double H, double L, int Nbox);
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
		GlobalKernelEstimator(int nboxy, int nboxz, double H, double L, int Nbox, double lambda, std::string kernelFunction);
		GlobalKernelEstimator(int nboxy, int nboxz, double H, double L, int Nbox, double lambda, double (*kernelFunction)(double y, double z));
		void Estimate(const Particles2D& particles);
		void EstimateAdim(const Particles2D& particles);
};

class GlobalBoxEstimator : public GlobalEstimator
{
	public:
		GlobalBoxEstimator(int nboxy, int nboxz, double H, double L, int Nbox);
		void Estimate(const Particles2D& particles);
		void EstimateAdim(const Particles2D& particles);
};

double Gaussian(double y, double z);
double Epanechnikov(double y, double z);
#endif