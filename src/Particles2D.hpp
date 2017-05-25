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
#include <algorithm>
#include "Utilities.hpp"

struct Particles2D
{
	int mN;
	double mTime;
	double *mY, *mZ;

	Particles2D(int N, double yStart=0., double zStart=0.);
	Particles2D(int N, double* yStart, double* zStart);
	Particles2D(int Nloc, double* yStart, double* zStart, int n); // unused
	Particles2D(int Nloc, double* yStart, double* zStart, int ny, int nz); // unused
	Particles2D(const Particles2D& otherParticles);
	~Particles2D();
	Particles2D& operator=(const Particles2D& p2D);
};

#endif