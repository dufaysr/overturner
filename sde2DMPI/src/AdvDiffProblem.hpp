/*
  Particles2D.hpp
  "overturner"

  Created by Renaud Dufays on 20/05/17.
  Copyright © 2017. All rights reserved.
*/

#ifndef ADVDIFFPROBLEMHEADERDEF
#define ADVDIFFPROBLEMHEADERDEF

#include <fstream>
#include <string>


class AbstractAdvDiffProblem
{
	protected:
		double mH0, mH1; // boundaries of the domain in the z-direction : H0 <= z <= H1
		double mL0, mL1; // boundaries of the domain in the y-direction : L0 <= y <= L1
		double mT; // Final time of simulation
		double mdt; // time step of the numerical scheme
	public:
		AbstractAdvDiffProblem(double H0, double H1, double L0, double L1, double T, double dt);
		virtual ~AbstractAdvDiffProblem(){};
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
	friend class TestProblem;
	private:
		double my0;
		double mz0;
		double mKh;
		double mKv1, mKv2, mKv3;
		double mPsi;

	public:
		OverturnerProblem(double T, double dt); // default case is using C. Timmermans values for the parameters
		double getKh(double y, double z) const;
		double getKv(double y, double z) const;
		double getV(double y, double z) const;
		double getW(double y, double z) const;
		void printInfo(std::ofstream& f) const;
};

inline double phi(double xsi, double xsi0);
inline double dphi(double xsi, double xsi0);
double v(double y, double y0, double z, double z0, double L, double H);
double w(double y, double y0, double z, double z0, double L, double H);
#endif