/*
  studyCases.cpp
  "overturner"

  Created by Renaud Dufays on 07/04/17
  Copyright © 2017. All rights reserved.
*/

#include "studyCases.hpp"

void StudyCaseTrajectories(AbstractAdvDiffProblem& prob, std::string model, int Nloc, double yStart, double zStart)
{
	std::cout << "\nRunning StudyCaseTrajectories..." << std::endl;
	
	std::cout << "Writing fInfo..." << std::endl;
	std::ofstream fInfo = openOutputFile(wd::root + "out/" + model + "/info.out");
	auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    fInfo << "File generated on " << std::put_time(&tm, "%d-%m-%Y at %Hh %Mm %Ss") << "\n";
    fInfo << "studyCaseTrajectories\n";
    fInfo << "Model loaded is " << wd::root << "in/" << model <<".in" << "\n";
    fInfo << "The values used are :\n";
    prob.printInfo(fInfo);
    fInfo << "Nloc = " << Nloc << "\n";
    fInfo << "yStart = " << yStart << "\n";
    fInfo << "zStart = " << zStart << "\n";
    fInfo.close();
	
	BISolver solver(Nloc, yStart, zStart);
	Particles2D part = solver.Run(prob, model);
	std::cout << "\nStudyCaseTrajectories runned successfully." << std::endl;
}

void StudyCaseTrajectoriesAdim(AbstractAdvDiffProblemAdim& prob, std::string model, int Nloc, double yStart, double zStart)
{
	std::cout << "\nRunning StudyCaseTrajectoriesAdim..." << std::endl;

	std::cout << "Writing fInfo..." << std::endl;
	std::ofstream fInfo = openOutputFile(wd::root + "out/" + model + "/infoAdim.out");
	auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    fInfo << "File generated on " << std::put_time(&tm, "%d-%m-%Y at %Hh %Mm %Ss") << "\n";
    fInfo << "StudyCaseTrajectoriesAdim\n";
    fInfo << "Model loaded is " << wd::root << "in/" << model <<".in" << "\n";
    fInfo << "The values used are :\n";
    prob.printInfo(fInfo);
    fInfo << "Nloc = " << Nloc << "\n";
    fInfo << "yStart = " << yStart << "\n";
    fInfo << "zStart = " << zStart << "\n";
    fInfo.close();

	BISolver solver(Nloc, yStart, zStart);
	Particles2D part = solver.RunAdim(prob, model);
	std::cout << "\nStudyCaseTrajectoriesAdim runned successfully." << std::endl;
}

void StudyCaseConcentration(AbstractAdvDiffProblemAdim &prob, std::string model, std::string estimator,
							int Nloc, double yStart, double zStart, int nboxy, int nboxz)
{
	std::cout << "\nRunning StudyCaseConcentration..." << std::endl;
	
	std::cout << "Writing fInfo..." << std::endl;
	std::ofstream fInfo = openOutputFile(wd::root + "out/" + model + "/info.out");
	auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    fInfo << "File generated on " << std::put_time(&tm, "%d-%m-%Y at %Hh %Mm %Ss") << "\n";
    fInfo << "studyCaseTrajectories\n";
    fInfo << "Model loaded is " << wd::root << "in/" << model <<".in" << "\n";
    fInfo << "The values used are :\n";
    prob.printInfo(fInfo);
    fInfo << "Nloc = " << Nloc << "\n";
    fInfo << "yStart = " << yStart << "\n";
    fInfo << "zStart = " << zStart << "\n";
    fInfo << "nboxy = " << nboxy << "\n";
    fInfo << "nboxz = " << nboxz << "\n";
    fInfo.close();

	std::cout << "1/3 : Generating trajectories..." << std::endl;
	BISolver solver(Nloc, yStart, zStart);
	Particles2D part = solver.RunAdim(prob);

	std::cout << "2/3 : Computing the concentrations..." << std::endl;
	Estimator *estim;
	transform(estimator.begin(), estimator.end(), estimator.begin(), ::tolower);
	if(estimator == "epanechnikov")
		estim = new KernelEstimator(nboxy,nboxz,1.,1.,.1*pow(part.mN,-1./6.),"Epanechnikov");
	else if (estimator == "gaussian")
		estim = new KernelEstimator(nboxy,nboxz,1.,1.,.1*pow(part.mN,-1./6.),"Gaussian");
	else if (estimator == "box")
		estim = new BoxEstimator(nboxy,nboxz,1.,1.);
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
									  int nboxy, int nboxz, int nyloc, int nzloc, bool binary)
{
	std::cout << "\nRunning StudyCaseTransitionProbabilities..." << std::endl;
	
	std::cout << "Writing fInfo..." << std::endl;
	std::ofstream fInfo = openOutputFile(wd::root + "out/" + model + "/info.out");
	auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    fInfo << "File generated on " << std::put_time(&tm, "%d-%m-%Y at %Hh %Mm %Ss") << "\n";
    fInfo << "studyCaseTrajectories\n";
    fInfo << "Model loaded is " << wd::root << "in/" << model <<".in" << "\n";
    fInfo << "The values used are :\n";
    prob.printInfo(fInfo);
    fInfo << "nyloc = " << nyloc << "\n";
    fInfo << "nzloc = " << nzloc << "\n";
    fInfo << "nboxy = " << nboxy << "\n";
    fInfo << "nboxz = " << nboxz << "\n";
    fInfo.close();
	
	std::cout << "1/4 : Generating the initial conditions..." << std::endl;
	int N = nyloc*nboxy*nzloc*nboxz;
	int Nbox = nyloc*nzloc;
	double *yStart = new double [N];
	double *zStart = new double [N];
	double dy = 1./nboxy;
	double dz = 1./nboxz;
	double dybox = dy/nyloc;
	double dzbox = dz/nzloc;

	for (int iy=0; iy<nboxy; iy++)
	{
		for (int iz=0; iz<nboxz; iz++)
		{
			for (int iybox=0; iybox<nyloc; iybox++)
			{
				for (int izbox=0; izbox<nzloc; izbox++)
				{
					yStart[iy*nboxz*Nbox+iz*Nbox+iybox*nzloc+izbox] = iy*dy + (iybox+.5)*dybox;
					zStart[iy*nboxz*Nbox+iz*Nbox+iybox*nzloc+izbox] = iz*dz + (izbox+.5)*dzbox;
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
		estim = new GlobalKernelEstimator(nboxy,nboxz,1.,1.,Nbox,.1*pow(Nbox,-1./6.),"Epanechnikov");
	else if (estimator == "gaussian")
		estim = new GlobalKernelEstimator(nboxy,nboxz,1.,1.,Nbox,.1*pow(Nbox,-1./6.),"Gaussian");
	else if (estimator == "box")
		estim = new GlobalBoxEstimator(nboxy,nboxz,1.,1.,Nbox);
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