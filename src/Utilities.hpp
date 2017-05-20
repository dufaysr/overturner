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
#include "workingdirectory.hpp"

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
	extern int nybox;
	extern int nzbox;
	extern int Nbox;
	extern int Nloc;
	extern int dimy;
	extern int dimz;
	extern double dt;
	extern double T;
	extern double dtPrime;
	extern double TPrime;
}

// void ReadIniFile(std::string model);
std::ofstream openOutputFile(std::string filename, bool binary=false);

#endif