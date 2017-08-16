/*
  main.cpp
  "sde2D"

  Created by Renaud Dufays on 17/03/17.
  Copyright © 2017. All rights reserved.
*/

#include <iostream>
#include <chrono>
#include <random>
#include <fstream>
#include "Particles2D.hpp"
#include "Solvers.hpp"
#include "Utilities.hpp"
#include "Estimator.hpp"
#include "studyCases.hpp"
#include "Compute.hpp"
					
int main(int argc, char *argv[])
{
	// StudyCaseTestProblem();
	StudyCaseTestProblemSemiInf();
	// StudyCaseOverturnerTPnTimes();
	// StudyCaseProblem2BoxTraj();
	// StudyCaseProblem2BoxTP();
	// StudyCaseP2BConcentration2Comp();

	return 0;
}