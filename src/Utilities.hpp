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
#include <cstring>
#include <iomanip>
#include <ctime>

namespace parameters
{
	// model parameters have global scope
	extern double Kh;
	extern double Kv1;
	extern double Kv2;
	extern double Kv3;
	extern double H;
	extern double L;
	extern double y0Prime;
	extern double y0;
	extern double z0Prime;
	extern double z0;
	extern double Psi;
	extern int Nloc;
	extern double dt;
	extern double T;
	extern double dtPrime;
	extern double TPrime;
}

void ReadIniFile(std::string model);
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