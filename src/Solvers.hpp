/*
  Solvers.hpp
  "overturner"

  Created by Renaud Dufays on 17/03/17.
  Copyright © 2017. All rights reserved.
*/

#ifndef SOLVERSHEADERFILE
#define SOLVERSHEADERFILE

#include "Particles2D.hpp"
#include "Utilities.hpp"
#include "AdvDiffProblem.hpp"
#include <random>
#include <iostream>
#include <iomanip>
#include <algorithm>

/*-------------- Abstract base class : Solver ---------------------*/
class Solver
{
	protected:
		Particles2D mParticles;
		unsigned seed;
		std::default_random_engine generator;
		std::normal_distribution<double> wiener;
		double mdt; // time step of the numerical scheme

	public:
		Solver(const Particles2D& particles, double dt);
		virtual ~Solver(){};
		virtual void UpdatePosition(const AbstractAdvDiffProblem& prob) =0;		// Pure virtual function
		Particles2D Run(const AbstractAdvDiffProblem& prob, double Time);
		Particles2D Run(const AbstractAdvDiffProblem& prob, double Time,
						std::string outputdir, int nPrint = 5, bool printInit = true, bool binary = false);
		void DisplayParticles() const;
		void PrintParticles(std::ofstream& fT, std::ofstream& fY, std::ofstream& fZ) const;
		void PrintParticlesBinary(std::ofstream& fT, std::ofstream& fY, std::ofstream& fZ) const;
		void TestWiener();
};

/*--------------Derived class from Solver : Euler-Maruyama (EM) solver-----------*/
class EMSolver : public Solver
{
	public:
		EMSolver(const Particles2D& particles, double dt);
		void UpdatePosition(const AbstractAdvDiffProblem& prob);
};

/*--------------Derived class from Solver : Backward Ito (BI) solver-----------*/
class BISolver : public Solver
{
	public:
		BISolver(const Particles2D& particles, double dt);
		void UpdatePosition(const AbstractAdvDiffProblem& prob);
};

#endif