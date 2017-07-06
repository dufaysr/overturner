#include <iostream>
#include <cmath>

struct LowerTriMatrix
{
  double x[3];
  LowerTriMatrix(double a, double b, double c)
  {
    x[0] = a;
    x[1] = b;
    x[2] = c;
  }
  double operator[](int i) const
  {
  	return x[i];
  }
  double operator()(int i, int j) const
  {
  	return (i>=j)*x[i+j-2];
  }
};

LowerTriMatrix testfct(double x)
{
	LowerTriMatrix out(x,2*x,3*x);
	return out;
}

int main()
{
	double x = 1.5;
	LowerTriMatrix a = testfct(x);
	std::cout << sqrt(a(1,1)) << ", " << a(1,2) << ", " << a(2,1) << ", " << a(2,2) << std::endl;
	return 0;
}