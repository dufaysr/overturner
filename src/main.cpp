/*
  main.cpp
  "overturner"

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

using namespace parameters;
int main(int argc, char *argv[])
{
	std::string model;
	if (argc == 2)
		model = argv[1];
	else
		model = "model"; //default model, if not specified as input argument
	// ReadIniFile(model);

	OverturnerProblem prob("model");
	StudyCaseTrajectories(prob,model,10,.5*prob.getL1(),prob.getH1()-1.);
	// StudyCaseConcentration(model,"box",Nloc,.5, .9, 10, 10);
	// StudyCaseTransitionProbabilities(model, "box", dimy, dimz, true);
	return 0;
}
