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
void StudyCaseTrajectoriesAdim(std::string model, int Nloc, double yStart, double zStart);
void StudyCaseConcentration(std::string model, std::string estimator, int Nloc, double yStart, double zStart, int dimy, int dimz);
void StudyCaseTransitionProbabilities(std::string model, std::string estimator, int dimy, int dimz, bool binary=false);