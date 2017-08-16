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
#include <algorithm>
#include "workingdirectory.hpp"

double parseMathExpr(std::string value, double H, double L);
// void ReadIniFile(std::string model);
std::ofstream openOutputFile(std::string filename, bool binary=false);
void show_usage(std::string name);
int get_args_traj(int argc, char *argv[], std::string& model, int& Nloc, double& y0, double& z0, bool& adim);
int get_args_conc(int argc, char *argv[], std::string& model, int& Nloc, int& ncelly, int& ncellz, double& y0, double& z0);
int get_args_tp(int argc, char *argv[], std::string& model, int& ncelly, int& ncellz, int& nyloc, int& nzloc);
#endif