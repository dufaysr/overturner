/*
  Particles2D.hpp
  "sde2D"

  Created by Renaud Dufays on 20/05/17.
  Copyright © 2017. All rights reserved.
*/

#ifndef ADVDIFFPROBLEMHEADERDEF
#define ADVDIFFPROBLEMHEADERDEF

#include "Utilities.hpp"
#include "workingdirectory.hpp"
#include "AdvDiffProblem.hpp"
#include "Structures.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <cstdlib>
#include <algorithm>

/*
	VIRTUAL BASE CLASS
	All the "problem classes" must inherit (directly or undirectly) from this class.
	When defining a class that inherits from this class, at least the two virtual methods
	"getK" and "getU" must be implemented.
	In some cases, "getB" can be overriden to save computational time.
*/
class AbstractAdvDiffProblem
{
	protected:
		double mH0, mH1; // boundaries of the domain in the z-direction : H0 <= z <= H1
		double mL0, mL1; // boundaries of the domain in the y-direction : L0 <= y <= L1

	public:
		AbstractAdvDiffProblem(double H0, double H1, double L0, double L1);
		virtual ~AbstractAdvDiffProblem(){};
		double getH0() const;
		double getH1() const;
		double getL0() const;
		double getL1() const;
		virtual SymMatrix getK(double y, double z) const=0; // diffusivity tensor
		virtual LowerTriMatrix getB(double y, double z) const; // 2K = BB'
		virtual Vec2 getU(double y, double z) const=0; // velocity vector
		virtual void printInfo(std::ofstream& f) const; // prints the domain data's in stream f
};

/*
	This class represents the two test problems presented in the appendix of the master's thesis.
	For the first test case, call the constructor with the argument domain = "infinite".
	For the first test case, call the constructor with the argument domain = "semi-infinite".
*/
class TestProblem : public AbstractAdvDiffProblem
{
	private:
		double mKyy, mKzz;
		double mV, mW;
		int mJ;

	public:
		TestProblem(double Ly, double Lz, double Kyy, double Kzz, double V, double W, int J, std::string domain = "infinite");
		SymMatrix getK(double y, double z) const;
		LowerTriMatrix getB(double y, double z) const;
		Vec2 getU(double y, double z) const;
		void printInfo(std::ofstream& f) const;
};

/*
	This class represents the bi-overturner problems.
*/
class Problem2Box : public AbstractAdvDiffProblem
{
	private:
		double mzstar;
		double my0;
		double mz0;
		double mKhm; // horizontal diffusivity in the exchange zone
		double mKhp; // horizontal diffusivity outside the exchange zone
		double mKv; // vertical diffusivity
		double mPsi;

	public:
		Problem2Box(double alpha); // default case is using C. Timmermans values for the parameters
		double getzstar() const;
		double gety0() const;
		double gety0m() const;
		double gety0p() const;
		double getz0() const;
		SymMatrix getK(double y, double z) const;
		LowerTriMatrix getB(double y, double z) const;
		Vec2 getU(double y, double z) const;
		void printInfo(std::ofstream& f) const;
};

/*
	This class represents the overturner problems.
*/
class OverturnerProblem : public AbstractAdvDiffProblem
{
	friend class TestProblem;
	private:
		double my0;
		double mz0;
		double mKh;
		double mKv1, mKv2, mKv3;
		double mPsi;

	public:
		OverturnerProblem(); // default case is using C. Timmermans values for the parameters
		OverturnerProblem(std::string model); // initialize the problem from the values in file model (see below for an example)
		SymMatrix getK(double y, double z) const;
		LowerTriMatrix getB(double y, double z) const;
		Vec2 getU(double y, double z) const;
		void printInfo(std::ofstream& f) const;
		void Display() const;
};


/*-----------------------Auxiliary functions---------------------------------------------------------------------*/
// Velocity field "overturner circulation"
double phi(double xsi, double xsi0);
double dphi(double xsi, double xsi0);
double v(double y, double y0, double z, double z0, double L, double H);
double w(double y, double y0, double z, double z0, double L, double H);
// Velocity field of the bi-overturner problems
double v2box(double y, double y0, double z, double z0, double L, double H);
double w2box(double y, double y0, double z, double z0, double L, double H);
#endif