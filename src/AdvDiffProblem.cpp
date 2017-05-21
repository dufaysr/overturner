#include "AdvDiffProblem.hpp"

/*
	AbstractAdvDiffProblem
*/
AbstractAdvDiffProblem::AbstractAdvDiffProblem(double H0, double H1, double L0, double L1, double T, double dt):
	mH0(H0),
	mH1(H1),
	mL0(L0),
	mL1(L1),
	mT(T),
	mdt(dt)
{}

double AbstractAdvDiffProblem::getH0() const
{
	return mH0;
}

double AbstractAdvDiffProblem::getH1() const
{
	return mH1;
}

double AbstractAdvDiffProblem::getL0() const
{
	return mL0;
}

double AbstractAdvDiffProblem::getL1() const
{
	return mL1;
}

double AbstractAdvDiffProblem::getT() const
{
	return mT;
}

double AbstractAdvDiffProblem::getdt() const
{
	return mdt;
}

/*
	OverturnerProblem
*/
// Default case is using the values from C. Timmermans's master thesis
OverturnerProblem::OverturnerProblem(double T, double dt):
	AbstractAdvDiffProblem(0.,5e3,0.,15e6,T,dt),
	my0(13e6),
	mz0(4050.),
	mKh(1e3),
	mKv1(1e-1), mKv2(1e-4), mKv3(1e-3),
	mPsi(2.)
{}

OverturnerProblem::OverturnerProblem(std::string model):
	AbstractAdvDiffProblem(0.,0.,0.,0.,0.,0.)
{
	// Read file
	std::string filename = wd::root + "in/" + model + ".in";
    std::ifstream iniFile(filename.c_str(), std::ios::in);
    if(iniFile)
    {
        std::string content, tmp, value;
        double val;
        while(std::getline(iniFile, content,' '))
        {
            if (content[0] == '#')
                std::getline(iniFile,tmp);
            else
            {
                std::transform(content.begin(), content.end(), content.begin(), ::tolower);
                std::getline(iniFile,tmp,' ');
                std::getline(iniFile, value);
                val = parseMathExpr(value, mdt, mH1, mL1);
                if (content == "kh"){
                    mKh = val;
                }
                else if (content == "kv1"){
                    mKv1 = val;
                }
                else if (content == "kv2"){
                    mKv2 = val;
                }
                else if (content == "kv3"){
                    mKv3 = val;
                }
                else if (content == "h"){
                    mH1 = val;
                }
                else if (content == "l"){
                    mL1 = val;
                }
                else if (content == "y0"){
                    my0 = val;
                }
                else if (content == "z0"){
                    mz0 = val;
                }
                else if (content == "psi"){
                    mPsi = val;
                }
                else if (content == "dt"){
                    mdt = val;
                }
                else if (content == "t" || content == "tfinal"){
                    mT = val;
                }
                else if (content[0] != '#'){ // # is used to comment a line in inifile
                	std::cout << "INFO : Ignoring content \"" << content 
                		<< "\" in constructor of OverturnerProblem" << std::endl;
                }
            }
        }
        iniFile.close();
    }
    else
    {
        std::cerr << "Unable to open ini file \"" << filename << "\" !" << std::endl;
        abort();
    }
}

double OverturnerProblem::getKh(double y, double z) const
{
    int ChiDomain = (y >= mL0 && y <= mL1 && z >= mH0 && z <= mH1);
	return ChiDomain*mKh;
}
	
double OverturnerProblem::getKv(double y, double z) const
{
	int ChiDomain = (y >= mL0 && y <= mL1 && z >= mH0 && z <= mH1);
	int ChiZ = (z < mz0);
	int ChiY = (y < my0);
	return ChiDomain*((1-ChiY)*mKv1 + ChiY*ChiZ*mKv2 + ChiY*(1-ChiZ)*mKv3);
}

double OverturnerProblem::getV(double y, double z) const
{
	int ChiDomain = (y >= mL0 && y <= mL1 && z >= mH0 && z <= mH1);
	return ChiDomain*mPsi*v(y,my0,z,mz0,mL1,mH1);
}

double OverturnerProblem::getW(double y, double z) const
{
	int ChiDomain = (y >= mL0 && y <= mL1 && z >= mH0 && z <= mH1);
	return ChiDomain*mPsi*w(y,my0,z,mz0,mL1,mH1);
}

