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
	int nybox, nzbox, Nbox, Nloc, dimy, dimz;
}

// void ReadIniFile(std::string model)
// {
//     std::string filename = wd::root + "in/" + model + ".in";
//     std::ifstream iniFile(filename.c_str(), std::ios::in);
//     std::ofstream fInfo(wd::root + "out/" + model + "/info.out", std::ios::out | std::ios::trunc);
//     if(iniFile && fInfo)
//     {
//         auto t = std::time(nullptr);
//         auto tm = *std::localtime(&t);
//         fInfo << "File generated on " << std::put_time(&tm, "%d-%m-%Y at %Hh %Mm %Ss") << "\n";
//         fInfo << "Model loaded is " << filename << "\n";
//         fInfo << "The values used are :\n";
//         std::string content, tmp, value;
//         double val;
//         bool bH = false, bL = false, bdt = false, bT = false, bnybox = false, bnzbox = false, bPsi = false;
//         while(std::getline(iniFile, content,' '))
//         {
//             std::getline(iniFile,tmp,' ');
//             std::getline(iniFile, value);
//             val = parseMathExpr(value, mdt);
//             if (content == "Kh"){
//                 Kh = val;
//                 fInfo << "\tKh = " << Kh << "\n"; 
//             }
//             else if (content == "Kv1"){
//                 Kv1 = val;
//                 fInfo << "\tKv1 = " << Kv1 << "\n";
//             }
//             else if (content == "Kv2"){
//                 Kv2 = val;
//                 fInfo << "\tKv2 = " << Kv2 << "\n";
//             }
//             else if (content == "Kv3"){
//                 Kv3 = val;
//                 fInfo << "\tKv3 = " << Kv3 << "\n";
//             }
//             else if (content == "H"){
//                 H = val;
//                 fInfo << "\tH = " << H << "\n";
//                 bH = true;
//             }
//             else if (content == "L"){
//                 L = val;
//                 fInfo << "\tL = " << L << "\n";
//                 bL = true;
//             }
//             else if (content == "y0Prime"){
//                 y0Prime = val;
//                 y0 = y0Prime*L;
//                 fInfo << "\ty0Prime = " << y0Prime << "\n";
//                 fInfo << "\ty0 = " << y0 << "\n";
//             }
//             else if (content == "z0Prime"){
//                 z0Prime = val;
//                 z0 = z0Prime*H;
//                 fInfo << "\tz0Prime = " << z0Prime << "\n";
//                 fInfo << "\tz0 = " << z0 << "\n";
//             }
//             else if (content == "Psi"){
//                 Psi = val;
//                 fInfo << "\tPsi = " << Psi << "\n";
//                 bPsi = true;
//             }
//             else if (content == "dt" || content == "Dt"){
//                 dt = val;
//                 fInfo << "\tdt = " << dt << "\n";
//                 bdt = true;
//             }
//             else if (content == "T" || content == "tFinal" || content == "Tfinal"){
//                 T = val;
//                 fInfo << "\tT = " << T << "\n";
//                 bT = true;
//             }
//             else if (content == "nybox"){
//                 nybox = int(val);
//                 fInfo << "\tnybox = " << nybox << "\n";
//                 bnybox = true;
//             }
//             else if (content == "nzbox"){
//                 nzbox = int(val);
//                 fInfo << "\tnzbox = " << nzbox << "\n";
//                 bnzbox = true;
//             }
//             else if (content == "Nloc"){
//                 Nloc = int(val);
//                 fInfo << "\tNloc = " << Nloc << "\n";
//             }
//             else if (content == "dimy" || content == "Dimy" || content == "ny"){
//                 dimy = int(val);
//                 fInfo << "\tdimy = " << dimy << "\n";
//             }
//             else if (content == "dimz" || content == "Dimz" || content == "nz"){
//                 dimz = int(val);
//                 fInfo << "\tdimz = " << dimz << "\n";
//             }
//             else{
//                 std::cerr << "Unexpected content \"" << content 
//                           << "\" in ini file. Please provide an appropriate ini file" << std::endl;
//                 abort();
//             }
//         }
        
//         if (bdt && bT && bH && bL && bPsi)
//         {
//             dtPrime = dt*Psi/(L*H);
//             TPrime = T*Psi/(L*H);
//             fInfo << "\tdtPrime = " << dtPrime << "\n"; 
//             fInfo << "\tTPrime = " << TPrime << "\n";
//         }
//         else
//         {
//             std::cerr << "Uncomplete ini file !" << std::endl;
//             abort();
//         }

//         if (bnybox && bnzbox)
//         {
//             Nbox = nybox*nzbox;
//             fInfo << "\tNbox = " << Nbox << "\n";
//         }

//         iniFile.close();
//         fInfo.close();    
//     }
//     else
//     {
//         if (!iniFile && !fInfo)
//             std::cerr << "Unable to open neither ini file nor out/info.out!" << std::endl;
//         else if (!iniFile)
//             std::cerr << "Unable to open ini file !" << std::endl;
//         else
//             std::cerr << "Unable to open file out/info.out" << std::endl;
//         abort();
//     }
// }

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
            std::cerr << "Did not manage to open the file." << std::endl;
            abort();
        }
    }   
}