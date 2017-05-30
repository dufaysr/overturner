/*
  studyCases.hpp
  "overturner"

  Created by Renaud Dufays on 07/04/17
  Copyright © 2017. All rights reserved.
*/

#include <string>
#include <algorithm>
#include <ctime>
#include "mpi.h"
#include "Particles2D.hpp"
#include "Utilities.hpp"
#include "Solvers.hpp"
#include "Estimator.hpp"
#include "AdvDiffProblem.hpp"
#include "workingdirectory.hpp"

void StudyCaseTrajectories(const AbstractAdvDiffProblem& prob, std::string model, int Nloc, double yStart, double zStart);
void StudyCaseTrajectories(const AbstractAdvDiffProblemAdim& prob, std::string model, int Nloc, double yStart, double zStart);
void StudyCaseConcentration(const AbstractAdvDiffProblem &prob, std::string model, std::string estimator, int Nloc,
							double yStart, double zStart, int nboxy, int nboxz);
void StudyCaseConcentration(const AbstractAdvDiffProblemAdim &prob, std::string model, std::string estimator, int Nloc,
							double yStart, double zStart, int nboxy, int nboxz);
void StudyCaseTransitionProbabilities(const AbstractAdvDiffProblemAdim& prob, std::string model, std::string estimator,
									  int nboxy, int nboxz, int nyloc, int nzloc, bool binary=false);
void StudyCaseTransitionProbabilities(const AbstractAdvDiffProblem& prob, std::string model, std::string estimator,
									  int nboxy, int nboxz, int nyloc, int nzloc, bool binary);
void StudyCaseTransitionProbabilities(const AbstractAdvDiffProblem& prob, std::string model,
									  int nboxy, int nboxz, int nyloc, int nzloc, double Times[], int nTimes, bool binary);
void StudyCaseTransitionProbabilitiesMPI(const AbstractAdvDiffProblem& prob, std::string model,
									  int nboxy, int nboxz, int nyloc, int nzloc, 
									  double Times[], int nTimes, bool binary);
void StudyCaseTestProblem();
void StudyCaseTestProblemSemiInf();
int StudyCaseComputeNloc(const AbstractAdvDiffProblemAdim& prob, const double epsilon, int nboxy, int nboxz);
int StudyCaseComputeNloc(const AbstractAdvDiffProblem& prob, const double epsilon, int nboxy, int nboxz);
