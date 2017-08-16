/*
  Utilities.cpp
  "sde2D"

  Created by Renaud Dufays on 17/03/17.
  Copyright © 2017. All rights reserved.
*/

#include "Utilities.hpp"

double parseMathExpr(std::string value, double H, double L)
{
    char *pEnd, *pEndtmp;
    double val, tmpval;
    std::transform(value.begin(), value.end(), value.begin(), ::tolower);
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
            else if (pEnd[1] == 'h')
            {
                val *= H;
                pEndtmp += 1;
            }
            else if (pEnd[1] == 'l')
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

std::ofstream openOutputFile(std::string filename, bool binary)
{
    std::ifstream f(filename.c_str());
    bool already_exist = f.is_open();
    f.close();
    if (already_exist)
    {
        std::string action;
        std::cout << "\nFile \"" << filename << "\" already exists." << std::endl;
        std::cout << "Do you want to overwrite this file (type \"o\") or create a new one (type \"c\") ? ";
        std::cin >> action;
        while (action!="o" && action!="c" )
        {
            std::cout << "Choice is incorrect. Type \"o\" to overwrite the file or \"c\" to create a new one : ";
            std::cin >> action;
        }
        if(action=="o")
        {
            std::ofstream fout;
            if (binary) 
                fout.open(filename.c_str(), std::ios::out | std::ios::trunc | std::ios::binary);
            else
                fout.open(filename.c_str(), std::ios::out | std::ios::trunc);

            if (fout)
                return  fout;
            else
            {
                std::cerr << "Did not manage to overwrite the file." << std::endl;
                abort();
            }
        }
        else
        {
            std::string new_output_file;
            std::cout << "Type the new file name. It will be created in " << wd::root << " :" << std::endl;
            std::cin >> new_output_file;
            return openOutputFile(wd::root + new_output_file, binary);
        }
    }
    else
    {
        std::ofstream fout;
        if (binary)
            fout.open(filename.c_str(), std::ios::out | std::ios::trunc | std::ios::binary);
        else
            fout.open(filename.c_str(), std::ios::out | std::ios::trunc);

        if (fout)
            return fout;
        else{
            std::cerr << "Did not manage to open the file. \"" << filename << "\"" << std::endl;
            abort();
        }
    }   
}