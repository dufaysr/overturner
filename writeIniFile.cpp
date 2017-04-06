#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

double getmathline(std::istream& fin, double dt);

using namespace std;

int main()
{
	double Kh, Kv1, Kv2, Kv3, H, L, y0Prime, z0Prime, Psi, dt, T, tmp;
	int N;
	string filename;
	cout << "Please enter the name of your ini file (without the extension \".in\") : ";
	cin >> filename;
	ofstream fout(("in/" + filename + ".in").c_str());
	if (fout)
	{
		cout << "Now enter successively the values of the specified parameters." << endl;
		cout << "dt : "; cin >> dt;
		fout << "dt = " << dt << "\n";
		cout << "T : "; tmp = getmathline(cin,dt); T = getmathline(cin,dt);
		fout << "T = " << T << "\n";
		cout << "N : "; N = getmathline(cin,dt);
		fout << "N = " << N << "\n";
		cout << "Kh : "; Kh = getmathline(cin,dt);
		fout << "Kh = " << Kh << "\n";
		cout << "Kv1 : "; Kv1 = getmathline(cin,dt);
		fout << "Kv1 = " << Kv1 << "\n";
		cout << "Kv2 : "; Kv2 = getmathline(cin,dt);
		fout << "Kv2 = " << Kv2 << "\n";
		cout << "Kv3 : "; Kv3 = getmathline(cin,dt);
		fout << "Kv3 = " << Kv3 << "\n";
		cout << "H : "; H = getmathline(cin,dt);
		fout << "H = " << H << "\n";
		cout << "y0Prime : "; y0Prime = getmathline(cin,dt);
		fout << "y0Prime = " << y0Prime << "\n";
		cout << "z0Prime : "; z0Prime = getmathline(cin,dt);
		fout << "z0Prime = " << z0Prime << "\n";
		cout << "Psi : "; Psi = getmathline(cin,dt);
		fout << "Psi = " << Psi << "\n";
	}
	else
	{
		cerr << "Unable to open \"in/" << filename << "\"." << endl;
		return 1; 
	}
	return 0;
}

double getmathline(std::istream& fin, double DT)
{
	std::string value;
    char *pEnd, *pEndtmp;
    double val, tmpval;	

	std::getline(fin, value);
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
           		val *= DT;
           		pEndtmp += 2;
           	}
        }
        else
            std::cerr << "Unexpected nonnumeric character in \"" << value << "\" : cannot interpret char \"" << pEnd[0] << "\"." << std::endl;
       	pEnd = pEndtmp;
    }
    return val;
}