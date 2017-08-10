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

void ComputeTrajectories(const AbstractAdvDiffProblem& prob, std::string outputdir, double dt, double T,
						int Nloc, double yStart, double zStart);
void ComputeConcentration(const AbstractAdvDiffProblem &prob, std::string outputdir, double dt, double Times[],
						int nTimes, Particles2D& part, int nboxy, int nboxz, bool binary, std::string estimator = "box");
void ComputeTransitionProbabilities(const AbstractAdvDiffProblem& prob, std::string outputdir,
									int nboxy, int nboxz, int nyloc, int nzloc, double dt,
									double Times[], int nTimes, bool binary, std::string estimator = "box");
void ComputeP2BTransitionProbabilities(const Problem2Box& prob, std::string outputdir,
									  int nboxy, int nboxz, int nyloc, int nzloc, double dt, 
									  double Times[], int nTimes, bool binary);

#endif