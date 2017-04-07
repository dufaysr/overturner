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
	BISolver solver(Nloc, yStart, zStart);
	Particles2D part = solver.Run(model);
}

void StudyCaseTrajectoriesAdim(std::string model, double yStart, double zStart)
{
	BISolver solver(Nloc, yStart, zStart);
	Particles2D part = solver.RunAdim(model);
}

void StudyCaseConcentration(std::string model, std::string estimator, double yStart, double zStart, int dimy, int dimz)
{
	BISolver solver(Nloc, yStart, zStart);
	Particles2D part = solver.RunAdim();

	Estimator *estim;
	transform(estimator.begin(), estimator.end(), estimator.begin(), ::tolower);
	if(estimator == "epanechnikov")
		estim = new KernelEstimator(dimy,dimz,.1*pow(part.mN,-1./6.),"Epanechnikov");
	else if (estimator == "gaussian")
		estim = new KernelEstimator(dimy,dimz,.1*pow(part.mN,-1./6.),"Gaussian");
	else if (estimator == "box")
		estim = new BoxEstimator(dimy,dimz);
	else{
		std::cerr << "Unknown estimator type. Valid estimators are \"epanechnikov\", \"gaussian\" and \"box\".";
		abort();
	}
	estim->Estimate(part);
	estim->Print("out/" + model + "/" + estimator + ".out");
	delete estim;
}

void StudyCaseTransitionProbabilities(std::string model, std::string estimator, int dimy, int dimz)
{
	double *yStart = new double [dimy];
	double dy = 1./dimy;
	for (int iy=0; iy<dimy; iy++)
		yStart[iy] = (iy+.5)*dy;

	double *zStart = new double [dimz];
	double dz = 1./dimz;
	for (int iz=0; iz<dimz; iz++)
		zStart[iz] = (iz+.5)*dz;

	// BISolver solver(Nloc, yStart, zStart, dimy, dimz);
	// Particles2D part = solver.RunAdim();
	Particles2D part(Nloc, yStart, zStart, dimy, dimz);

	GlobalEstimator *estim;
	transform(estimator.begin(), estimator.end(), estimator.begin(), ::tolower);
	if(estimator == "epanechnikov")
		estim = new GlobalKernelEstimator(dimy,dimz,.1*pow(Nloc,-1./6.),"Epanechnikov");
	else if (estimator == "gaussian")
		estim = new GlobalKernelEstimator(dimy,dimz,.1*pow(Nloc,-1./6.),"Gaussian");
	else if (estimator == "box")
		estim = new GlobalBoxEstimator(dimy,dimz);
	else{
		std::cerr << "Unknown estimator type. Valid estimators are \"epanechnikov\", \"gaussian\" and \"box\".";
		abort();
	}
	estim->Estimate(part);
	estim->Print("out/" + model + "/global_" + estimator + ".out");
	
	delete[] yStart;
	delete[] zStart;
	delete estim;
}