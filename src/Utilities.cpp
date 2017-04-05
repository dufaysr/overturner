/*
  Utilities.cpp
  "overturner"

  Created by Renaud Dufays on 17/03/17.
  Copyright © 2017. All rights reserved.
*/

#include "Utilities.hpp"


using namespace constants;

void ReadIniFile(std::string filename)
{
	std::ifstream iniFile(filename.c_str(), std::ios::in);
    if(iniFile)
    {
        std::string content, tmp, value;
        char* pEnd;
        while(std::getline(iniFile, content,' '))
        {
            std::getline(iniFile,tmp,' ');
        	std::getline(iniFile, value);
            double val = strtod(value.c_str(), &pEnd);
            if (pEnd[0])
            {
                if (pEnd[0] == '/')
                    val /= strtod(&(pEnd[1]),NULL);
                else
                    std::cerr << "Unexpected nonnumeric character in \"" << value << "\" : cannot interpret char \"" << pEnd[0] << "\"." << std::endl;
            }
        	if (content == "Kh")
        	  	constants::Kh = val;
        	else if (content == "Kv1")
        		constants::Kv1 = val;
        	else if (content == "Kv2")
        		constants::Kv2 = val;
        	else if (content == "Kv3")
        		constants::Kv3 = val;
        	else if (content == "H")
        		constants::H = val;
        	else if (content == "L")
        		constants::L = val;
        	else if (content == "y0Prime")
        		constants::y0Prime = val;
        	else if (content == "z0Prime")
        		constants::z0Prime = val;
        	else if (content == "Psi")
        		constants::Psi = val;
        	else
        		std::cerr << "Unexpected content \"" << content << "\" in ini file. Please provide an appropriate ini file" << std::endl;
        }
        constants::y0 = constants::y0Prime*constants::L;
        constants::z0 = constants::z0Prime*constants::H;
        iniFile.close();
    }
    else
	{
		std::cerr << "Unable to open ini file !" << std::endl;
	}
}

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