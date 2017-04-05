/*
  Utilities.hpp
  "overturner"

  Created by Renaud Dufays on 17/03/17.
  Copyright © 2017. All rights reserved.
*/

#ifndef UTILITIESHEADERFILE
#define UTILITIESHEADERFILE

#include <iostream>
#include <fstream>
#include <string>

namespace constants
{
	// default model constants. Other models can be loaded using an ini file with readIniFile().
	double Kh = 1e2;
	double Kv1 = 1e-1;
	double Kv2 = 5e-5;
	double Kv3 = 1e-3;
	double H = 4e3;
	double L = 12e6;
	double y0Prime = .9;
	double y0 = y0Prime*L;
	double z0Prime = 7./8.;
	double z0 = z0Prime*H;
	double Psi = 5;
}

void ReadIniFile(std::string filename);
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