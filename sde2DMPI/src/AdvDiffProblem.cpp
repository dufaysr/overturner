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
	mz0(4000.),
	mKh(1e3),
	mKv1(1e-1), mKv2(1e-4), mKv3(1e-3),
	mPsi(2.)
{}
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
        +  by0p*bz0p*(-dphi(L-y,L-y0)*phi(H-z,H-z0))
        +  by0m*bz0*(dphi(y,y0))
        +  by0p*bz0*(-dphi(L-y,L-y0));
}