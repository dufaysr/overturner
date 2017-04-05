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
	ReadIniFile("in/model1.in");

	std::cout << "Kh = " << Kh << "\n";
    std::cout << "Kv1 = " << Kv1 << "\n";
    std::cout << "Kv2 = " << Kv2 << "\n";
    std::cout << "Kv3 = " << Kv3 << "\n";
    std::cout << "H = " << H << "\n";
    std::cout << "L = " << L << "\n";
    std::cout << "y0Prime = " << y0Prime << "\n";
    std::cout << "z0Prime = " << z0Prime << "\n";
    std::cout << "Psi = " << Psi << "\n";
    std::cout << "dt = " << dt << "\n";
    std::cout << "T = " << T << "\n";
    std::cout << "N = " << N << "\n";
	// remove("out/time.out");
	// remove("out/Y.out");
	// remove("out/Z.out");
	// std::cout << "Kv1 = " << Kv1 << ", Kv2 = " << Kv2 << ", Kv3 = " << Kv3 << "\n";

	// // EMSolver(nParticles, dt, finalTime, yStart, zStart)
	// int N = 10;
	// double dt = 3600*24.;
	// double finalTime = 1000*365*dt;
	// double yStart = .5*L;
	// double zStart = .9*H; 

	// BISolver solver(N, yStart, zStart, dt, finalTime);
	// Particles2D part = solver.Run();

	// KernelEstimator epan(101,101,.1*pow(N,-1./6.),"Epanechnikov");
	// epan.Estimate(part);
	// epan.Print("out/kernelE.out");

	// KernelEstimator gaus(101,101,.1*pow(N,-1./6.),"Gaussian");
	// gaus.Estimate(part);
	// gaus.Print("out/kernelG.out");

	// BoxEstimator box(101,101);
	// box.Estimate(part);
	// box.Print("out/box.out");

	return 0;
}
