/*
  Particles2D.hpp
  "sde2D"

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

/*
	Structure that stores particles, where:
		- mN is the number of particles
		- mY stores the y position of each particle (it is thus of size mN)
		- mZ stores the z position of each particle (it is thus of size mN)
		- mT : "age" of the particles
*/
struct Particles2D
{
	int mN;
	double mTime;
	double *mY, *mZ;

	// creates N particles, all at initial position (yStart, zStart)
	Particles2D(int N, double yStart=0., double zStart=0.);
	
	// one particle at every couple (yStart[i],zStart[i]), N couples
	Particles2D(int N, double* yStart, double* zStart);
	
	//	Nloc particles at every couple (yStart[i], zStart[i]).
	//	n is the length of yStart and zStart.
	Particles2D(int Nloc, double* yStart, double* zStart, int n);

	//	Nloc particles at every couple (yStart[i], zStart[j]).
	//	ny is the length of yStart and nz is the length of zStart.
	Particles2D(int Nloc, double* yStart, double* zStart, int ny, int nz);

	// copy constructor
	Particles2D(const Particles2D& otherParticles);
	
	// destructor
	~Particles2D();

	// overload = operator.
	Particles2D& operator=(const Particles2D& p2D);
};

#endif