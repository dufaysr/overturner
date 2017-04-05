/*
  Utilities.cpp
  "overturner"

  Created by Renaud Dufays on 17/03/17.
  Copyright © 2017. All rights reserved.
*/

#include "Utilities.hpp"


using namespace constants;
double GetPhi(double xsi, double xsi0)
{
	int Chi = (xsi < xsi0);
	return Chi*(xsi*(2*xsi0-xsi))/(xsi0*xsi0);
}

double GetdPhi(double xsi, double xsi0)
{
	int Chi = (xsi < xsi0);
	return Chi*(2*(xsi0-xsi))/(xsi0*xsi0);
}

double GetV(double y, double z)
{
	int ChiDomain = (y >= 0 && y <= L && z >= 0 && z <= H);
	return ChiDomain*.25*Psi*(-GetPhi(y,y0)*GetdPhi(z,z0)
		+ GetPhi(y,y0)*GetdPhi(H-z,H-z0)
		+ GetPhi(L-y,L-y0)*GetdPhi(H-z,H-z0)
		- GetPhi(L-y,L-y0)*GetdPhi(z,z0));
}

double GetVPrime(double yPrime, double zPrime)
{
	double y = yPrime*L;
	double z = zPrime*H;
	int ChiDomain = (y >= 0 && y <= L && z >= 0 && z <= H);
	return ChiDomain*.25*H*(-GetPhi(y,y0)*GetdPhi(z,z0)
		+ GetPhi(y,y0)*GetdPhi(H-z,H-z0)
		+ GetPhi(L-y,L-y0)*GetdPhi(H-z,H-z0)
		- GetPhi(L-y,L-y0)*GetdPhi(z,z0));
}

double GetW(double y, double z)
{
	int ChiDomain = (y >= 0 && y <= L && z >= 0 && z <= H);
	return ChiDomain*.25*Psi*(GetdPhi(y,y0)*GetPhi(z,z0)
		+ GetdPhi(y,y0)*GetPhi(H-z,H-z0)
		- GetdPhi(L-y,L-y0)*GetPhi(H-z,H-z0)
		- GetdPhi(L-y,L-y0)*GetPhi(z,z0));
}

double GetWPrime(double yPrime, double zPrime)
{
	double y = yPrime*L;
	double z = zPrime*H;
	int ChiDomain = (y >= 0 && y <= L && z >= 0 && z <= H);
	return ChiDomain*.25*L*(GetdPhi(y,y0)*GetPhi(z,z0)
		+ GetdPhi(y,y0)*GetPhi(H-z,H-z0)
		- GetdPhi(L-y,L-y0)*GetPhi(H-z,H-z0)
		- GetdPhi(L-y,L-y0)*GetPhi(z,z0));
}

double Kv(double y, double z)
{
	int ChiDomain = z >= 0 && z <= H && y >= 0 && y <= L;
	int ChiZ = (z < z0);
	int ChiY = (y < y0);
	return ChiDomain*((1-ChiY)*Kv1 + ChiY*ChiZ*Kv2 + ChiY*(1-ChiZ)*Kv3);
}

double PevInv(double yPrime, double zPrime)
{
	int ChiDomain = zPrime >= 0 && zPrime <= 1 && yPrime >= 0 && yPrime <= 1;
	int ChiYPrime = (yPrime < y0Prime);
	int ChiZPrime = (zPrime < z0Prime);
	double KvTmp = ChiDomain*((1-ChiYPrime)*Kv1 + ChiYPrime*ChiZPrime*Kv2 + ChiYPrime*(1-ChiZPrime)*Kv3);
	return KvTmp*L/(Psi*H);
}

double PehInv(double yPrime, double zPrime)
{
	int ChiDomain = zPrime >= 0 && zPrime <= 1 && yPrime >= 0 && yPrime <= 1;
	return ChiDomain*Kh*H/(Psi*L);
}