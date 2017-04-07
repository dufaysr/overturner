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

void StudyCaseTrajectories(std::string model, double yStart, double zStart);
void StudyCaseTrajectoriesAdim(std::string model, double yStart, double zStart);
void StudyCaseConcentration(std::string model, std::string estimator, double yStart, double zStart, int dimy, int dimz);
void StudyCaseTransitionProbabilities(std::string model, std::string estimator, int dimy, int dimz);