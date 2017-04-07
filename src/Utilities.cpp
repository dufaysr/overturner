/*
  Utilities.cpp
  "overturner"

  Created by Renaud Dufays on 17/03/17.
  Copyright © 2017. All rights reserved.
*/

#include "Utilities.hpp"


namespace parameters
{
	double Kh, Kv1, Kv2, Kv3;
	double H, L;
	double y0Prime, z0Prime, y0, z0;
	double Psi;
    double dt, dtPrime;
    double T, TPrime;
	int Nloc;
}

using namespace parameters;

void ReadIniFile(std::string model)
{
    std::string filename = "in/" + model + ".in";
	std::ifstream iniFile(filename.c_str(), std::ios::in);
    if(iniFile)
    {
        std::string content, tmp, value;
        char *pEnd, *pEndtmp;
        double val, tmpval;
        while(std::getline(iniFile, content,' '))
        {
            std::getline(iniFile,tmp,' ');
        	std::getline(iniFile, value);
            val = strtod(value.c_str(), &pEnd);
            while (pEnd[0])
            {
                if (pEnd[0] == '/')
                    val /= strtod(&(pEnd[1]),&pEndtmp);
                else if (pEnd[0] == '*')
                {
                	tmpval = strtod(&(pEnd[1]),&pEndtmp);
                	if (tmpval)
                		val *= tmpval;
                	else if (strncmp("dt",&(pEnd[1]),2) || strncmp("Dt",&(pEnd[1]),2))
                	{
                		val *= dt;
                		pEndtmp += 2;
                	}
                }
                else
                {
                    std::cerr << "Unexpected nonnumeric character in \"" << value << "\" : cannot interpret char \"" << pEnd[0] << "\"." << std::endl;
                	abort();
                }
            	pEnd = pEndtmp;
            }
        	if (content == "Kh")
        	  	Kh = val;
        	else if (content == "Kv1")
        		Kv1 = val;
        	else if (content == "Kv2")
        		Kv2 = val;
        	else if (content == "Kv3")
        		Kv3 = val;
        	else if (content == "H")
        		H = val;
        	else if (content == "L")
        		L = val;
        	else if (content == "y0Prime")
        		y0Prime = val;
        	else if (content == "z0Prime")
        		z0Prime = val;
        	else if (content == "Psi")
        		Psi = val;
            else if (content == "dt" || content == "Dt")
                dt = val;
            else if (content == "T" || content == "tFinal" || content == "Tfinal")
                T = val;
            else if (content == "Nloc")
                Nloc = int(val);
        	else{
        		std::cerr << "Unexpected content \"" << content << "\" in ini file. Please provide an appropriate ini file" << std::endl;
                abort();
            }
        }
        y0 = y0Prime*L;
        z0 = z0Prime*H;
        dtPrime = dt*Psi/(L*H);
        TPrime = T*Psi/(L*H);
        iniFile.close();

        std::ofstream fInfo("out/" + model + "/info.out", std::ios::out | std::ios::trunc);
        if (fInfo.is_open())
        {
            auto t = std::time(nullptr);
            auto tm = *std::localtime(&t);
            fInfo << "File generated on " << std::put_time(&tm, "%d-%m-%Y at %Hh %Mm %Ss") << "\n";
            fInfo << "Model loaded is " << filename << ".in\n";
            fInfo << "The values used are :\n";
            fInfo << "\tKh = " << Kh << "\n"; 
            fInfo << "\tKv1 = " << Kv1 << "\n"; 
            fInfo << "\tKv2 = " << Kv2 << "\n"; 
            fInfo << "\tKv3 = " << Kv3 << "\n"; 
            fInfo << "\tH = " << H << "\n"; 
            fInfo << "\tL = " << L << "\n"; 
            fInfo << "\ty0Prime = " << y0Prime << "\n"; 
            fInfo << "\ty0 = " << y0 << "\n"; 
            fInfo << "\tz0Prime = " << z0Prime << "\n"; 
            fInfo << "\tz0 = " << z0 << "\n"; 
            fInfo << "\tPsi = " << Psi << "\n"; 
            fInfo << "\tNloc = " << Nloc << "\n"; 
            fInfo << "\tdt = " << dt << "\n"; 
            fInfo << "\tdtPrime = " << dtPrime << "\n"; 
            fInfo << "\tTPrime = " << TPrime << "\n"; 
            fInfo.close();
        }
        else
        {
            std::cerr << "Unable to open file Out/info.out\n";
            abort();
        }
    }
    else
	{
		std::cerr << "Unable to open ini file !" << std::endl;
		abort();
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
	return H*GetV(y,z)/Psi;
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
	return L*GetW(y,z)/Psi;
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