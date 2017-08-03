#ifndef STRUCTURES_HEADER_DEF
#define STRUCTURES_HEADER_DEF

#include <cassert>

class LowerTriMatrix
{
	private:
  		double x[3];
  		// Matrix = | x[0]  0   |
  		// 			| x[1] x[2] |
	public:
      LowerTriMatrix();
  		LowerTriMatrix(double a, double b, double c);
  		double& operator()(int i, int j);
  		LowerTriMatrix& operator=(const LowerTriMatrix& ltm);
};

class SymMatrix
{
	private:
  		double x[3];
  		// Matrix = | x[0] x[1] |
  		// 			| x[1] x[2] |
 
	public:
      SymMatrix();
  		SymMatrix(double a, double b, double c);
  		double& operator()(int i, int j);
  		SymMatrix& operator=(const SymMatrix& sm);
};

SymMatrix operator*(double a, SymMatrix sm);

class Vec2
{
	private:
		double x[2];
	public:
    Vec2();
		Vec2(double a, double b);
    double& operator()(int i);
		double& operator[](int i);
		Vec2& operator=(const Vec2& v2);
};

#endif