void OverturnerProblem::printInfo(std::ofstream& f) const
{
    f << "H0 = " << mH0 << "\n";
    f << "H1 = " << mH1 << "\n";
    f << "L0 = " << mL0 << "\n";
    f << "L1 = " << mL1 << "\n";
    f << "T = " << mT << "\n";
    f << "dt = " << mdt << "\n";
    f << "y0 = " << my0 << "\n";
    f << "z0 = " << mz0 << "\n";
    f << "Kh = " << mKh << "\n";
    f << "Kv1 = " << mKv1 << "\n";
    f << "Kv2 = " << mKv2 << "\n";
    f << "Kv3 = " << mKv3 << "\n";
    f << "Psi = " << mPsi << "\n";
}

void OverturnerProblem::Display() const
{
	using namespace std;
	cout << "mH0 = " << getH0() << endl;
	cout << "mH1 = " << getH1() << endl;
	cout << "mL0 = " << getL0() << endl;
	cout << "mL1 = " << getL1() << endl;
	cout << "mT = " << getT() << endl;
	cout << "mdt = " << getdt() << endl;
	cout << "my0 = " << my0 << endl;
	cout << "mz0 = " << mz0 << endl;
	cout << "mKh = " << mKh << endl;
	cout << "mKv1 = " << mKv1 << endl;
	cout << "mKv2 = " << mKv2 << endl;
	cout << "mKv3 = " << mKv3 << endl;
	cout << "mPsi = " << mPsi << endl;
}

/*    Adimensionnal Advection-Diffusion problem    */
AbstractAdvDiffProblemAdim::AbstractAdvDiffProblemAdim(double TPrime, double dtPrime, double H, double L):
	mTPrime(TPrime),
	mdtPrime(dtPrime),
    mH(H),
    mL(L)
{}

double AbstractAdvDiffProblemAdim::getTPrime() const
{
	return mTPrime;
}

double AbstractAdvDiffProblemAdim::getdtPrime() const
{
	return mdtPrime;
}

/*
	OverturnerProblem
*/
OverturnerProblemAdim::OverturnerProblemAdim(double TPrime, double dtPrime):
	AbstractAdvDiffProblemAdim(TPrime,dtPrime,5000.,15e6),
	my0Prime(13./15.),
	mz0Prime(4050./5000),
	mPehInv(1./6.), // hence Peh = 6
	mPev1Inv(150), // hence Pev1 = 6.67e-3
	mPev2Inv(.15), // hence Pev2 = 6.67
	mPev3Inv(1.5) // hence Pev3 = 0.667
{}

OverturnerProblemAdim::OverturnerProblemAdim(std::string model):
	AbstractAdvDiffProblemAdim(0.,0.,0.,0.)
{
	// Read file
	std::string filename = wd::root + "in/" + model + ".in";
    std::ifstream iniFile(filename.c_str(), std::ios::in);
    if(iniFile)
    {
        std::string content, tmp, value;
        double val;
        while(std::getline(iniFile, content,' '))
        {
        	if (content[0] == '#')
        		std::getline(iniFile,tmp);
        	else
        	{
        		std::transform(content.begin(), content.end(), content.begin(), ::tolower);
            	std::getline(iniFile,tmp,' ');
            	std::getline(iniFile, value);
            	val = parseMathExpr(value, mdtPrime, 1., 1.);
            	if (content == "pehinv"){
            	    mPehInv = val;
            	}
            	else if (content == "pev1inv"){
            	    mPev1Inv = val;
            	}
            	else if (content == "pev2inv"){
            	    mPev2Inv = val;
            	}
            	else if (content == "pev3inv"){
            	    mPev3Inv = val;
            	}
            	else if (content == "z0prime"){
            	    mz0Prime = val;
            	}
            	else if (content == "y0prime"){
            	    my0Prime = val;
            	}
            	else if (content == "tprime"){
            	    mTPrime = val;
            	}
            	else if (content == "dtprime"){
            	    mdtPrime = val;
            	}
            	else if (content == "h"){
            	    mH = val;
            	}
            	else if (content == "l"){
            	    mL = val;
            	}
            	else{ // # is used to comment a line in inifile
            		std::cout << "INFO : Ignoring content \"" << content 
            			<< "\" in constructor of OverturnerProblemAdim" << std::endl;
            	}
        	}
        }
       	iniFile.close();
    }
    else
    {
        std::cerr << "Unable to open ini file \"" << filename << "\" !" << std::endl;
        abort();
    }
}

OverturnerProblemAdim::OverturnerProblemAdim(OverturnerProblem& prob):
	AbstractAdvDiffProblemAdim(prob.mT*prob.mPsi/(prob.mH1*prob.mL1),
		prob.mdt*prob.mPsi/(prob.mH1*prob.mL1),
        prob.mH1-prob.mH0,
        prob.mL1-prob.mL0)
{
	my0Prime = prob.my0/mL;
	mz0Prime = prob.mz0/mH;
	mPehInv = prob.mKh*mH/(prob.mPsi*mL);
	mPev1Inv = prob.mKv1*mL/(prob.mPsi*mH);
	mPev2Inv = prob.mKv2*mL/(prob.mPsi*mH);
	mPev3Inv = prob.mKv3*mL/(prob.mPsi*mH);
}

