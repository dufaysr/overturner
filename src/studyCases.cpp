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

void StudyCaseProblem2BoxTraj()
{
	/* trajectories */
	double dt = 3600;
	double year = 365*24*3600;
	double T = 1000*year;
	double alpha = .75;
	Problem2Box prob(T,dt,alpha);
	ComputeTrajectories(prob,"data2box",10,-13e6,4.5e3);
}

void StudyCaseProblem2BoxTP()
{
	/* Transition probability matrices */
	double dt = 3600;
	double year = 365*24*3600;
	double T = 1*year;
	double alpha[4] = {0,.5,.9,1};
	int nameindex[4] = {0,5,9,1};

	int nboxy = 30;
	int nboxz = 10;
	int nlocy = 100, nlocz = 100;

	std::string model;
	const int nTimes = 1;
	double Times[nTimes] = {1.*year};

	for (int i=0; i<4; i++){
		Problem2Box prob(T,dt,alpha[i]);
		model = "problem2box_a" + std::to_string(nameindex[i]);
		ComputeTransitionProbabilities(prob, model, nboxy, nboxz, nlocy, nlocz, Times, nTimes, true);
	}	
}

void StudyCaseOverturnerTPnTimes()
{
	std::string model = "timmermans";
	OverturnerProblem prob(model);

	int nboxy = 15;
	int nboxz = 10;
	int nlocy = 100, nlocz = 100;

	double year = 365*24*3600;
	const int nTimes = 11;
	double Times[nTimes] = {1.*year,10.*year,20.*year,30.*year,40.*year,50.*year,60.*year,70.*year,80.*year,90.*year,100.*year};
	ComputeTransitionProbabilities(prob, model, nboxy, nboxz, nlocy, nlocz, Times, nTimes, true);	
}