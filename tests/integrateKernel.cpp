#include <iostream>
#include <cmath>

double Gaussian(double y, double z)
{
	return exp(-.5*(y*y+z*z))/(2*M_PI);
}

double Epanechnikov(double y, double z)
{
	int indicator = (y*y + z*z <= 1.);
	return indicator*2*(1-y*y-z*z)/M_PI;
}

double integrate2D(double (*func)(double x, double y), int n)
{
	double L = 100.;
	double dx = 2*L/(n-1);
	double I = 0.;
	for (int i=0; i<n-1; i++)
	{
		for (int j=0; j<n-1; j++)
		{
			I += .25*dx*dx*(func(-L+i*dx,-L+j*dx)+func(-L+(i+1)*dx,-L+j*dx)+func(-L+(i+1)*dx,-L+(j+1)*dx)+func(-L+i*dx,-L+(j+1)*dx));
		}
	}
	return I;
}

int main()
{
	int n = 10000;
	std::cout << "Integral of the Gaussian = " << integrate2D(Gaussian,n) << std::endl;
	std::cout << "Integral of Epanechnikov = " << integrate2D(Epanechnikov,n) << std::endl;

	return 0;

}