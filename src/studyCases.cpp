/*
  studyCases.cpp
  "overturner"

  Created by Renaud Dufays on 07/04/17
  Copyright © 2017. All rights reserved.
*/

#include "studyCases.hpp"

void StudyCaseTrajectories(const AbstractAdvDiffProblem& prob, std::string model, int Nloc, double yStart, double zStart)
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
    std::cout << "fInfo written succesfully !" << std::endl;

	std::cout << "Generating (and writing) trajectories..." << std::endl;
	BISolver solver(Nloc, yStart, zStart);
	Particles2D part = solver.Run(prob, model);
	std::cout << "\nStudyCaseTrajectories runned successfully." << std::endl;
}

void StudyCaseTrajectoriesAdim(const AbstractAdvDiffProblemAdim& prob, std::string model, int Nloc, double yStart, double zStart)
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

void StudyCaseConcentration(const AbstractAdvDiffProblem &prob, std::string model, std::string estimator,
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
	std::cout << "\nStudyCaseConcentration runned successfully." << std::endl;
}


void StudyCaseConcentrationAdim(const AbstractAdvDiffProblemAdim &prob, std::string model, std::string estimator,
							int Nloc, double yStart, double zStart, int nboxy, int nboxz)
{
	std::cout << "\nRunning StudyCaseConcentrationAdim..." << std::endl;
	
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
	std::cout << "\nStudyCaseConcentrationAdim runned successfully." << std::endl;
}

void StudyCaseTransitionProbabilities(const AbstractAdvDiffProblemAdim& prob, std::string model, std::string estimator,
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
		estim = new GlobalBoxEstimator(nboxy,nboxz,1.,1.,Nloc);
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
	std::cout << "\nStudyCaseTransitionProbabilities runned successfully." << std::endl;
}

void StudyCaseTestProblem()
{
	std::cout << "\nRunning StudyCaseTestProblem..." << std::endl;
		// parameters values from C. Timmermans for overturner model
	double Psi = 2.;
	double L = 15e6;
	double H = 5e3;
	double Kh = 1e3;
	double Kv2 = 1e-4;
	// Values for test problem
	double T = 365*24*3600;
	double dt = 3600;
	double V = Psi/H;
	double W = Psi/L;
	double Kyy = Kh;
	double Kzz = Kv2;
	double Ly = 10*std::max(V*T, sqrt(Kyy*T));
	double Lz = 10*std::max(W*T, sqrt(Kzz*T));
	double y1 = 0., z1 = 0.;
	int J = 50000;
	std::string model = "testcase";
	TestProblem testprob(T,dt,Ly,Lz,Kyy,Kzz,V,W,J);
	
	std::cout << "Writing fInfo..." << std::endl;
	std::ofstream fInfo = openOutputFile(wd::root + "out/" + model + "/info.out");
	auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    fInfo << "File generated on " << std::put_time(&tm, "%d-%m-%Y at %Hh %Mm %Ss") << "\n";
    fInfo << "StudyCaseTestProblem\n";
    fInfo << "The values used are :\n";
    testprob.printInfo(fInfo);
    fInfo << "y1 = " << y1 << "\n";
    fInfo << "z1 = " << z1 << "\n";
    fInfo.close();
    std::cout << "fInfo written succesfully !" << std::endl;

	std::cout << "Generating trajectories..." << std::endl;
	BISolver solver(J, y1, z1);
	Particles2D part = solver.Run(testprob, model,testprob.getT()/testprob.getdt(),false,false);
	std::cout << "\nStudyCaseTestProblem runned successfully." << std::endl;
}

void StudyCaseTestProblemSemiInf()
{
	std::cout << "\nRunning StudyCaseTestProblemSemiInf..." << std::endl;
		// parameters values from C. Timmermans for overturner model
	double Psi = 2.;
	double H = 5e3;
	double Kh = 1e3;
	double Kv1 = 1e-1;
	// Values for test problem
	double T = 365*24*3600;
	double dt = 3600;
	double V = Psi/H;
	double W = 0.;
	double Kyy = Kh;
	double Kzz = Kv1;
	double Ly = 10*std::max(V*T, sqrt(Kyy*T));
	double Lz = 10*std::max(W*T, sqrt(Kzz*T));
	double y1 = 0.;
	double z1 = H;
	int J = 20000;
	std::string model = "testcaseSI";
	TestProblem testprob(T,dt,Ly,Lz,Kyy,Kzz,V,W,J,"semi-infinite");
	
	std::cout << "Writing fInfo..." << std::endl;
	std::ofstream fInfo = openOutputFile(wd::root + "out/" + model + "/info.out");
	auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    fInfo << "File generated on " << std::put_time(&tm, "%d-%m-%Y at %Hh %Mm %Ss") << "\n";
    fInfo << "StudyCaseTestProblem\n";
    fInfo << "The values used are :\n";
    testprob.printInfo(fInfo);
    fInfo << "y1 = " << y1 << "\n";
    fInfo << "z1 = " << z1 << "\n";
    fInfo.close();
    std::cout << "fInfo written succesfully !" << std::endl;

	std::cout << "Generating trajectories..." << std::endl;
	BISolver solver(J, y1, z1);
	Particles2D part = solver.Run(testprob, model,testprob.getT()/testprob.getdt(),false,false);
	std::cout << "\nStudyCaseTestProblemSemiInf runned successfully." << std::endl;
}

int StudyCaseComputeNloc(const AbstractAdvDiffProblemAdim& prob, const double epsilon, int nboxy, int nboxz)
{
	int nyloc = 32, nzloc = 32; // hence Nloc = 1024 
	// Store initial positions of the particles in yStart and zStart
	int Nloc = nyloc*nzloc;
	int Nbox = nboxy*nboxz;
	int N = Nloc*Nbox;
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
	// Compute 2 sets of trajectories (the random generator is part of the solver object : we need two solvers in order to generate
	// two different sets of trajectories)
	int iter = 0;
	double F;
	BISolver *solver1, *solver2;
	Particles2D part1(N), part2(N);
	GlobalBoxEstimator *estim1, *estim2;
	Matrix A1(Nbox,Nbox), A2(Nbox,Nbox);
	do{
		iter++;

		solver1 = new BISolver(N, yStart, zStart);
		solver2 = new BISolver(N, yStart, zStart);

		part1 = solver1->RunAdim(prob);
		part2 = solver2->RunAdim(prob);
		delete solver1;
		delete solver2;
		// Estimators
		estim1 = new GlobalBoxEstimator(nboxy,nboxz,1.,1.,Nloc);
		estim2 = new GlobalBoxEstimator(nboxy,nboxz,1.,1.,Nloc);
		A1 += estim1->Count(part1);
		A2 += estim2->Count(part2);
		delete estim1;
		delete estim2;

		F = Frobenius((A1-A2)/Nloc);
		std::cout << "Iteration " << iter << " : Nloc = " << Nloc << ", F = " << F << std::endl;
		Nloc += Nloc;
	} while ((F > epsilon) && (Nloc < 5e4));

	std::cout << "StudyCaseComputeNloc terminated after " << iter << " iterations." << std::endl;
	std::cout << "Nloc = " << Nloc << ", F = " << F << std::endl;
	if (Nloc >= 5e4){
		std::cerr << "WARNING : NO CONVERGENCE REACHED. Nloc > NlocMax" << std::endl;
		return -1;
	}
	return Nloc;
}
