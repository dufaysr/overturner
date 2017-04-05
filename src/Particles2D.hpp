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

class Particles2D
{
	public:
		int mN; // number of particles
		double mTime;
		double *mY, *mZ;

		Particles2D(int nParticles, double yStart, double zStart);
		Particles2D(const Particles2D& otherParticles);
		~Particles2D();
		double GetY(int ithParticle) const; // 1-based indexing
		double GetZ(int ithParticle) const; // 1-based indexing
		double GetN() const;
		double& SetY(int ithParticle);
		double& SetZ(int ithParticle);
		double& SetTime(double T);
		void Print() const;
};

#endif