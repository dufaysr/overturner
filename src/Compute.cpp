/*
  studyCases.cpp
  "overturner"

  Created by Renaud Dufays on 07/04/17
  Copyright © 2017. All rights reserved.
*/

#include "Compute.hpp"

void ComputeTrajectories(const AbstractAdvDiffProblem& prob, std::string outputdir, double dt, double T, int Nloc, double yStart, double zStart)
{
	std::cout << "\nRunning ComputeTrajectories..." << std::endl;
	
	std::cout << "Writing fInfo..." << std::endl;
	std::ofstream fInfo = openOutputFile(wd::root + "out/" + outputdir + "/info.out");
	auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    fInfo << "File generated on " << std::put_time(&tm, "%d-%m-%Y at %Hh %Mm %Ss") << "\n";
    fInfo << "ComputeTrajectories\n";
    fInfo << "The values used are :\n";
    prob.printInfo(fInfo);
    fInfo << "Nloc = " << Nloc << "\n";
    fInfo << "yStart = " << yStart << "\n";
    fInfo << "zStart = " << zStart << "\n";
    fInfo.close();
    std::cout << "fInfo written succesfully !" << std::endl;

	std::cout << "Generating (and writing) trajectories..." << std::endl;
	Particles2D part(Nloc, yStart, zStart);
	BISolver solver(part,dt);
	part = solver.Run(prob, T, outputdir);
	std::cout << "\nComputeTrajectories runned successfully." << std::endl;
}

void ComputeConcentration(const AbstractAdvDiffProblem &prob, std::string outputdir, double dt, double Times[],
						int nTimes, Particles2D& part, int nboxy, int nboxz, bool binary, std::string estimator)
{
	std::cout << "\nRunning ComputeConcentration..." << std::endl;
	
	std::cout << "Writing fInfo..." << std::endl;
	std::ofstream fInfo = openOutputFile(wd::root + "out/" + outputdir + "/info.out");
	auto tt = std::time(nullptr);
    auto tm = *std::localtime(&tt);
    fInfo << "File generated on " << std::put_time(&tm, "%d-%m-%Y at %Hh %Mm %Ss") << "\n";
    fInfo << "ComputeTrajectories\n";
    fInfo << "The values used are :\n";
    prob.printInfo(fInfo);
    fInfo << "nboxy = " << nboxy << "\n";
    fInfo << "nboxz = " << nboxz << "\n";
    fInfo.close();

	// Estimator
	Estimator *estim;
	double H = prob.getH1()-prob.getH0();
	double L = prob.getL1()-prob.getL0();
	transform(estimator.begin(), estimator.end(), estimator.begin(), ::tolower);
	if (estimator == "box")
		estim = new BoxEstimator(nboxy,nboxz,prob.getH0(),H,prob.getL0(),L);
	else if(estimator == "epanechnikov")
		estim = new KernelEstimator(nboxy,nboxz,H,L,.1*pow(part.mN,-1./6.),"Epanechnikov");
	else if (estimator == "gaussian")
		estim = new KernelEstimator(nboxy,nboxz,H,L,.1*pow(part.mN,-1./6.),"Gaussian");
	else{
		std::cerr << "Unknown estimator type. Valid estimators are \"epanechnikov\", \"gaussian\" and \"box\"." << std::endl;
		abort();
	}
	// Solver
	BISolver *solver;
	// timing
	clock_t tstart,t; // timing
	double tot_seconds, minutes; // timing

	const double year = 3600*24*365;
	int Tyear;

	for (int i=0; i<nTimes; i++)
	{
		Tyear = (int) (Times[i]/year);
		std::cout << "--------------------ITERATION " << i+1 << "/" << nTimes << ": T = " << Tyear << " year(s)--------------------" << std::endl;
		tstart = clock();

		std::cout << "1/3 : Generating trajectories..." << std::endl;
		t = clock();
		solver = new BISolver(part,dt);
		part = solver->Run(prob,Times[i]);
		delete solver;
		tot_seconds = double(clock()-t)/(double)CLOCKS_PER_SEC;
		minutes = floor(tot_seconds/60);
		std::cout << "Finished in " << minutes << " min " << tot_seconds - minutes*60 << " seconds." << std::endl;

		std::cout << "2/3 : Computing estimator..." << std::endl;
		t = clock();
		estim->Estimate(part);
		tot_seconds = double(clock()-t)/(double)CLOCKS_PER_SEC;
		minutes = floor(tot_seconds/60);
		std::cout << "Finished in " << minutes << " min " << tot_seconds - minutes*60 << " seconds." << std::endl;

		std::cout << "3/3 : Writing in the files..." << std::endl;
		t = clock();
		estim->Print(wd::root + "out/" + outputdir + "/C" + estimator + std::to_string(Tyear) + ".out",binary);
		tot_seconds = double(clock()-t)/(double)CLOCKS_PER_SEC;
		minutes = floor(tot_seconds/60);
		std::cout << "Finished in " << minutes << " min " << tot_seconds - minutes*60 << " seconds." << std::endl;

		tot_seconds = double(clock()-tstart)/(double)CLOCKS_PER_SEC;
		minutes = floor(tot_seconds/60);
		std::cout << "Complete iteration took " << minutes << " min " << tot_seconds - minutes*60 << " seconds." << std::endl;
	}
	delete estim;
	std::cout << "\nComputeConcentration runned successfully." << std::endl;
}

