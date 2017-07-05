/*
  studyCases.cpp
  "overturner"

  Created by Renaud Dufays on 07/04/17
  Copyright © 2017. All rights reserved.
*/

#include "Compute.hpp"

void ComputeTrajectories(const AbstractAdvDiffProblem& prob, std::string model, int Nloc, double yStart, double zStart)
{
	std::cout << "\nRunning ComputeTrajectories..." << std::endl;
	
	std::cout << "Writing fInfo..." << std::endl;
	std::ofstream fInfo = openOutputFile(wd::root + "out/" + model + "/info.out");
	auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    fInfo << "File generated on " << std::put_time(&tm, "%d-%m-%Y at %Hh %Mm %Ss") << "\n";
    fInfo << "ComputeTrajectories\n";
    fInfo << "Model loaded is " << wd::root << "in/" << model <<".in" << "\n";
    fInfo << "The values used are :\n";
    prob.printInfo(fInfo);
    fInfo << "Nloc = " << Nloc << "\n";
    fInfo << "yStart = " << yStart << "\n";
    fInfo << "zStart = " << zStart << "\n";
    fInfo.close();
    std::cout << "fInfo written succesfully !" << std::endl;

	std::cout << "Generating (and writing) trajectories..." << std::endl;
	BISolver solver(Nloc, yStart, zStart);
	Particles2D part = solver.Run(prob, model);
	std::cout << "\nComputeTrajectories runned successfully." << std::endl;
}

void ComputeTrajectories(const AbstractAdvDiffProblemAdim& prob, std::string model, int Nloc, double yStart, double zStart)
{
	std::cout << "\nRunning ComputeTrajectoriesAdim..." << std::endl;

	std::cout << "Writing fInfo..." << std::endl;
	std::ofstream fInfo = openOutputFile(wd::root + "out/" + model + "/infoAdim.out");
	auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    fInfo << "File generated on " << std::put_time(&tm, "%d-%m-%Y at %Hh %Mm %Ss") << "\n";
    fInfo << "ComputeTrajectoriesAdim\n";
    fInfo << "Model loaded is " << wd::root << "in/" << model <<".in" << "\n";
    fInfo << "The values used are :\n";
    prob.printInfo(fInfo);
    fInfo << "Nloc = " << Nloc << "\n";
    fInfo << "yStart = " << yStart << "\n";
    fInfo << "zStart = " << zStart << "\n";
    fInfo.close();

	BISolver solver(Nloc, yStart, zStart);
	Particles2D part = solver.RunAdim(prob, model);
	std::cout << "\nComputeTrajectoriesAdim runned successfully." << std::endl;
}

void ComputeConcentration(const AbstractAdvDiffProblem &prob, std::string model, std::string estimator,
							int Nloc, double yStart, double zStart, int nboxy, int nboxz)
{
	std::cout << "\nRunning ComputeConcentration..." << std::endl;
	
	std::cout << "Writing fInfo..." << std::endl;
	std::ofstream fInfo = openOutputFile(wd::root + "out/" + model + "/info.out");
	auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    fInfo << "File generated on " << std::put_time(&tm, "%d-%m-%Y at %Hh %Mm %Ss") << "\n";
    fInfo << "ComputeTrajectories\n";
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
	Particles2D part = solver.Run(prob);

	std::cout << "2/3 : Computing the concentrations..." << std::endl;
	Estimator *estim;
	double H = prob.getH1()-prob.getH0();
	double L = prob.getL1()-prob.getL0();
	transform(estimator.begin(), estimator.end(), estimator.begin(), ::tolower);
	if(estimator == "epanechnikov")
		estim = new KernelEstimator(nboxy,nboxz,H,L,.1*pow(part.mN,-1./6.),"Epanechnikov");
	else if (estimator == "gaussian")
		estim = new KernelEstimator(nboxy,nboxz,H,L,.1*pow(part.mN,-1./6.),"Gaussian");
	else if (estimator == "box")
		estim = new BoxEstimator(nboxy,nboxz,H,L);
	else{
		std::cerr << "Unknown estimator type. Valid estimators are \"epanechnikov\", \"gaussian\" and \"box\"." << std::endl;
		abort();
	}
	estim->Estimate(part);
	std::cout << "3/3 : Writing in the files..." << std::endl;
	estim->Print(wd::root + "out/" + model + "/" + estimator + ".out");
	delete estim;
	std::cout << "\nComputeConcentration runned successfully." << std::endl;
}


void ComputeConcentration(const AbstractAdvDiffProblemAdim &prob, std::string model, std::string estimator,
							int Nloc, double yStart, double zStart, int nboxy, int nboxz)
{
	std::cout << "\nRunning ComputeConcentrationAdim..." << std::endl;
	
	std::cout << "Writing fInfo..." << std::endl;
	std::ofstream fInfo = openOutputFile(wd::root + "out/" + model + "/info.out");
	auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    fInfo << "File generated on " << std::put_time(&tm, "%d-%m-%Y at %Hh %Mm %Ss") << "\n";
    fInfo << "ComputeTrajectories\n";
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
	std::cout << "\nComputeConcentrationAdim runned successfully." << std::endl;
}