double OverturnerProblemAdim::getPehInv(double yPrime, double zPrime) const
{
    int ChiDomain = (yPrime >= 0. && yPrime <= 1. && zPrime >= 0. && zPrime <= 1.);
	return ChiDomain*mPehInv;
}

double OverturnerProblemAdim::getPevInv(double yPrime, double zPrime) const
{
	int ChiDomain = (yPrime >= 0. && yPrime <= 1. && zPrime >= 0. && zPrime <= 1.);
	int ChiZ = (zPrime < mz0Prime);
	int ChiY = (yPrime < my0Prime);
	return ChiDomain*((1-ChiY)*mPev1Inv + ChiY*ChiZ*mPev2Inv + ChiY*(1-ChiZ)*mPev3Inv);
}

double OverturnerProblemAdim::getVPrime(double yPrime, double zPrime) const
{
	int ChiDomain = (yPrime >= 0. && yPrime <= 1. && zPrime >= 0. && zPrime <= 1.);
	return ChiDomain*v(yPrime,my0Prime,zPrime,mz0Prime,1.,1.);
}

double OverturnerProblemAdim::getWPrime(double yPrime, double zPrime) const
{
	int ChiDomain = (yPrime >= 0. && yPrime <= 1. && zPrime >= 0. && zPrime <= 1.);
	return ChiDomain*w(yPrime,my0Prime,zPrime,mz0Prime,1.,1.);
}

void OverturnerProblemAdim::printInfo(std::ofstream& f) const
{
    f << "TPrime = " << mTPrime << "\n";
    f << "dtPrime = " << mdtPrime << "\n";
    f << "y0Prime = " << my0Prime << "\n";
    f << "z0Prime = " << mz0Prime << "\n";
    f << "PehInv = " << mPehInv << "\n";
    f << "Pev1Inv = " << mPev1Inv << "\n";
    f << "Pev2Inv = " << mPev2Inv << "\n";
    f << "Pev3Inv = " << mPev3Inv << "\n";
    f << "H = " << mH << "\n";
    f << "L = " << mL << "\n";
}

void OverturnerProblemAdim::Display() const
{
	using namespace std;
	cout << "mTPrime = " << getTPrime() << endl;
	cout << "mdtPrime = " << getdtPrime() << endl;
	cout << "my0Prime = " << my0Prime << endl;
	cout << "mz0Prime = " << mz0Prime << endl;
	cout << "mPehInv = " << mPehInv << endl;
	cout << "mPev1Inv = " << mPev1Inv << endl;
	cout << "mPev2Inv = " << mPev2Inv << endl;
	cout << "mPev3Inv = " << mPev3Inv << endl;
}

/* Other utility functions */
double phi(double xsi, double xsi0)
{
    return (xsi*(2.*xsi0-xsi))/(xsi0*xsi0);
}

double dphi(double xsi, double xsi0)
{
	return (2.*(xsi0-xsi))/(xsi0*xsi0);
}

double v(double y, double y0, double z, double z0, double L, double H)
{
    int by0m = y < y0;
    int by0 = y == y0;
    int by0p = y > y0;
    int bz0m = z < z0;
    int bz0p = z > z0;
    return by0m*bz0m*(-phi(y,y0)*dphi(z,z0))
        +  by0m*bz0p*(phi(y,y0)*dphi(H-z,H-z0))
        +  by0p*bz0m*(-phi(L-y,L-y0)*dphi(z,z0))
        +  by0p*bz0p*(phi(L-y,L-y0)*dphi(H-z,H-z0))
        +  by0*bz0m*(-dphi(z,z0))
        +  by0*bz0p*(dphi(H-z,H-z0));
}

double w(double y, double y0, double z, double z0, double L, double H)
{
    int by0m = y < y0;
    int by0p = y > y0;
    int bz0m = z < z0;
    int bz0 = z == z0;
    int bz0p = z > z0;

    return by0m*bz0m*(dphi(y,y0)*phi(z,z0))
        +  by0m*bz0p*(dphi(y,y0)*phi(H-z,H-z0))
        +  by0p*bz0m*(-dphi(L-y,L-y0)*phi(z,z0))
        +  by0p*bz0p*(-phi(L-y,L-y0)*phi(H-z,H-z0))
        +  by0m*bz0*(dphi(y,y0))
        +  by0p*bz0*(-dphi(L-y,L-y0));
}