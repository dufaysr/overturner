/*
  main.cpp
  "overturner"

  Created by Renaud Dufays on 17/03/17.
  Copyright © 2017. All rights reserved.
*/

#define NOARGS

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

#define HANDLES_FLAG if (flag == 1)\
						return 1;\
					if (flag == 2)\
						return 0;\
					if (flag < 0){\
						std::cerr << "Not enough parameters specified.\n" << std::endl;\
						show_usage(argv[0]);\
						return 0;\
					}

#if defined(NOARGS)
					
int main(int argc, char *argv[])
{
	// StudyCaseTestProblem();
	// StudyCaseTestProblemSemiInf();
	// StudyCaseOverturnerTPnTimes();
	StudyCaseProblem2BoxTraj();
	// StudyCaseProblem2BoxTP();

	return 0;
}

#else
int main(int argc, char *argv[])
{
	if (argc < 2) // i.e. no argument provided
	{
    	show_usage(argv[0]);
    	return 1;
  	}
  	int flag = 0;
  	std::string computeFct = argv[1];
  	if ((computeFct == "-t") || (computeFct == "--trajectories"))
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
			ComputeTrajectories(probadim,model,Nloc,yStart,zStart);
		}
		else{
			ComputeTrajectories(prob,model,Nloc,yStart,zStart);
		}
	}
	else if ((computeFct == "-c") || (computeFct == "--concentration"))
	{
		std::string model;
		int Nloc, dimy, dimz;
		double yStart, zStart;
		flag = get_args_conc(argc, argv, model, Nloc, dimy, dimz, yStart, zStart);
		HANDLES_FLAG
		OverturnerProblem prob(model);
		OverturnerProblemAdim probadim(prob);
		ComputeConcentration(probadim,model,"box",Nloc,yStart,zStart,dimy,dimz);
	}
	else if ((computeFct == "-M") || (computeFct == "--transition_proba"))
	{
		std::string model;
		int nyloc, nzloc, nboxy, nboxz;
		flag = get_args_tp(argc, argv, model, nboxy, nboxz, nyloc, nzloc);
		std::cout << "model : " << model << "\n";
		std::cout << "nboxy : " << nboxy << "\n";
		std::cout << "nboxz : " << nboxz << "\n";
		std::cout << "nyloc : " << nyloc << "\n";
		std::cout << "nzloc : " << nzloc << "\n";
		HANDLES_FLAG
		OverturnerProblem prob(model);
		const OverturnerProblemAdim probadim(prob);
		ComputeTransitionProbabilities(probadim, model, "box", nboxy, nboxz, nyloc, nzloc, true);
	}
	else 
		show_usage(argv[0]);
	return 0;
}
#endif