void ComputeTransitionProbabilities(const AbstractAdvDiffProblem& prob, std::string outputdir,
									  int nboxy, int nboxz, int nyloc, int nzloc, double dt, 
									  double Times[], int nTimes, bool binary, std::string estimator)
{
	std::cout << "\nRunning ComputeTransitionProbabilities..." << std::endl;
	
	std::cout << "Writing fInfo..." << std::endl;
	std::ofstream fInfo = openOutputFile(wd::root + "out/" + outputdir + "/info.out");
	auto tt = std::time(nullptr);
    auto tm = *std::localtime(&tt);
    fInfo << "File generated on " << std::put_time(&tm, "%d-%m-%Y at %Hh %Mm %Ss") << "\n";
    fInfo << "ComputeTransitionProbabilities\n";
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
	// Estimator
    TPMatrixEstimator *estim;
	transform(estimator.begin(), estimator.end(), estimator.begin(), ::tolower);
	if (estimator == "box")
		estim = new TPMatrixBoxEstimator(nboxy,nboxz,prob.getH0(),H,prob.getL0(),L,Nloc);
	else if(estimator == "epanechnikov")
		estim = new TPMatrixKernelEstimator(nboxy,nboxz,H,L,Nloc,.1*pow(Nloc,-1./6.),"Epanechnikov");
	else if (estimator == "gaussian")
		estim = new TPMatrixKernelEstimator(nboxy,nboxz,H,L,Nloc,.1*pow(Nloc,-1./6.),"Gaussian");
	else{
		std::cerr << "Unknown estimator type. Valid estimators are \"epanechnikov\", \"gaussian\" and \"box\"." << std::endl;
		abort();
	}
	// Solver
	BISolver *solver;
	// timing
	clock_t tstart,t; // timing
	double tot_seconds, minutes; // timing

	const double year = 3600*24*365;
	int Tyear;

	for(int i=0; i<nTimes; i++)
	{
		Tyear = (int) (Times[i]/year);
		std::cout << "--------------------ITERATION " << i+1 << "/" << nTimes << ": T = " << Tyear <<"--------------------" << std::endl;
		tstart = clock();

		std::cout << "1/3 : Generating trajectories..." << std::endl;
		t = clock();
		solver = new BISolver(part,dt);
		part = solver->Run(prob,Times[i]);
		delete solver;
		tot_seconds = double(clock()-t)/(double)CLOCKS_PER_SEC;
		minutes = floor(tot_seconds/60);
		std::cout << "Finished in " << minutes << " min " << tot_seconds - minutes*60 << " seconds." << std::endl;

		std::cout << "2/3 : Computing estimator..." << std::endl;
		t = clock();
		estim->Estimate(part);
		tot_seconds = double(clock()-t)/(double)CLOCKS_PER_SEC;
		minutes = floor(tot_seconds/60);
		std::cout << "Finished in " << minutes << " min " << tot_seconds - minutes*60 << " seconds." << std::endl;

		std::cout << "3/3 : Writing in the files..." << std::endl;
		t = clock();
		estim->Print(wd::root + "out/" + outputdir + "/M" + std::to_string(Tyear) +  ".bin", binary);
		tot_seconds = double(clock()-t)/(double)CLOCKS_PER_SEC;
		minutes = floor(tot_seconds/60);
		std::cout << "Finished in " << minutes << " min " << tot_seconds - minutes*60 << " seconds." << std::endl;

		tot_seconds = double(clock()-tstart)/(double)CLOCKS_PER_SEC;
		minutes = floor(tot_seconds/60);
		std::cout << "Complete iteration took " << minutes << " min " << tot_seconds - minutes*60 << " seconds." << std::endl;
	}
	delete estim;
	std::cout << "\nComputeTransitionProbabilities runned successfully." << std::endl;
}

