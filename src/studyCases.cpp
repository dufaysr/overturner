/*
  studyCases.cpp
  "overturner"

  Created by Renaud Dufays on 07/04/17
  Copyright © 2017. All rights reserved.
*/

#include "studyCases.hpp"

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
	std::string outputdir = "testcase";
	TestProblem testprob(Ly,Lz,Kyy,Kzz,V,W,J);
	
	std::cout << "Writing fInfo..." << std::endl;
	std::ofstream fInfo = openOutputFile(wd::root + "out/" + outputdir + "/info.out");
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
	Particles2D part(J,y1,z1);
	BISolver solver(part,dt);
	part = solver.Run(testprob,T,outputdir,T/dt,false,false);
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
	std::string outputdir = "testcaseSI";
	TestProblem testprob(Ly,Lz,Kyy,Kzz,V,W,J,"semi-infinite");
	
	std::cout << "Writing fInfo..." << std::endl;
	std::ofstream fInfo = openOutputFile(wd::root + "out/" + outputdir + "/info.out");
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
	Particles2D part(J,y1,z1);
	BISolver solver(part,dt);
	part = solver.Run(testprob,T,outputdir,T/dt,false,false);
	std::cout << "\nStudyCaseTestProblemSemiInf runned successfully." << std::endl;
}

void StudyCaseProblem2BoxTraj()
{
	/* trajectories */
	double dt = 3600;
	double year = 365*24*3600;
	double T = 1000*year;
	double alpha = .75;
	Problem2Box prob(alpha);
	ComputeTrajectories(prob,"data2box",dt,T,10,-13e6,4.5e3);
}

void StudyCaseProblem2BoxTP()
{
	/* Transition probability matrices */
	double dt = 3600;
	double year = 365*24*3600;
	const int nalpha = 1;
	double alpha[nalpha] = {.75};
	int nameindex[nalpha] = {75};

	int nboxy = 2;
	int nboxz = 1;
	int nlocy = 100, nlocz = 100;

	std::string outputdir;
	const int nTimes = 10;
	double Times[nTimes] = {10.*year,20*year,30*year,40*year,50*year,60*year,70*year,80*year,90*year,100*year};

	for (int i=0; i<nalpha; i++){
		Problem2Box prob(alpha[i]);
		outputdir = "problem2box_a" + std::to_string(nameindex[i]);
		ComputeTransitionProbabilities(prob, outputdir, nboxy, nboxz, nlocy, nlocz, dt, Times, nTimes, true);
	}	
}

void StudyCaseOverturnerTPnTimes()
{
	std::string model = "timmermans";
	OverturnerProblem prob(model);

	double dt = 3600;
	int nboxy = 15;
	int nboxz = 10;
	int nlocy = 100, nlocz = 100;
	std::string outputdir = model;

	double year = 365*24*3600;
	const int nTimes = 11;
	double Times[nTimes] = {1.*year,10.*year,20.*year,30.*year,40.*year,50.*year,60.*year,70.*year,80.*year,90.*year,100.*year};
	ComputeTransitionProbabilities(prob, outputdir, nboxy, nboxz, nlocy, nlocz, dt, Times, nTimes, true);	
}