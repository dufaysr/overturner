/*
  main.cpp
  "overturner"

  Created by Renaud Dufays on 17/03/17.
  Copyright © 2017. All rights reserved.
*/

// #define TEST
#define PROBLEM2BOX

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
						return 0;\
					}

#if defined(TEST)
int main(int argc, char *argv[])
{
	// StudyCaseTestProblem();
	// StudyCaseTestProblemSemiInf();
	std::string model = "timmermans";
	OverturnerProblem prob(model);
	// OverturnerProblemAdim probadim(prob);
	// const double epsilon = .1;
	int nboxy = 15;
	int nboxz = 10;
	int nlocy = 100, nlocz = 100;
	// StudyCaseComputeNloc(probadim,epsilon,nboxy,nboxz);
	// StudyCaseComputeNloc(prob,epsilon,nboxy,nboxz);
	double year = 365*24*3600;
	const int nTimes = 11;
	double Times[nTimes] = {1.*year,10.*year,20.*year,30.*year,40.*year,50.*year,60.*year,70.*year,80.*year,90.*year,100.*year};
	StudyCaseTransitionProbabilities(prob, model, nboxy, nboxz, nlocy, nlocz, Times, nTimes, true);

	return 0;
}

#elif defined(PROBLEM2BOX)
int main(int argc, char *argv[])
{
	double dt = 3600;
	double year = 365*24*3600;
	double T = 1*year;
	double alpha = 1; // .9, .5, .1
	Problem2Box prob(T,dt,alpha);

	int nboxy = 30;
	int nboxz = 10;
	int nlocy = 100, nlocz = 100;

	std::string model = "problem2box";

	const int nTimes = 1;
	double Times[nTimes] = {1.*year};

	StudyCaseTransitionProbabilities(prob, model, nboxy, nboxz, nlocy, nlocz, Times, nTimes, true);

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
			StudyCaseTrajectories(probadim,model,Nloc,yStart,zStart);
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
		StudyCaseConcentration(probadim,model,"box",Nloc,yStart,zStart,dimy,dimz);
	}
	else if ((studycase == "-M") || (studycase == "--transition_proba"))
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
		StudyCaseTransitionProbabilities(probadim, model, "box", nboxy, nboxz, nyloc, nzloc, true);
	}
	else 
		show_usage(argv[0]);
	return 0;
}
#endif