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

	public:
		Solver(int Nloc, double yStart, double zStart);
		Solver(int Nloc, double* yStart, double* zStart, int n);
		Solver(int Nloc, double* yStart, double* zStart, int ny, int nz);
		Solver(const Particles2D& particles);
		virtual void UpdatePosition() =0;		// Pure virtual function
		virtual void UpdatePositionAdim() =0;	// Pure virtual function
		Particles2D& Run();
		Particles2D& Run(std::string model, int nPrint = 5);
		Particles2D& RunAdim();
		Particles2D& RunAdim(std::string model, int nPrint = 5);
		void DisplayParticles() const;
		void PrintParticles(std::string model) const;
		void TestWiener();
};

/*--------------Derived class from Solver : Euler-Maruyama (EM) solver-----------*/
class EMSolver : public Solver
{
	public:
		EMSolver(int Nloc, double yStart, double zStart);
		EMSolver(int Nloc, double* yStart, double* zStart, int n);
		EMSolver(int Nloc, double* yStart, double* zStart, int ny, int nz);
		EMSolver(const Particles2D& particles);
		void UpdatePosition();
		void UpdatePositionAdim();
};

/*--------------Derived class from Solver : Backward Ito (BI) solver-----------*/
class BISolver : public Solver
{
	public:
		BISolver(int N, double yStart, double zStart);
		BISolver(int N, double* yStart, double* zStart, int n);
		BISolver(int N, double* yStart, double* zStart, int ny, int nz);
		BISolver(const Particles2D& particles);
		void UpdatePosition();
		void UpdatePositionAdim();
};

#endif