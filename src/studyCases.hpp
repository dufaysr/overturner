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

void StudyCaseTrajectories(AbstractAdvDiffProblem& prob, std::string model, int Nloc, double yStart, double zStart);
void StudyCaseTrajectoriesAdim(AbstractAdvDiffProblemAdim& prob, std::string model, int Nloc, double yStart, double zStart);
void StudyCaseConcentration(AbstractAdvDiffProblem &prob, std::string model, std::string estimator, int Nloc,
							double yStart, double zStart, int nboxy, int nboxz);
void StudyCaseConcentrationAdim(AbstractAdvDiffProblemAdim &prob, std::string model, std::string estimator, int Nloc,
							double yStart, double zStart, int nboxy, int nboxz);
void StudyCaseTransitionProbabilities(AbstractAdvDiffProblemAdim& prob, std::string model, std::string estimator,
									  int nboxy, int nboxz, int nyloc, int nzloc, bool binary=false);
void StudyCaseTestProblem();
void StudyCaseTestProblemSemiInf();