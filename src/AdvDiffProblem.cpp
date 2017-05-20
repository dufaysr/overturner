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
            std::getline(iniFile,tmp,' ');
            std::getline(iniFile, value);
            val = parseMathExpr(value, mdt, mH1, mL1);
            if (content == "Kh"){
                mKh = val;
            }
            else if (content == "Kv1"){
                mKv1 = val;
            }
            else if (content == "Kv2"){
                mKv2 = val;
            }
            else if (content == "Kv3"){
                mKv3 = val;
            }
            else if (content == "H"){
                mH1 = val;
            }
            else if (content == "L"){
                mL1 = val;
            }
            else if (content == "y0"){
                my0 = val;
            }
            else if (content == "z0"){
                mz0 = val;
            }
            else if (content == "Psi"){
                mPsi = val;
            }
            else if (content == "dt" || content == "Dt"){
                mdt = val;
            }
            else if (content == "T" || content == "tFinal" || content == "Tfinal"){
                mT = val;
            }
            else if (content[0] != '#'){ // # is used to comment a line in inifile
            	std::cout << "INFO : Ignoring content \"" << content 
            		<< "\" in constructor of OverturnerProblem" << std::endl;
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
	return mKh;
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
	return ChiDomain*.25*mPsi*(-getPhi(y,my0)*getdPhi(z,mz0)
		+ getPhi(y,my0)*getdPhi(mH1-z,mH1-mz0)
		+ getPhi(mL1-y,mL1-my0)*getdPhi(mH1-z,mH1-mz0)
		- getPhi(mL1-y,mL1-my0)*getdPhi(z,mz0));
}

double OverturnerProblem::getW(double y, double z) const
{
	int ChiDomain = (y >= mL0 && y <= mL1 && z >= mH0 && z <= mH1);
	return ChiDomain*.25*mPsi*(getdPhi(y,my0)*getPhi(z,mz0)
		+ getdPhi(y,my0)*getPhi(mH1-z,mH1-mz0)
		- getdPhi(mL1-y,mL1-my0)*getPhi(mH1-z,mH1-mz0)
		- getdPhi(mL1-y,mL1-my0)*getPhi(z,mz0));
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
AbstractAdvDiffProblemAdim::AbstractAdvDiffProblemAdim(double TPrime, double dtPrime):
	mTPrime(TPrime),
	mdtPrime(dtPrime)
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
	AbstractAdvDiffProblemAdim(TPrime,dtPrime),
	my0Prime(13./15.),
	mz0Prime(4050./5000),
	mPehInv(1./6.), // hence Peh = 6
	mPev1Inv(150), // hence Pev1 = 6.67e-3
	mPev2Inv(.15), // hence Pev2 = 6.67
	mPev3Inv(1.5), // hence Pev3 = 0.667
	mH(5000),
	mL(15e6)
{}

OverturnerProblemAdim::OverturnerProblemAdim(std::string model):
	AbstractAdvDiffProblemAdim(0.,0.)
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
		prob.mdt*prob.mPsi/(prob.mH1*prob.mL1))
{
	mH = prob.mH1-prob.mH0;
	mL = prob.mL1-prob.mL0;
	my0Prime = prob.my0/mL;
	mz0Prime = prob.mz0/mH;
	mPehInv = prob.mKh*mH/(prob.mPsi*mL);
	mPev1Inv = prob.mKv1*mL/(prob.mPsi*mH);
	mPev2Inv = prob.mKv2*mL/(prob.mPsi*mH);
	mPev3Inv = prob.mKv3*mL/(prob.mPsi*mH);
}

double OverturnerProblemAdim::getPehInv(double yPrime, double zPrime) const
{
	return mPehInv;
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
	return ChiDomain*.25*mH*(-getPhi(yPrime,my0Prime)*getdPhi(zPrime,mz0Prime)
		+ getPhi(yPrime,my0Prime)*getdPhi(1.-zPrime,1.-mz0Prime)
		+ getPhi(1.-yPrime,1.-my0Prime)*getdPhi(1.-zPrime,1.-mz0Prime)
		- getPhi(1.-yPrime,1.-my0Prime)*getdPhi(zPrime,mz0Prime));
}

double OverturnerProblemAdim::getWPrime(double yPrime, double zPrime) const
{
	int ChiDomain = (yPrime >= 0. && yPrime <= 1. && zPrime >= 0. && zPrime <= 1.);
	return ChiDomain*.25*mL*(getdPhi(yPrime,my0Prime)*getPhi(zPrime,mz0Prime)
		+ getdPhi(yPrime,my0Prime)*getPhi(1.-zPrime,1.-mz0Prime)
		- getdPhi(1.-yPrime,1.-my0Prime)*getPhi(1.-zPrime,1.-mz0Prime)
		- getdPhi(1.-yPrime,1.-my0Prime)*getPhi(zPrime,mz0Prime));
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
double getPhi(double xsi, double xsi0)
{
	int Chi = (xsi < xsi0);
	return Chi*(xsi*(2*xsi0-xsi))/(xsi0*xsi0);
}

double getdPhi(double xsi, double xsi0)
{
	int Chi = (xsi < xsi0);
	return Chi*(2*(xsi0-xsi))/(xsi0*xsi0);
}

double parseMathExpr(std::string value, double dt, double H, double L)
{
    char *pEnd, *pEndtmp;
    double val, tmpval;
    val = strtod(value.c_str(), &pEnd);
    while (pEnd[0])
    {
        if (pEnd[0] == '/')
            val /= strtod(&(pEnd[1]),&pEndtmp);
        else if (pEnd[0] == '*')
        {
            tmpval = strtod(&(pEnd[1]),&pEndtmp);
            if (tmpval){
                val *= tmpval;
            }
            else if (strncmp("dt",&(pEnd[1]),2) == 0 || strncmp("Dt",&(pEnd[1]),2) == 0)
            {
                val *= dt;
                pEndtmp += 2;
            }
            else if (pEnd[1] == 'H')
            {
                val *= H;
                pEndtmp += 1;
            }
            else if (pEnd[1] == 'L')
            {
                val *= L;
            	pEndtmp += 1;
            }
            else{
            	std::cerr << "Unexpected nonnumeric character in \"" << value 
                        << "\" : cannot interpret char \"" << pEnd[-1] << "\"." << std::endl;
            	abort();
            }
        }
        else
        {
            std::cerr << "Unexpected nonnumeric character in \"" << value 
                        << "\" : cannot interpret char \"" << pEnd[-1] << "\"." << std::endl;
            abort();
        }
        pEnd = pEndtmp;
    }
    return val;
}