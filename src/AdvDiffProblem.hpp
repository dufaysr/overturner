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
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <algorithm>


class AbstractAdvDiffProblem
{
	protected:
		double mH0, mH1; // boundaries of the domain in the z-direction : H0 <= z <= H1
		double mL0, mL1; // boundaries of the domain in the y-direction : L0 <= y <= L1
		double mT; // Final time of simulation
		double mdt; // time step of the numerical scheme
	public:
		AbstractAdvDiffProblem(double H0, double H1, double L0, double L1, double T, double dt);
		double getH0() const;
		double getH1() const;
		double getL0() const;
		double getL1() const;
		double getT()  const;
		double getdt() const;
		virtual double getKh(double y, double z) const=0;
		virtual double getKv(double y, double z) const=0;
		virtual double getV(double y, double z) const=0;
		virtual double getW(double y, double z) const=0;
		virtual void printInfo(std::ofstream& f) const=0;
};

class OverturnerProblem : public AbstractAdvDiffProblem
{
	friend class OverturnerProblemAdim; // useful in the constructor that builds adim from dim prob
	private:
		double my0;
		double mz0;
		double mKh;
		double mKv1, mKv2, mKv3;
		double mPsi;

	public:
		OverturnerProblem(double T, double dt); // default case is using C. Timmermans values for the parameters
		OverturnerProblem(std::string model);
		double getKh(double y, double z) const;
		double getKv(double y, double z) const;
		double getV(double y, double z) const;
		double getW(double y, double z) const;
		void printInfo(std::ofstream& f) const;
		void Display() const;
};


/*    Adimensionnal Advection-Diffusion problem    */
class AbstractAdvDiffProblemAdim
{
	protected:
		// the adimensionnal domain is [0,1] x [0,1]
		double mTPrime; // Final time of simulation
		double mdtPrime; // time step of the numerical scheme
		double mH, mL; // vertical and meridional characteristic lengths (used in VPrime and WPrime)

	public:
		AbstractAdvDiffProblemAdim(double TPrime, double dtPrime, double H, double L);
		double getTPrime()  const;
		double getdtPrime() const;
		virtual double getPehInv(double y, double z) const=0;
		virtual double getPevInv(double y, double z) const=0;
		virtual double getVPrime(double y, double z) const=0;
		virtual double getWPrime(double y, double z) const=0;
		virtual void printInfo(std::ofstream& f) const=0;
};

class OverturnerProblemAdim : public AbstractAdvDiffProblemAdim
{
	private:
		double my0Prime;
		double mz0Prime;
		double mPehInv;
		double mPev1Inv, mPev2Inv, mPev3Inv;
	public:
		OverturnerProblemAdim(double T, double dt); // default case is using C. Timmermans values for the parameters
		OverturnerProblemAdim(std::string model);
		OverturnerProblemAdim(OverturnerProblem& prob);
		double getPehInv(double yPrime, double zPrime) const;
		double getPevInv(double yPrime, double zPrime) const;
		double getVPrime(double yPrime, double zPrime) const;
		double getWPrime(double yPrime, double zPrime) const;
		void printInfo(std::ofstream& f) const;
		void Display() const;
};

inline double phi(double xsi, double xsi0);
inline double dphi(double xsi, double xsi0);
double v(double y, double y0, double z, double z0, double L, double H);
double w(double y, double y0, double z, double z0, double L, double H);
#endif