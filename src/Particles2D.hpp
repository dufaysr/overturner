/*
  Particles2D.hpp
  "overturner"

  Created by Renaud Dufays on 17/03/17.
  Copyright © 2017. All rights reserved.
*/

#ifndef PARTICLES2DHEADERDEF
#define PARTICLES2DHEADERDEF

#include <iostream>
#include <fstream>
#include <cassert>

struct Particles2D
{
	int mN, mNloc; // total number of particles, number of particles per initial position
	double mTime;
	double *mY, *mZ;

	Particles2D(int Nloc, double yStart, double zStart);
	Particles2D(int Nloc, double* yStart, double* zStart, int n); // assume ny = nz = n
	Particles2D(int Nloc, double* yStart, double* zStart, int ny, int nz);
	Particles2D(const Particles2D& otherParticles);
	~Particles2D();
	double& SetTime(double T);
	void Print(std::string model) const;
};

#endif