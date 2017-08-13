/*
  main.cpp
  "overturner"

  Created by Renaud Dufays on 17/03/17.
  Copyright © 2017. All rights reserved.
*/

#include "mpi.h"
#include "studyCases.hpp"

int main(int argc, char *argv[])
{
	MPI_Init(&argc,&argv);
	
	std::string model = "timmermans";
	double T = 0.; // not used
	double dt = 3600;
	OverturnerProblem prob(T,dt);

	int nboxy = 15, nboxz = 10;
	int nyloc = 10, nzloc = 10;

	double year = 365*24*3600;
	// const int nTimes = 11;
	// double Times[nTimes] = {1.*year,10.*year,20.*year,30.*year,40.*year,50.*year,60.*year,70.*year,80.*year,90.*year,100.*year};
	const int nTimes = 2;
	double Times[nTimes] = {year, 2*year};

	StudyCaseTransitionProbabilitiesMPI(prob, model, nboxy, nboxz, nyloc, nzloc, Times, nTimes, true);

	
	MPI_Finalize();
}