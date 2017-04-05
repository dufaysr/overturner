/*
  Utilities.hpp
  "overturner"

  Created by Renaud Dufays on 17/03/17.
  Copyright © 2017. All rights reserved.
*/

#ifndef UTILITIESHEADERFILE
#define UTILITIESHEADERFILE

namespace constants
{
	const double Kh = 1e2;
	const double Kv1 = 1e-1;
	const double Kv2 = 5e-5;
	const double Kv3 = 1e-3;
	const double H = 4e3;
	const double L = 12e6;
	const double y0Prime = .9;
	const double y0 = y0Prime*L;
	const double z0Prime = 7./8.;
	const double z0 = z0Prime*H;
	const double Psi = 5;
}

double GetPhi(double xsi, double xsi0);
double GetdPhi(double xsi, double xsi0);
double GetV(double y, double z);
double GetVPrime(double yPrime, double zPrime);
double GetW(double y, double z);
double GetWPrime(double yPrime, double zPrime);
double Kv(double y, double z);
double PevInv(double yPrime, double zPrime);
double PehInv(double yPrime, double zPrime);

#endif