/*
  Utilities.hpp
  "sde2D"

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
#include <algorithm>
#include "workingdirectory.hpp"

// Only used for one constructor of "OverturnerProblem"
double parseMathExpr(std::string value, double H, double L);

// check that the file exist and if yes, proposes the user to enter another
// name for his output file, or to overwrite the existing one
std::ofstream openOutputFile(std::string filename, bool binary=false);
#endif