/*
  studyCases.cpp
  "sde2D"

  Created by Renaud Dufays on 07/04/17
  Copyright © 2017. All rights reserved.
*/

#include "Compute.hpp"

void ComputeTrajectories(const AbstractAdvDiffProblem& prob, std::string outputdir, double dt, double T, Particles2D& part)
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
    fInfo << "Nloc = " << part.mN << "\n";
    fInfo.close();
    std::cout << "fInfo written succesfully !" << std::endl;

	std::cout << "Generating (and writing) trajectories..." << std::endl;
	BISolver solver(part,dt);
	part = solver.Run(prob, T, outputdir);
	std::cout << "\nComputeTrajectories runned successfully." << std::endl;
}

void ComputeConcentration(const AbstractAdvDiffProblem &prob, std::string outputdir, double dt, double Times[],
						int nTimes, Particles2D& part, int ncelly, int ncellz, bool binary)
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
    fInfo << "ncelly = " << ncelly << "\n";
    fInfo << "ncellz = " << ncellz << "\n";
    fInfo.close();

	// Estimator
	double H = prob.getH1()-prob.getH0();
	double L = prob.getL1()-prob.getL0();
	Estimator *estim = new BoxEstimator(ncelly,ncellz,prob.getH0(),H,prob.getL0(),L);
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
		estim->Print(wd::root + "out/" + outputdir + "/C" + std::to_string(Tyear) + ".out",binary);
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
									  int ncelly, int ncellz, int nyloc, int nzloc, double dt, 
									  double Times[], int nTimes, bool binary)
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
    fInfo << "ncelly = " << ncelly << "\n";
    fInfo << "ncellz = " << ncellz << "\n";
    fInfo.close();
	
    std::cout << "Generating the initial conditions..." << std::endl;
	int N = nyloc*ncelly*nzloc*ncellz;
	int Nloc = nyloc*nzloc;
	double H = prob.getH1()-prob.getH0();
	double L = prob.getL1()-prob.getL0();
	double *yStart = new double [N];
	double *zStart = new double [N];
	double dy = L/ncelly;
	double dz = H/ncellz;
	double dybox = dy/nyloc;
	double dzbox = dz/nzloc;

	for (int iy=0; iy<ncelly; iy++){
		for (int iz=0; iz<ncellz; iz++){
			for (int iybox=0; iybox<nyloc; iybox++){
				for (int izbox=0; izbox<nzloc; izbox++){
					yStart[iy*ncellz*Nloc+iz*Nloc+iybox*nzloc+izbox] = prob.getL0() + iy*dy + (iybox+.5)*dybox;
					zStart[iy*ncellz*Nloc+iz*Nloc+iybox*nzloc+izbox] = prob.getH0() + iz*dz + (izbox+.5)*dzbox;
				}
			}
		}
	}

	Particles2D part(N,yStart,zStart);
	delete[] yStart; delete[] zStart;
	// Estimator
    TPMatrixEstimator *estim = new TPMatrixBoxEstimator(ncelly,ncellz,prob.getH0(),H,prob.getL0(),L,Nloc);
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
									  int ncelly, int ncellz, int nyloc, int nzloc, double dt, 
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
    fInfo << "ncelly = " << ncelly << "\n";
    fInfo << "ncellz = " << ncellz << "\n";
    fInfo.close();
	
    std::cout << "Generating the initial conditions..." << std::endl;
    int ncelly_left = ncelly/2;
	int N = nyloc*ncelly_left*nzloc*ncellz; // we simulate only the trajectories of the particles starting in Omega- 
	int Nloc = nyloc*nzloc;
	double H = prob.getH1()-prob.getH0();
	double L = prob.getL1()-prob.getL0();
	double *yStart = new double [N];
	double *zStart = new double [N];
	double dy = L/ncelly;
	double dz = H/ncellz;
	double dybox = dy/nyloc;
	double dzbox = dz/nzloc;

	for (int iy=0; iy<ncelly_left; iy++){
		for (int iz=0; iz<ncellz; iz++){
			for (int iybox=0; iybox<nyloc; iybox++){
				for (int izbox=0; izbox<nzloc; izbox++){
					yStart[iy*ncellz*Nloc+iz*Nloc+iybox*nzloc+izbox] = prob.getL0() + iy*dy + (iybox+.5)*dybox;
					zStart[iy*ncellz*Nloc+iz*Nloc+iybox*nzloc+izbox] = prob.getH0() + iz*dz + (izbox+.5)*dzbox;
				}
			}
		}
	}

	Particles2D part(N,yStart,zStart);
	delete[] yStart; delete[] zStart;
	// Estimator
    TPMatrixBoxEstimatorP2B estim(ncelly,ncellz,prob.getH0(),H,prob.getL0(),L,Nloc);
	// Solver
	BISolver *solver;
	// timing
	clock_t tstart,t; // timing
	double tot_seconds, minutes, hours; // timing

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
		hours = floor(minutes/60);
		std::cout << "Finished in " << hours << " h " << minutes-60*hours << " min " << tot_seconds - minutes*60 << " seconds." << std::endl;

		std::cout << "2/3 : Computing estimator..." << std::endl;
		t = clock();
		estim.Estimate(part);
		tot_seconds = double(clock()-t)/(double)CLOCKS_PER_SEC;
		std::cout << "Finished in " << tot_seconds << " seconds." << std::endl;

		std::cout << "3/3 : Writing in the files..." << std::endl;
		t = clock();
		estim.Print(wd::root + "out/" + outputdir + "/M" + std::to_string(Tyear) +  ".bin", binary);
		tot_seconds = double(clock()-t)/(double)CLOCKS_PER_SEC;
		std::cout << "Finished in " << tot_seconds << " seconds." << std::endl;

		tot_seconds = double(clock()-tstart)/(double)CLOCKS_PER_SEC;
		minutes = floor(tot_seconds/60);
		hours = floor(minutes/60);
		std::cout << "Complete iteration took " << hours << " h " << minutes-60*hours << " min " << tot_seconds - minutes*60 << " seconds." << std::endl;
	}
	std::cout << "\nComputeP2BTransitionProbabilities runned successfully." << std::endl;
}