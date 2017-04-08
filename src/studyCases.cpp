/*
  studyCases.cpp
  "overturner"

  Created by Renaud Dufays on 07/04/17
  Copyright © 2017. All rights reserved.
*/

#include "studyCases.hpp"

using namespace parameters;

void StudyCaseTrajectories(std::string model, double yStart, double zStart)
{
	std::cout << "\nRunning StudyCaseTrajectories..." << std::endl;
	BISolver solver(Nloc, yStart, zStart);
	Particles2D part = solver.Run(model);
	std::cout << "\nStudyCaseTrajectories runned successfully." << std::endl;
}

void StudyCaseTrajectoriesAdim(std::string model, double yStart, double zStart)
{
	std::cout << "\nRunning StudyCaseTrajectoriesAdim..." << std::endl;
	BISolver solver(Nloc, yStart, zStart);
	Particles2D part = solver.RunAdim(model);
	std::cout << "\nStudyCaseTrajectoriesAdim runned successfully." << std::endl;
}

void StudyCaseConcentration(std::string model, std::string estimator, double yStart, double zStart, int dimy, int dimz)
{
	std::cout << "\nRunning StudyCaseConcentration..." << std::endl;
	std::cout << "1/3 : Generating trajectories..." << std::endl;
	BISolver solver(Nloc, yStart, zStart);
	Particles2D part = solver.RunAdim();

	std::cout << "2/3 : Computing the concentrations..." << std::endl;
	Estimator *estim;
	transform(estimator.begin(), estimator.end(), estimator.begin(), ::tolower);
	if(estimator == "epanechnikov")
		estim = new KernelEstimator(dimy,dimz,.1*pow(part.mN,-1./6.),"Epanechnikov");
	else if (estimator == "gaussian")
		estim = new KernelEstimator(dimy,dimz,.1*pow(part.mN,-1./6.),"Gaussian");
	else if (estimator == "box")
		estim = new BoxEstimator(dimy,dimz);
	else{
		std::cerr << "Unknown estimator type. Valid estimators are \"epanechnikov\", \"gaussian\" and \"box\"." << std::endl;
		abort();
	}
	estim->Estimate(part);
	std::cout << "3/3 : Writing in the files..." << std::endl;
	estim->Print("out/" + model + "/" + estimator + ".out");
	delete estim;
	std::cout << "\nStudyCaseConcentration runned successfully." << std::endl;
}

void StudyCaseTransitionProbabilities(std::string model, std::string estimator, int dimy, int dimz)
{
	std::cout << "\nRunning StudyCaseTransitionProbabilities..." << std::endl;
	std::cout << "1/4 : Generating the initial conditions..." << std::endl;
	double *yStart = new double [dimy];
	double dy = 1./dimy;
	for (int iy=0; iy<dimy; iy++)
		yStart[iy] = (iy+.5)*dy;

	double *zStart = new double [dimz];
	double dz = 1./dimz;
	for (int iz=0; iz<dimz; iz++)
		zStart[iz] = (iz+.5)*dz;

	std::cout << "2/4 : Generating trajectories..." << std::endl;
	BISolver solver(Nloc, yStart, zStart, dimy, dimz);
	Particles2D part = solver.RunAdim();
	// Particles2D part(Nloc, yStart, zStart, dimy, dimz);

	std::cout << "3/4 : Computing the transition probability matrix..." << std::endl;
	GlobalEstimator *estim;
	transform(estimator.begin(), estimator.end(), estimator.begin(), ::tolower);
	if(estimator == "epanechnikov")
		estim = new GlobalKernelEstimator(dimy,dimz,.1*pow(Nloc,-1./6.),"Epanechnikov");
	else if (estimator == "gaussian")
		estim = new GlobalKernelEstimator(dimy,dimz,.1*pow(Nloc,-1./6.),"Gaussian");
	else if (estimator == "box")
		estim = new GlobalBoxEstimator(dimy,dimz);
	else{
		std::cerr << "Unknown estimator type. Valid estimators are \"epanechnikov\", \"gaussian\" and \"box\"." << std::endl;
		abort();
	}
	estim->Estimate(part);
	std::cout << "4/4 : Writing in the files..." << std::endl;
	estim->Print("out/" + model + "/global_" + estimator + ".out");
	
	delete[] yStart;
	delete[] zStart;
	delete estim;
	std::cout << "\nStudyCaseTransitionProbabilities runned successfully." << std::endl;
}