void ComputeTransitionProbabilities(const AbstractAdvDiffProblemAdim& prob, std::string model, std::string estimator,
									  int nboxy, int nboxz, int nyloc, int nzloc, bool binary)
{
	std::cout << "\nRunning ComputeTransitionProbabilities..." << std::endl;
	
	std::cout << "Writing fInfo..." << std::endl;
	std::ofstream fInfo = openOutputFile(wd::root + "out/" + model + "/info.out");
	auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    fInfo << "File generated on " << std::put_time(&tm, "%d-%m-%Y at %Hh %Mm %Ss") << "\n";
    fInfo << "ComputeTrajectories\n";
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
	int Nloc = nyloc*nzloc;
	double *yStart = new double [N];
	double *zStart = new double [N];
	double dy = 1./nboxy;
	double dz = 1./nboxz;
	double dybox = dy/nyloc;
	double dzbox = dz/nzloc;

	for (int iy=0; iy<nboxy; iy++){
		for (int iz=0; iz<nboxz; iz++){
			for (int iybox=0; iybox<nyloc; iybox++){
				for (int izbox=0; izbox<nzloc; izbox++){
					yStart[iy*nboxz*Nloc+iz*Nloc+iybox*nzloc+izbox] = iy*dy + (iybox+.5)*dybox;
					zStart[iy*nboxz*Nloc+iz*Nloc+iybox*nzloc+izbox] = iz*dz + (izbox+.5)*dzbox;
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
		estim = new GlobalKernelEstimator(nboxy,nboxz,1.,1.,Nloc,.1*pow(Nloc,-1./6.),"Epanechnikov");
	else if (estimator == "gaussian")
		estim = new GlobalKernelEstimator(nboxy,nboxz,1.,1.,Nloc,.1*pow(Nloc,-1./6.),"Gaussian");
	else if (estimator == "box")
		estim = new GlobalBoxEstimator(nboxy,nboxz,0.,1.,0.,1.,Nloc);
	else{
		std::cerr << "Unknown estimator type. Valid estimators are \"epanechnikov\", \"gaussian\" and \"box\"." << std::endl;
		abort();
	}
	// estim->EstimateAdim(part); // equivalent
	estim->Estimate(part);
	std::cout << "4/4 : Writing in the files..." << std::endl;
	
	if (binary)
		estim->Print(wd::root + "out/" + model + "/global_" + estimator + ".bin", binary);
	else
		estim->Print(wd::root + "out/" + model + "/global_" + estimator + ".out", binary);

	delete[] yStart;
	delete[] zStart;
	delete estim;
	std::cout << "\nComputeTransitionProbabilities runned successfully." << std::endl;
}

void ComputeTransitionProbabilities(const AbstractAdvDiffProblem& prob, std::string model, std::string estimator,
									  int nboxy, int nboxz, int nyloc, int nzloc, bool binary)
{
	std::cout << "\nRunning ComputeTransitionProbabilities..." << std::endl;
	
	std::cout << "Writing fInfo..." << std::endl;
	std::ofstream fInfo = openOutputFile(wd::root + "out/" + model + "/info.out");
	auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    fInfo << "File generated on " << std::put_time(&tm, "%d-%m-%Y at %Hh %Mm %Ss") << "\n";
    fInfo << "ComputeTrajectories\n";
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
	int Nloc = nyloc*nzloc;
	double H = prob.getH1()-prob.getH0();
	double L = prob.getL1()-prob.getL0();
	double *yStart = new double [N];
	double *zStart = new double [N];
	double dy = L/nboxy;
	double dz = H/nboxz;
	double dybox = dy/nyloc;
	double dzbox = dz/nzloc;

	for (int iy=0; iy<nboxy; iy++){
		for (int iz=0; iz<nboxz; iz++){
			for (int iybox=0; iybox<nyloc; iybox++){
				for (int izbox=0; izbox<nzloc; izbox++){
					yStart[iy*nboxz*Nloc+iz*Nloc+iybox*nzloc+izbox] = prob.getL0() + iy*dy + (iybox+.5)*dybox;
					zStart[iy*nboxz*Nloc+iz*Nloc+iybox*nzloc+izbox] = prob.getH0() + iz*dz + (izbox+.5)*dzbox;
				}
			}
		}
	}

	std::cout << "2/4 : Generating trajectories..." << std::endl;
	BISolver solver(N, yStart, zStart);
	Particles2D part = solver.Run(prob);

	std::cout << "3/4 : Computing the transition probability matrix..." << std::endl;
	GlobalEstimator *estim;
	transform(estimator.begin(), estimator.end(), estimator.begin(), ::tolower);
	if(estimator == "epanechnikov")
		estim = new GlobalKernelEstimator(nboxy,nboxz,H,L,Nloc,.1*pow(Nloc,-1./6.),"Epanechnikov");
	else if (estimator == "gaussian")
		estim = new GlobalKernelEstimator(nboxy,nboxz,H,L,Nloc,.1*pow(Nloc,-1./6.),"Gaussian");
	else if (estimator == "box")
		estim = new GlobalBoxEstimator(nboxy,nboxz,prob.getH0(),H,prob.getL0(),L,Nloc);
	else{
		std::cerr << "Unknown estimator type. Valid estimators are \"epanechnikov\", \"gaussian\" and \"box\"." << std::endl;
		abort();
	}
	// estim->EstimateAdim(part); // equivalent
	estim->Estimate(part);
	std::cout << "4/4 : Writing in the files..." << std::endl;
	
	if (binary)
		estim->Print(wd::root + "out/" + model + "/global_" + estimator + ".bin", binary);
	else
		estim->Print(wd::root + "out/" + model + "/global_" + estimator + ".out", binary);

	delete[] yStart;
	delete[] zStart;
	delete estim;
	std::cout << "\nComputeTransitionProbabilities runned successfully." << std::endl;
}

void ComputeTransitionProbabilities(const AbstractAdvDiffProblem& prob, std::string model,
									  int nboxy, int nboxz, int nyloc, int nzloc, double Times[], int nTimes, bool binary)
{
	std::cout << "\nRunning ComputeTransitionProbabilities..." << std::endl;
	
	std::cout << "Writing fInfo..." << std::endl;
	std::ofstream fInfo = openOutputFile(wd::root + "out/" + model + "/info.out");
	auto tt = std::time(nullptr);
    auto tm = *std::localtime(&tt);
    fInfo << "File generated on " << std::put_time(&tm, "%d-%m-%Y at %Hh %Mm %Ss") << "\n";
    fInfo << "ComputeTrajectories\n";
    fInfo << "Model loaded is " << wd::root << "in/" << model <<".in" << "\n";
    fInfo << "The values used are :\n";
    prob.printInfo(fInfo);
    fInfo << "nyloc = " << nyloc << "\n";
    fInfo << "nzloc = " << nzloc << "\n";
    fInfo << "nboxy = " << nboxy << "\n";
    fInfo << "nboxz = " << nboxz << "\n";
    fInfo.close();
	
	std::cout << "Generating the initial conditions..." << std::endl;
	int N = nyloc*nboxy*nzloc*nboxz;
	int Nloc = nyloc*nzloc;
	double H = prob.getH1()-prob.getH0();
	double L = prob.getL1()-prob.getL0();
	double *yStart = new double [N];
	double *zStart = new double [N];
	double dy = L/nboxy;
	double dz = H/nboxz;
	double dybox = dy/nyloc;
	double dzbox = dz/nzloc;

	for (int iy=0; iy<nboxy; iy++){
		for (int iz=0; iz<nboxz; iz++){
			for (int iybox=0; iybox<nyloc; iybox++){
				for (int izbox=0; izbox<nzloc; izbox++){
					yStart[iy*nboxz*Nloc+iz*Nloc+iybox*nzloc+izbox] = prob.getL0() + iy*dy + (iybox+.5)*dybox;
					zStart[iy*nboxz*Nloc+iz*Nloc+iybox*nzloc+izbox] = prob.getH0() + iz*dz + (izbox+.5)*dzbox;
				}
			}
		}
	}

	Particles2D part(N,yStart,zStart);
	delete[] yStart; delete[] zStart;
	BISolver *solver;
	GlobalBoxEstimator estim(nboxy,nboxz,prob.getH0(),H,prob.getL0(),L,Nloc); 
	clock_t tstart,t; // timing
	double tot_seconds, minutes; // timing

	for(int i=0; i<nTimes; i++)
	{
		std::cout << "--------------------ITERATION " << i+1 << "/" << nTimes <<"--------------------" << std::endl;
		tstart = clock();

		std::cout << "1/3 : Generating trajectories..." << std::endl;
		t = clock();
		solver = new BISolver(part);
		part = solver->Run(prob,Times[i]);
		delete solver;
		tot_seconds = double(clock()-t)/(double)CLOCKS_PER_SEC;
		minutes = floor(tot_seconds/60);
		std::cout << "Finished in " << minutes << " min " << tot_seconds - minutes*60 << " seconds." << std::endl;

		std::cout << "2/3 : Computing estimator..." << std::endl;
		t = clock();
		estim.Estimate(part);
		tot_seconds = double(clock()-t)/(double)CLOCKS_PER_SEC;
		minutes = floor(tot_seconds/60);
		std::cout << "Finished in " << minutes << " min " << tot_seconds - minutes*60 << " seconds." << std::endl;

		std::cout << "3/3 : Writing in the files..." << std::endl;
		t = clock();
		estim.Print(wd::root + "out/" + model + "/M" + std::to_string(i) +  ".bin", binary);
		tot_seconds = double(clock()-t)/(double)CLOCKS_PER_SEC;
		minutes = floor(tot_seconds/60);
		std::cout << "Finished in " << minutes << " min " << tot_seconds - minutes*60 << " seconds." << std::endl;

		tot_seconds = double(clock()-tstart)/(double)CLOCKS_PER_SEC;
		minutes = floor(tot_seconds/60);
		std::cout << "Complete iteration took " << minutes << " min " << tot_seconds - minutes*60 << " seconds." << std::endl;
	}
	
	std::cout << "\nComputeTransitionProbabilities runned successfully." << std::endl;
}
