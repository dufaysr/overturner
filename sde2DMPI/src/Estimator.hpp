/*
  KernelEstimator.hpp
  "overturner"

  Created by Renaud Dufays on 04/04/17
  Copyright © 2017. All rights reserved.
*/

#ifndef KERNELESTIMATORHEADERDEF
#define KERNELESTIMATORHEADERDEF

#include <string>
#include "Particles2D.hpp"
#include "Matrix.hpp"

class Estimator
{
	protected:
		int mNcelly, mNcellz;
		double mH, mL;
		Matrix mEstimator;

	public:
		Estimator(int ncelly, int ncellz, double H, double L);
		virtual ~Estimator() {}
		virtual void Estimate(const Particles2D& particles) =0;
		void Print(std::ofstream& f, bool binary=false) const;
};

class BoxEstimator : public Estimator
{
	public:
		BoxEstimator(int ncelly, int ncellz, double H, double L);
		void Estimate(const Particles2D& particles);
};
#endif