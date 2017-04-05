#include <iostream>
#include <fstream>
#include <string>

namespace constants
{
    double Kh, Kv1, Kv2, Kv3, H, L, y0Prime, z0Prime, y0, z0, Psi;
}

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

using namespace constants;
int main()
{
    ReadIniFile("../in/model1.in");
    std::cout << "Kh = " << Kh << "\n";
    std::cout << "Kv1 = " << Kv1 << "\n";
    std::cout << "Kv2 = " << Kv2 << "\n";
    std::cout << "Kv3 = " << Kv3 << "\n";
    std::cout << "H = " << H << "\n";
    std::cout << "L = " << L << "\n";
    std::cout << "y0Prime = " << y0Prime << "\n";
    std::cout << "z0Prime = " << z0Prime << "\n";
    std::cout << "Psi = " << Psi << "\n";
}
