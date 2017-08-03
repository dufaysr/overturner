#include "AdvDiffProblem.hpp"

/*
	AbstractAdvDiffProblem
*/
AbstractAdvDiffProblem::AbstractAdvDiffProblem(double H0, double H1, double L0, double L1):
	mH0(H0),
	mH1(H1),
	mL0(L0),
	mL1(L1)
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

LowerTriMatrix AbstractAdvDiffProblem::getB(double y, double z) const
{
    SymMatrix D = 2*getK(y,z);
    double B11 = sqrt(D(1,1));
    double B21 = (B11 == 0) ? 0. : D(1,2)/B11;
    double B22 = sqrt(D(2,2)-B21*B21);
    LowerTriMatrix B(B11,B21,B22);

    return B;
}

void AbstractAdvDiffProblem::printInfo(std::ofstream& f) const
{
    f << "H0 = " << mH0 << "\n";
    f << "H1 = " << mH1 << "\n";
    f << "L0 = " << mL0 << "\n";
    f << "L1 = " << mL1 << "\n";
}

/*
	OverturnerProblem
*/
// Default case is using the values from C. Timmermans's master thesis
OverturnerProblem::OverturnerProblem():
	AbstractAdvDiffProblem(0.,5e3,0.,15e6),
	my0(13e6),
	mz0(4000.),
	mKh(1e3),
	mKv1(1e-1), mKv2(1e-4), mKv3(1e-3),
	mPsi(2.)
{}

OverturnerProblem::OverturnerProblem(std::string model):
	AbstractAdvDiffProblem(0.,0.,0.,0.)
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
                val = parseMathExpr(value, mH1, mL1);
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

SymMatrix OverturnerProblem::getK(double y, double z) const
{
    int ChiDomain = (y >= mL0 && y <= mL1 && z >= mH0 && z <= mH1);
	int ChiZ = (z < mz0);
    int ChiY = (y < my0);
    SymMatrix K(ChiDomain*mKh,0,ChiDomain*((1-ChiY)*mKv1 + ChiY*ChiZ*mKv2 + ChiY*(1-ChiZ)*mKv3));

    return K;
}
	
LowerTriMatrix OverturnerProblem::getB(double y, double z) const
{
    SymMatrix D = 2*getK(y,z);
    LowerTriMatrix B(sqrt(D(1,1)),0,sqrt(D(2,2)));

    return B;
}

Vec2 OverturnerProblem::getU(double y, double z) const
{
	int ChiDomain = (y >= mL0 && y <= mL1 && z >= mH0 && z <= mH1);
    Vec2 U(ChiDomain*mPsi*v(y,my0,z,mz0,mL1,mH1), ChiDomain*mPsi*w(y,my0,z,mz0,mL1,mH1));
	return U;
}

void OverturnerProblem::printInfo(std::ofstream& f) const
{
    f << "H0 = " << mH0 << "\n";
    f << "H1 = " << mH1 << "\n";
    f << "L0 = " << mL0 << "\n";
    f << "L1 = " << mL1 << "\n";
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
	cout << "my0 = " << my0 << endl;
	cout << "mz0 = " << mz0 << endl;
	cout << "mKh = " << mKh << endl;
	cout << "mKv1 = " << mKv1 << endl;
	cout << "mKv2 = " << mKv2 << endl;
	cout << "mKv3 = " << mKv3 << endl;
	cout << "mPsi = " << mPsi << endl;
}

/*
    TestProblem
*/
TestProblem::TestProblem(double Ly, double Lz, double Kyy, double Kzz, double V, double W, int J, std::string domain):
    AbstractAdvDiffProblem(-Lz, Lz, -Ly, Ly),
    mKyy(Kyy),
    mKzz(Kzz),
    mV(V),
    mW(W),
    mJ(J)
{
    if (domain == "semi-infinite")
        mH0 = 0.;
}

SymMatrix TestProblem::getK(double y, double z) const
{
    SymMatrix K(mKyy,0,mKzz);
    return K;
}

LowerTriMatrix TestProblem::getB(double y, double z) const
{
    SymMatrix D = 2*getK(y,z);
    LowerTriMatrix B(sqrt(D(1,1)),0,sqrt(D(2,2)));

    return B;
}

