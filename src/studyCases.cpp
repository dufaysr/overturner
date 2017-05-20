/*
  studyCases.cpp
  "overturner"

  Created by Renaud Dufays on 07/04/17
  Copyright © 2017. All rights reserved.
*/

#include "studyCases.hpp"

using namespace parameters;

void StudyCaseTrajectories(AbstractAdvDiffProblem& prob, std::string model, int Nloc, double yStart, double zStart)
{
	std::cout << "\nRunning StudyCaseTrajectories..." << std::endl;
	BISolver solver(Nloc, yStart, zStart);
	Particles2D part = solver.Run(prob, model);
	std::cout << "\nStudyCaseTrajectories runned successfully." << std::endl;
}

void StudyCaseTrajectoriesAdim(AbstractAdvDiffProblemAdim& prob, std::string model, int Nloc, double yStart, double zStart)
{
	std::cout << "\nRunning StudyCaseTrajectoriesAdim..." << std::endl;
	BISolver solver(Nloc, yStart, zStart);
	Particles2D part = solver.RunAdim(prob, model);
	std::cout << "\nStudyCaseTrajectoriesAdim runned successfully." << std::endl;
}

void StudyCaseConcentration(AbstractAdvDiffProblemAdim &prob, std::string model, std::string estimator,
							int Nloc, double yStart, double zStart, int dimy, int dimz)
{
	std::cout << "\nRunning StudyCaseConcentration..." << std::endl;
	std::cout << "1/3 : Generating trajectories..." << std::endl;
	BISolver solver(Nloc, yStart, zStart);
	Particles2D part = solver.RunAdim(prob);

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
	estim->EstimateAdim(part);
	std::cout << "3/3 : Writing in the files..." << std::endl;
	estim->Print(wd::root + "out/" + model + "/" + estimator + ".out");
	delete estim;
	std::cout << "\nStudyCaseConcentration runned successfully." << std::endl;
}

void StudyCaseTransitionProbabilities(AbstractAdvDiffProblemAdim& prob, std::string model, std::string estimator,
									  int dimy, int dimz, bool binary)
{
	std::cout << "\nRunning StudyCaseTransitionProbabilities..." << std::endl;
	std::cout << "1/4 : Generating the initial conditions..." << std::endl;
	int N = nybox*dimy*nzbox*dimz;
	double *yStart = new double [N];
	double *zStart = new double [N];
	double dy = 1./dimy;
	double dz = 1./dimz;
	double dybox = dy/nybox;
	double dzbox = dz/nzbox;

	for (int iy=0; iy<dimy; iy++)
	{
		for (int iz=0; iz<dimz; iz++)
		{
			for (int iybox=0; iybox<nybox; iybox++)
			{
				for (int izbox=0; izbox<nzbox; izbox++)
				{
					yStart[iy*dimz*Nbox+iz*Nbox+iybox*nzbox+izbox] = iy*dy + (iybox+.5)*dybox;
					zStart[iy*dimz*Nbox+iz*Nbox+iybox*nzbox+izbox] = iz*dz + (izbox+.5)*dzbox;
				}
			}
		}
	}

	std::cout << "2/4 : Generating trajectories..." << std::endl;
	BISolver solver(N, yStart, zStart);
	Particles2D part = solver.RunAdim(prob);

	std::cout << "3/4 : Computing the transition probability matrix..." << std::endl;
	GlobalEstimator *estim;
	transform(estimator.begin(), estimator.end(), estimator.begin(), ::tolower);
	if(estimator == "epanechnikov")
		estim = new GlobalKernelEstimator(dimy,dimz,Nbox,.1*pow(Nbox,-1./6.),"Epanechnikov");
	else if (estimator == "gaussian")
		estim = new GlobalKernelEstimator(dimy,dimz,Nbox,.1*pow(Nbox,-1./6.),"Gaussian");
	else if (estimator == "box")
		estim = new GlobalBoxEstimator(dimy,dimz,Nbox);
	else{
		std::cerr << "Unknown estimator type. Valid estimators are \"epanechnikov\", \"gaussian\" and \"box\"." << std::endl;
		abort();
	}
	estim->EstimateAdim(part);
	std::cout << "4/4 : Writing in the files..." << std::endl;
	
	if (binary)
		estim->Print(wd::root + "out/" + model + "/global_" + estimator + ".bin", binary);
	else
		estim->Print(wd::root + "out/" + model + "/global_" + estimator + ".out", binary);

	delete[] yStart;
	delete[] zStart;
	delete estim;
	std::cout << "\nStudyCaseTransitionProbabilities runned successfully." << std::endl;
}