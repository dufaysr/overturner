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

/*
	The studyCase functions are the different "mains" used for the master's thesis
	
	The "high level" functions of this code are the "compute functions", described in Compute.hpp
*/


int main(int argc, char *argv[])
{
	// StudyCaseTestProblem();
	// StudyCaseTestProblemSemiInf();
	// StudyCaseProblem2BoxTraj();
	// StudyCaseProblem2BoxTP();
	// StudyCaseP2BConcentration2Comp();
	StudyCaseOverturnerTPnTimes();

	return 0;
}