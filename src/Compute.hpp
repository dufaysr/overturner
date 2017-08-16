/*
  Compute.hpp
  "overturner"

  Created by Renaud Dufays on 05/07/17
  Copyright © 2017. All rights reserved.
*/

#ifndef COMPUTE_HEADER_DEF
#define COMPUTE_HEADER_DEF

#include <string>
#include <algorithm>
#include <ctime>
#include "Particles2D.hpp"
#include "Utilities.hpp"
#include "Solvers.hpp"
#include "Estimator.hpp"
#include "AdvDiffProblem.hpp"
#include "workingdirectory.hpp"
/*
	The "compute functions" are the main functions of the code. The arguments common to those functions are
	- prob : object that describes the problem (domain, diffusivities, velocities, etc.) See AAdvDiffProblem.hpp
	- outputdir : the outputs are written in the directory "root/outputdir/"" where root is a common root
				  directory for the whole code that can be changed in workingdirectory.hpp (this allows to avoid
				  having to write the whole root directory everytime we need it.).
	- dt : time step
*/

/* COMPUTETRAJECTORIES
Simulate particles trajectories on problem prob, and writes the particles location, the time vector and info about
the problem in the directory root/outputdir/. By default, the particles locations are written every 5 time steps.
Arguments:
	- T : final integration tim
	- part : structure containg the initial particles locations. There are several way to initialize a Particle2D
			 structure (see Particle2D.hpp)
*/
void ComputeTrajectories(const AbstractAdvDiffProblem& prob, std::string outputdir, double dt, double T,
						Particles2D& part);

/* COMPUTECONCENTRATION
Computes the concentration at the times in Times[], on problem prob and for the initial particles repartition part.
Arguments:
	- Times[] : array containing the times at which the concentration has to be computed.
	- nTimes  : size of Times[]
	- part : structure containing the initial particles locations.
	- ncelly, ncellz : number of grid cells in the y- and z-directions, assuming an uniform grid for box-counting
	- binary : if true, the output matrices are written in binary files, which might save time for big matrices.
*/
void ComputeConcentration(const AbstractAdvDiffProblem &prob, std::string outputdir, double dt, double Times[],
						int nTimes, Particles2D& part, int ncelly, int ncellz, bool binary);

/* COMPUTETRANSITIONPROBABILITIES
Computes the transition probability matrix.
Arguments:
	- ncelly, ncellz: number of grid cells in the y- and z-directions, assuming an uniform grid for box-counting
	- nyloc, nzloc : there are initially nyloc*nzloc particles in each grid cell, located according to a uniform
					 nyloc x nzloc grid.
	- Times[] : array containing the times at which the concentration has to be computed.
	- nTimes  : size of Times[]
	- binary : if true, the output matrices are written in binary files, which might save time for big matrices.
*/
void ComputeTransitionProbabilities(const AbstractAdvDiffProblem &prob, std::string outputdir,
									int ncelly, int ncellz, int nyloc, int nzloc, double dt,
									double Times[], int nTimes, bool binary);

/* COMPUTEP2BTRANSITIONPROBABILITIES
Compute the transition probability matrix for the bi-overturner problem "prob" by taking advantage of the symmetry
of the problem, which allows to simulate only half of the particles trajectories. The arguments are the same as for
the "ComputeTransitionProbabilities" method.
*/ 
void ComputeP2BTransitionProbabilities(const Problem2Box& prob, std::string outputdir,
									  int ncelly, int ncellz, int nyloc, int nzloc, double dt, 
									  double Times[], int nTimes, bool binary);

#endif