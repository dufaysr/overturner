/*
  main.cpp
  "overturner"

  Created by Renaud Dufays on 17/03/17.
  Copyright © 2017. All rights reserved.
*/

#include <iostream>
#include <chrono>
#include <random>
#include <fstream>
#include "Particles2D.hpp"
#include "Solvers.hpp"
#include "Utilities.hpp"
#include "Estimator.hpp"

using namespace constants;
int main()
{
	// remove("out/time.out");
	// remove("out/Y.out");
	// remove("out/Z.out");
	// std::cout << "Kv1 = " << Kv1 << ", Kv2 = " << Kv2 << ", Kv3 = " << Kv3 << "\n";

	// // EMSolver(nParticles, dt, finalTime, yStart, zStart)
	// int nParticles = 10;
	// double dt = 3600*24.;
	// double finalTime = 1000*365*dt;
	// double yStart = .5*L;
	// double zStart = .9*H; 

	// std::ofstream myfileV ("out/V.out", std::ios::out | std::ios::app);
	// std::ofstream myfileW ("out/W.out", std::ios::out | std::ios::app);
	// std::ofstream myfileY ("out/Y.out", std::ios::out | std::ios::app);
	// std::ofstream myfileZ ("out/Z.out", std::ios::out | std::ios::app);
 //  	if (myfileV.is_open())
 //  	{
 //  		double alpha = L/H;
	// 	int nz = 100;
	// 	int ny = 2*nz;
	// 	double dy = L/(ny-1);
	// 	double dz = H/(nz-1);
	// 	for (int j=0; j<nz; j++){
	// 		for (int i=0; i<ny; i++){
	// 			myfileV << GetV(i*dy,j*dz) << " ";
	// 			myfileW << GetW(i*dy,j*dz) << " ";
	// 			myfileY << i*dy << " ";
	// 			myfileZ << j*dz << " ";
	// 		}
	// 		myfileV << "\n";
	// 		myfileW << "\n";
	// 		myfileY << "\n";
	// 		myfileZ << "\n";
	// 	}
 //  		myfileV.close();
 //  		myfileW.close();
 //  		myfileY.close();
 //  		myfileZ.close();
 //  	}
 //  	else
 //  	{
 //  		std::cout << "Unable to open file Out/time.txt\n";
 //  	}
	
	// BISolver solver(nParticles, dt, finalTime, yStart, zStart);
	// solver.Run();
	int N = 1e3;
	Particles2D part(N,.5,.5);
	KernelEstimator epan(101,101,.1*pow(N,-1./6.),"Epanechnikov");
	epan.Estimate(part);
	epan.Print("out/kernelE.out");

	KernelEstimator gaus(101,101,.1*pow(N,-1./6.),"Gaussian");
	gaus.Estimate(part);
	gaus.Print("out/kernelG.out");

	BoxEstimator box(101,101);
	box.Estimate(part);
	box.Print("out/box.out");

	return 0;
}
