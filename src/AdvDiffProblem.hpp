/*
  Particles2D.hpp
  "overturner"

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
		virtual SymMatrix getK(double y, double z) const=0;
		virtual LowerTriMatrix getB(double y, double z) const;
		virtual Vec2 getU(double y, double z) const=0;
		virtual void printInfo(std::ofstream& f) const=0;
};

class OverturnerProblem : public AbstractAdvDiffProblem
{
	friend class OverturnerProblemAdim; // useful in the constructor that builds adim from dim prob
	friend class TestProblem;
	private:
		double my0;
		double mz0;
		double mKh;
		double mKv1, mKv2, mKv3;
		double mPsi;

	public:
		OverturnerProblem(); // default case is using C. Timmermans values for the parameters
		OverturnerProblem(std::string model);
		SymMatrix getK(double y, double z) const;
		LowerTriMatrix getB(double y, double z) const;
		Vec2 getU(double y, double z) const;
		void printInfo(std::ofstream& f) const;
		void Display() const;
};

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

class Problem2Box : public AbstractAdvDiffProblem
{
	private:
		double mzstar;
		double my0;
		double mz0;
		double mKhm, mKhp;
		double mKv;
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

/*------------------------------------------------------------------------------------------------------*/
double phi(double xsi, double xsi0);
double dphi(double xsi, double xsi0);
double v(double y, double y0, double z, double z0, double L, double H);
double w(double y, double y0, double z, double z0, double L, double H);
double v2box(double y, double y0, double z, double z0, double L, double H);
double w2box(double y, double y0, double z, double z0, double L, double H);
#endif