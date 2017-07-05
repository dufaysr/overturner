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

void ComputeTrajectories(const AbstractAdvDiffProblem& prob, std::string model, int Nloc, double yStart, double zStart);
void ComputeTrajectories(const AbstractAdvDiffProblemAdim& prob, std::string model, int Nloc, double yStart, double zStart);
void ComputeConcentration(const AbstractAdvDiffProblem &prob, std::string model, std::string estimator, int Nloc,
							double yStart, double zStart, int nboxy, int nboxz);
void ComputeConcentration(const AbstractAdvDiffProblemAdim &prob, std::string model, std::string estimator, int Nloc,
							double yStart, double zStart, int nboxy, int nboxz);
void ComputeTransitionProbabilities(const AbstractAdvDiffProblemAdim& prob, std::string model, std::string estimator,
									  int nboxy, int nboxz, int nyloc, int nzloc, bool binary=false);
void ComputeTransitionProbabilities(const AbstractAdvDiffProblem& prob, std::string model, std::string estimator,
									  int nboxy, int nboxz, int nyloc, int nzloc, bool binary);
void ComputeTransitionProbabilities(const AbstractAdvDiffProblem& prob, std::string model,
									  int nboxy, int nboxz, int nyloc, int nzloc, double Times[], int nTimes, bool binary);

#endif