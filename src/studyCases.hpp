/*
  studyCases.hpp
  "overturner"

  Created by Renaud Dufays on 07/04/17
  Copyright © 2017. All rights reserved.
*/

#include <string>
#include <algorithm>
#include "Particles2D.hpp"
#include "Utilities.hpp"
#include "Solvers.hpp"
#include "Estimator.hpp"
#include "AdvDiffProblem.hpp"
#include "workingdirectory.hpp"

void StudyCaseTrajectories(const AbstractAdvDiffProblem& prob, std::string model, int Nloc, double yStart, double zStart);
void StudyCaseTrajectoriesAdim(const AbstractAdvDiffProblemAdim& prob, std::string model, int Nloc, double yStart, double zStart);
void StudyCaseConcentration(const AbstractAdvDiffProblem &prob, std::string model, std::string estimator, int Nloc,
							double yStart, double zStart, int nboxy, int nboxz);
void StudyCaseConcentrationAdim(const AbstractAdvDiffProblemAdim &prob, std::string model, std::string estimator, int Nloc,
							double yStart, double zStart, int nboxy, int nboxz);
void StudyCaseTransitionProbabilities(const AbstractAdvDiffProblemAdim& prob, std::string model, std::string estimator,
									  int nboxy, int nboxz, int nyloc, int nzloc, bool binary=false);
void StudyCaseTestProblem();
void StudyCaseTestProblemSemiInf();
int StudyCaseComputeNloc(const AbstractAdvDiffProblemAdim& prob, const double epsilon, int nboxy, int nboxz);