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

class Estimator
{
	protected:
		int mDim1, mDim2;
		Field mEstimator;

	public:
		Estimator(int dim1, int dim2);
		virtual void Estimate(Particles2D particles) =0;
		void Print(std::string filename) const;
};

class KernelEstimator : public Estimator
{
	private:
		double mLambda;
		double (*mKernel)(double y, double z);

	public:
		KernelEstimator(int dim1, int dim2, double lambda, std::string kernelFunction);
		KernelEstimator(int dim1, int dim2, double lambda, double (*kernelFunction)(double y, double z));
		void Estimate(Particles2D particles);
};

class BoxEstimator : public Estimator
{
	public:
		BoxEstimator(int dim1, int dim2);
		void Estimate(Particles2D particles);
};

double Gaussian(double y, double z);
double Epanechnikov(double y, double z);
#endif