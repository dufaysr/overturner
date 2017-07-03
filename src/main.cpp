/*
  main.cpp
  "overturner"

  Created by Renaud Dufays on 17/03/17.
  Copyright © 2017. All rights reserved.
*/

// #define TEST
#define PROBLEM2BOX
// #define KHKV2BOX

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
	/* trajectories */
	// double dt = 3600;
	// double year = 365*24*3600;
	// double T = 1000*year;
	// double alpha = .75;
	// Problem2Box prob(T,dt,alpha);
	// StudyCaseTrajectories(prob,"data2box",10,-13e6,4.5e3);
	
	/* Transition probability matrices */
	double dt = 3600;
	double year = 365*24*3600;
	double T = 1*year;
	double alpha[4] = {0,.5,.9,1};
	int nameindex[4] = {0,5,9,1};

	int nboxy = 30;
	int nboxz = 10;
	int nlocy = 100, nlocz = 100;

	std::string model;
	const int nTimes = 1;
	double Times[nTimes] = {1.*year};

	for (int i=0; i<4; i++){
		Problem2Box prob(T,dt,alpha[i]);
		model = "problem2box_a" + std::to_string(nameindex[i]);
		StudyCaseTransitionProbabilities(prob, model, nboxy, nboxz, nlocy, nlocz, Times, nTimes, true);
	}	
	return 0;
}

#elif defined(KHKV2BOX)
int main(int argc, char *argv[])
{
	int nboxy = 30;
	int nboxz = 10;

	Problem2Box prob(0.,0.,.5);
	double dy = (prob.getL1()-prob.getL0())/nboxy;
	double dz = (prob.getH1()-prob.getH0())/nboxz;
	Matrix Kh(nboxy,nboxz);
	Matrix Kv(nboxy,nboxz);
	double y, z;

	for (int iy=0; iy<nboxy; iy++){
		y = prob.getL0() + iy*dy + dy/2;
		for (int iz=0; iz<nboxz; iz++){
			z = prob.getH0() + iz*dz + dz/2;
			Kh(iy,iz) = prob.getKh(y,z);
			Kv(iy,iz) = prob.getKv(y,z);
		}
	}

	std::string fileKh = wd::root + "out/Kh2box_a5.out";
	std::string fileKv = wd::root + "out/Kv2box_a5.out";
	Kh.Print(fileKh,false);
	Kv.Print(fileKv,false);

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