Vec2 TestProblem::getU(double x, double y) const
{
    Vec2 U(mV,mW);
    return U;
}

void TestProblem::printInfo(std::ofstream& f) const
{
    f << "H0 = " << mH0 << "\n";
    f << "H1 = " << mH1 << "\n";
    f << "L0 = " << mL0 << "\n";
    f << "L1 = " << mL1 << "\n";
    f << "Kyy = " << mKyy << "\n";
    f << "Kzz = " << mKzz << "\n";
    f << "V = " << mV << "\n";
    f << "W = " << mW << "\n";
    f << "J = " << mJ << "\n";
}


/*
   Problem2Box
*/
Problem2Box::Problem2Box(double alpha):
    AbstractAdvDiffProblem(0.,5e3,-15e6,15e6),
    mzstar(alpha*5e3),
    my0(13e6),
    mz0(4000.),
    mKhm(1e3),
    mKhp(1e5),
    mKv(1e-3),
    mPsi(10.)
{}

double Problem2Box::getzstar() const
{
    return mzstar;
}

double Problem2Box::gety0() const
{
    return my0;
}

double Problem2Box::gety0m() const
{
    return mL0 + my0;
}

double Problem2Box::gety0p() const
{
    return mL1 - my0;
}

double Problem2Box::getz0() const
{
    return mz0;
}

SymMatrix Problem2Box::getK(double y, double z) const
{
    int ChiDomain = (y >= mL0 && y <= mL1 && z >= mH0 && z <= mH1);
    int ChiInterface = (y >= mL0+my0 && y <= mL1-my0);
    int ChiMixing = (ChiInterface && z >= mzstar && z <= mH1);
    int ChiExt =  (ChiDomain && !ChiInterface);
    SymMatrix K(ChiExt*mKhm + ChiMixing*mKhp,0,ChiDomain*mKv);

    return K;
}
    
LowerTriMatrix Problem2Box::getB(double y, double z) const
{
    SymMatrix D = 2*getK(y,z);
    LowerTriMatrix B(sqrt(D(1,1)),0,sqrt(D(2,2)));

    return B;   
}

Vec2 Problem2Box::getU(double y, double z) const
{
    int ChiDomain = (y >= mL0 && y <= mL1 && z >= mH0 && z <= mH1);
    Vec2 U(ChiDomain*mPsi*v2box(y,my0,z,mz0,mL1,mH1),ChiDomain*mPsi*w2box(y,my0,z,mz0,mL1,mH1));
    return U;
}

void Problem2Box::printInfo(std::ofstream& f) const
{
    f << "H0 = " << mH0 << "\n";
    f << "H1 = " << mH1 << "\n";
    f << "L0 = " << mL0 << "\n";
    f << "L1 = " << mL1 << "\n";
    f << "y0 = " << my0 << "\n";
    f << "z0 = " << mz0 << "\n";
    f << "zstar = " << mzstar << "\n";
    f << "Khm = " << mKhm << "\n";
    f << "Khp = " << mKhp << "\n";
    f << "Kv = " << mKv << "\n";
    f << "Psi = " << mPsi << "\n";
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
    int by0m = y < y0 && y >= 0;
    int by0p = y > y0 && y <= L;
    int bz0m = z < z0 && y >= 0;
    int bz0 = z == z0;
    int bz0p = z > z0 && z <= H;

    return by0m*bz0m*(dphi(y,y0)*phi(z,z0))
        +  by0m*bz0p*(dphi(y,y0)*phi(H-z,H-z0))
        +  by0p*bz0m*(-dphi(L-y,L-y0)*phi(z,z0))
        +  by0p*bz0p*(-dphi(L-y,L-y0)*phi(H-z,H-z0))
        +  by0m*bz0*(dphi(y,y0))
        +  by0p*bz0*(-dphi(L-y,L-y0));
}

double v2box(double y, double y0, double z, double z0, double L, double H)
{
    int byl = y < 0;
    int byu = y > 0;
    return byl*v(y+L,y0,z,z0,L,H) - byu*v(y,L-y0,z,z0,L,H);
}

double w2box(double y, double y0, double z, double z0, double L, double H)
{
    int byl = y < 0;
    int byu = y > 0;
    return byl*w(y+L,y0,z,z0,L,H) - byu*w(y,L-y0,z,z0,L,H);
}