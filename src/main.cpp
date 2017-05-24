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

#define HANDLES_FLAG if (flag == 1)\
						return 1;\
					if (flag == 2)\
						return 0;\
					if (flag < 0){\
						std::cerr << "Not enough parameters specified.\n" << std::endl;\
						show_usage(argv[0]);\
					}

#define TEST


int main(int argc, char *argv[])
{
#ifndef TEST
	if (argc < 2)
	{
    	show_usage(argv[0]);
    	return 1;
  	}
  	int flag = 0;
  	std::string studycase = argv[1];
  	if ((studycase == "-t") || (studycase == "--trajectories"))
  	{
  		std::string model;
    	bool adim = true;
    	int Nloc;
    	double yStart, zStart;
  		flag = get_args_traj(argc, argv, model, Nloc, yStart, zStart, adim);
  		HANDLES_FLAG
		OverturnerProblem prob(model);
		if (adim){
			OverturnerProblemAdim probadim(prob);
			StudyCaseTrajectoriesAdim(probadim,model,Nloc,yStart,zStart);
		}
		else{
			StudyCaseTrajectories(prob,model,Nloc,yStart,zStart);
		}
	}
	else if ((studycase == "-c") || (studycase == "--concentration"))
	{
		std::string model;
		int Nloc, dimy, dimz;
		double yStart, zStart;
		flag = get_args_conc(argc, argv, model, Nloc, dimy, dimz, yStart, zStart);
		HANDLES_FLAG
		OverturnerProblem prob(model);
		OverturnerProblemAdim probadim(prob);
		StudyCaseConcentrationAdim(probadim,model,"box",Nloc,yStart,zStart,dimy,dimz);
	}
	else if ((studycase == "-M") || (studycase == "--transition_proba"))
	{
		std::string model;
		int nybox, nzbox, dimy, dimz;
		flag = get_args_tp(argc, argv, model, dimy, dimz, nybox, nzbox);
		HANDLES_FLAG
		OverturnerProblem prob(model);
		OverturnerProblemAdim probadim(prob);
		StudyCaseTransitionProbabilities(probadim, model, "box", dimy, dimz, nybox, nzbox, true);
	}
	else 
		show_usage(argv[0]);
	return 0;
#else
	// StudyCaseTestProblem();
	StudyCaseTestProblemSemiInf();
#endif
}
