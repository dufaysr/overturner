/*
  Solvers.hpp
  "overturner"

  Created by Renaud Dufays on 17/03/17.
  Copyright © 2017. All rights reserved.
*/

#ifndef SOLVERSHEADERFILE
#define SOLVERSHEADERFILE

#include "Particles2D.hpp"
#include "AdvDiffProblem.hpp"
#include <random>
#include <chrono>

/*-------------- Abstract base class : Solver ---------------------*/
class Solver
{
	protected:
		Particles2D mParticles;
		unsigned seed;
		std::default_random_engine generator;
		std::normal_distribution<double> wiener;

	public:
		Solver(const Particles2D& particles);
		virtual ~Solver(){};
		virtual void UpdatePosition(const AbstractAdvDiffProblem& prob) =0;		// Pure virtual function
		Particles2D Run(const AbstractAdvDiffProblem& prob, double Time);
};

/*--------------Derived class from Solver : Backward Ito (BI) solver-----------*/
class BISolver : public Solver
{
	public:
		BISolver(const Particles2D& particles);
		void UpdatePosition(const AbstractAdvDiffProblem& prob);
};

#endif