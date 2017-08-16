/*
  studyCases.hpp
  "overturner"

  Created by Renaud Dufays on 07/04/17
  Copyright © 2017. All rights reserved.
*/
#ifndef STUDYCASES_HEADER_DEF
#define STUDYCASES_HEADER_DEF

#include <string>
#include <algorithm>
#include <ctime>
#include "Particles2D.hpp"
#include "Utilities.hpp"
#include "Solvers.hpp"
#include "Estimator.hpp"
#include "AdvDiffProblem.hpp"
#include "workingdirectory.hpp"
#include "Compute.hpp"

/*
	Those are the different "mains" used for the sake of the master's thesis
*/

void StudyCaseTestProblem();
void StudyCaseTestProblemSemiInf();
void StudyCaseProblem2BoxTraj();
void StudyCaseProblem2BoxTP();
void StudyCaseP2BConcentration2Comp();
void StudyCaseOverturnerTPnTimes();
#endif