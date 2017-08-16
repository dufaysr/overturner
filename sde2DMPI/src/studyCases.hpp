/*
  studyCases.hpp
  "overturner"

  Created by Renaud Dufays on 07/04/17
  Copyright © 2017. All rights reserved.
*/

#include <string>
#include "mpi.h"
#include "Particles2D.hpp"
#include "Solvers.hpp"
#include "Estimator.hpp"
#include "AdvDiffProblem.hpp"
#include "workingdirectory.hpp"

void StudyCaseTransitionProbabilitiesMPI(const AbstractAdvDiffProblem& prob, std::string model,
									  int ncelly, int ncellz, int nyloc, int nzloc, 
									  double Times[], int nTimes, bool binary);