void ComputeP2BTransitionProbabilities(const Problem2Box& prob, std::string outputdir,
									  int nboxy, int nboxz, int nyloc, int nzloc, double dt, 
									  double Times[], int nTimes, bool binary)
{
	/* 
		Here we exploit the symmetry of the bi-overturner problems: it is possible to compute the
		transition probability matrix by simulating only the particles trajectories starting in Omega-,
		hence saving half of the computational time.
	*/
	std::cout << "\nRunning ComputeP2BTransitionProbabilities..." << std::endl;
	
	std::cout << "Writing fInfo..." << std::endl;
	std::ofstream fInfo = openOutputFile(wd::root + "out/" + outputdir + "/info.out");
	auto tt = std::time(nullptr);
    auto tm = *std::localtime(&tt);
    fInfo << "File generated on " << std::put_time(&tm, "%d-%m-%Y at %Hh %Mm %Ss") << "\n";
    fInfo << "ComputeTransitionProbabilities\n";
    fInfo << "The values used are :\n";
    prob.printInfo(fInfo);
    fInfo << "nyloc = " << nyloc << "\n";
    fInfo << "nzloc = " << nzloc << "\n";
    fInfo << "nboxy = " << nboxy << "\n";
    fInfo << "nboxz = " << nboxz << "\n";
    fInfo.close();
	
    std::cout << "Generating the initial conditions..." << std::endl;
    int nboxy_left = nboxy/2;
	int N = nyloc*nboxy_left*nzloc*nboxz; // we simulate only the trajectories of the particles starting in Omega- 
	int Nloc = nyloc*nzloc;
	double H = prob.getH1()-prob.getH0();
	double L = prob.getL1()-prob.getL0();
	double *yStart = new double [N];
	double *zStart = new double [N];
	double dy = L/nboxy;
	double dz = H/nboxz;
	double dybox = dy/nyloc;
	double dzbox = dz/nzloc;

	for (int iy=0; iy<nboxy_left; iy++){
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
	// Estimator
    TPMatrixBoxEstimatorP2B estim(nboxy,nboxz,prob.getH0(),H,prob.getL0(),L,Nloc);
	// Solver
	BISolver *solver;
	// timing
	clock_t tstart,t; // timing
	double tot_seconds, minutes; // timing

	const double year = 3600*24*365;
	int Tyear;

	for(int i=0; i<nTimes; i++)
	{
		Tyear = (int) (Times[i]/year);
		std::cout << "--------------------ITERATION " << i+1 << "/" << nTimes << ": T = " << Tyear <<"--------------------" << std::endl;
		tstart = clock();

		std::cout << "1/3 : Generating trajectories..." << std::endl;
		t = clock();
		solver = new BISolver(part,dt);
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
		estim.Print(wd::root + "out/" + outputdir + "/M" + std::to_string(Tyear) +  ".bin", binary);
		tot_seconds = double(clock()-t)/(double)CLOCKS_PER_SEC;
		minutes = floor(tot_seconds/60);
		std::cout << "Finished in " << minutes << " min " << tot_seconds - minutes*60 << " seconds." << std::endl;

		tot_seconds = double(clock()-tstart)/(double)CLOCKS_PER_SEC;
		minutes = floor(tot_seconds/60);
		std::cout << "Complete iteration took " << minutes << " min " << tot_seconds - minutes*60 << " seconds." << std::endl;
	}
	std::cout << "\nComputeP2BTransitionProbabilities runned successfully." << std::endl;
}