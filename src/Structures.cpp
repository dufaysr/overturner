/*
  Structures.cpp
  "sde2D"

  Created by Renaud Dufays.
  Copyright © 2017. All rights reserved.
*/

#include "Structures.hpp"

/* LOWERTRIMATRIX */
LowerTriMatrix::LowerTriMatrix()
{
	x[0] = 0.;
	x[1] = 0.;
	x[2] = 0.;
}

LowerTriMatrix::LowerTriMatrix(double a, double b, double c)
{
	x[0] = a;
	x[1] = b;
	x[2] = c;
}

double& LowerTriMatrix::operator()(int i, int j)
{
	assert(i>=j);
	return x[i+j-2];
}

LowerTriMatrix& LowerTriMatrix::operator=(const LowerTriMatrix& ltm)
{
	x[0] = ltm.x[0];
	x[1] = ltm.x[1];
	x[2] = ltm.x[2];
	return *this;
}

/* SYMMATRIX */
SymMatrix::SymMatrix()
{
	x[0] = 0.;
	x[1] = 0.;
	x[2] = 0.;
}


SymMatrix::SymMatrix(double a, double b, double c)
{
	x[0] = a;
	x[1] = b;
	x[2] = c;
}

double& SymMatrix::operator()(int i, int j)
{
	return x[i+j-2];
}

SymMatrix& SymMatrix::operator=(const SymMatrix& sm)
{
	x[0] = sm.x[0];
	x[1] = sm.x[1];
	x[2] = sm.x[2];
	return *this;
}

SymMatrix operator*(double a, SymMatrix sm)
{
	SymMatrix out(a*sm(1,1),a*sm(1,2),a*sm(2,2));
	return out;
}


/* VEC2 */
Vec2::Vec2()
{
	x[0] = 0.;
	x[1] = 0.;
}

Vec2::Vec2(double a, double b)
{
	x[0] = a;
	x[1] = b;
}

double& Vec2::operator()(int i)
{
	return x[i-1];
}

double& Vec2::operator[](int i)
{
	return x[i];
}

Vec2& Vec2::operator=(const Vec2& v2)
{
	x[0] = v2.x[0];
	x[1] = v2.x[1];
	x[2] = v2.x[2];
	return *this;
}