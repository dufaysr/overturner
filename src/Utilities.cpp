/*
  Utilities.cpp
  "overturner"

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

void show_usage(std::string name)
{
    std::cerr << "\nUsage: " << name << " <studycase> <studycase_param>\n"
              << "\tor " << name << " -h,--help (displays this help message)\n\n"
              << "<studycase>:\n\n"
              << "\t-t,--trajectories : generates particles trajectories\n"
              << "\t <trajectories_param> : \n"
              << "\t\t-m,--model : model name (there must be a \"model.in\" file).\n"
              << "\t\t-N,--Nloc : number of trajectories generated.\n"
              << "\t\t--ystart : initial y position.\n"
              << "\t\t--zstart : initial z position.\n"
              << "\t\t(--dim) : optional. By default the trajectoires are simulated\n"
              << "\t\ton the adimensionnal model. This option ensures that the\n"
              << "\t\ttrajectories are generated on the dimensionnal model.\n\n"
              << "\t-c,--concentration : compute the concentration of the tracer.\n"
              << "\t<concentration_param> : \n"
              << "\t\t-m,--model : model name (there must be a \"model.in\" file).\n"
              << "\t\t-N,--Nloc : number of trajectories generated.\n"
              << "\t\t--nboxy : number of boxes in the y-drection.\n"
              << "\t\t--nboxz : number of boxes in the z-drection.\n"
              << "\t\t--ystart : initial y position.\n"
              << "\t\t--zstart : initial z position.\n\n"
              << "\t-M,--transition_proba : compute the transition probability matrix M.\n"
              << "\t<transition_proba_param> :\n"
              << "\t\t-m,--model : model name (there must be a \"model.in\" file).\n"
              << "\t\t--nboxy : number of boxes in the y-drection.\n"
              << "\t\t--nboxz : number of boxes in the z-drection.\n"
              << "\t\t--nyloc : dimension of the discretization of the\n"
              << "\t\tindividual boxes along the y-direction.\n"
              << "\t\t--nzloc : dimension of the discretization of the\n"
              << "\t\tindividual boxes along the z-direction.\n\n"
              << std::endl;
}

int get_args_traj(int argc, char *argv[], std::string& model, int& Nloc, double& yStart, double& zStart, bool& adim)
{
    int flag = -4; // the 2 parameters model and Nloc are mandatory
    for (int i = 2; i < argc; i++)
    {
        std::string arg = argv[i];
        if ((arg == "-h") || (arg == "--help"))
        {
            show_usage(argv[0]);
            return 2;
        }
        else if (arg == "--dim")
        {
            adim = false;
        }
        else if ((arg == "-m") || (arg == "--model")) 
        {
            if (i + 1 < argc) 
            { 
                model = argv[++i]; 
                flag++;
            }
            else
            {
                std::cerr << "--model (-m) parameter requires one string argument." << std::endl;
                return 1;
            }
        }
        else if ((arg == "-N") || (arg == "--Nloc")) {
            if (i + 1 < argc)
            { 
                Nloc = std::stoi(argv[++i]);
                flag++;
            }
            else
            {
                std::cerr << "--Nloc (-N) parameter requires one integer argument." << std::endl;
                return 1;
            }  
        }
        else if (arg == "--ystart") {
            if (i + 1 < argc) {
                yStart = std::stod(argv[++i]);
                flag++;
            }
            else {
                std::cerr << "--ystart parameter requires one double argument." << std::endl;
                return 1;
            }  
        }
        else if (arg == "--zstart") {
            if (i + 1 < argc) {
                zStart = std::stod(argv[++i]);
                flag++;
            }
            else {
                std::cerr << "--zstart parameter requires one double argument." << std::endl;
                return 1;
            }  
        }
    }
    return flag;
}

int get_args_conc(int argc, char *argv[], std::string& model, int& Nloc, int& nboxy, int& nboxz, double& yStart, double& zStart)
{
    int flag = -6;
    for (int i = 2; i < argc; i++)
    {
        std::string arg = argv[i];
        if ((arg == "-h") || (arg == "--help"))
        {
            show_usage(argv[0]);
            return 2;
        }
        else if ((arg == "-m") || (arg == "--model")) 
        {
            if (i + 1 < argc) {
                model = argv[++i];
                flag++;
            }
            else {
                std::cerr << "--model (-m) parameter requires one string argument." << std::endl;
                return 1;
            }
        }
        else if ((arg == "-N") || (arg == "--Nloc")) {
            if (i + 1 < argc) {
                Nloc = std::stoi(argv[++i]);
                flag++;
            }
            else {
                std::cerr << "--Nloc (-N) parameter requires one integer argument." << std::endl;
                return 1;
            }  
        }
        else if (arg == "--nboxy") {
            if (i + 1 < argc) {
                nboxy = std::stoi(argv[++i]);
                flag++;
            }
            else {
                std::cerr << "--nboxy parameter requires one integer argument." << std::endl;
                return 1;
            }  
        }
        else if (arg == "--nboxz") {
            if (i + 1 < argc) {
                nboxz = std::stoi(argv[++i]);
                flag++;
            }
            else {
                std::cerr << "--nboxz parameter requires one integer argument." << std::endl;
                return 1;
            }  
        }
        else if (arg == "--ystart") {
            if (i + 1 < argc) {
                yStart = std::stod(argv[++i]);
                flag++;
            }
            else {
                std::cerr << "--ystart parameter requires one double argument." << std::endl;
                return 1;
            }  
        }
        else if (arg == "--zstart") {
            if (i + 1 < argc) {
                zStart = std::stod(argv[++i]);
                flag++;
            }
            else {
                std::cerr << "--zstart parameter requires one double argument." << std::endl;
                return 1;
            }  
        }
    }
    return flag;
}

int get_args_tp(int argc, char *argv[], std::string& model, int& nboxy, int& nboxz, int& nyloc, int& nzloc)
{
    int flag = -5;
    for (int i = 2; i < argc; i++)
    {
        std::string arg = argv[i];
        if ((arg == "-h") || (arg == "--help"))
        {
            show_usage(argv[0]);
            return 2;
        }
        else if ((arg == "-m") || (arg == "--model")) 
        {
            if (i + 1 < argc) {
                model = argv[++i];
                flag++;
            }
            else {
                std::cerr << "--model (-m) parameter requires one string argument." << std::endl;
                return 1;
            }
        }
        else if (arg == "--nboxy") {
            if (i + 1 < argc) {
                nboxy = std::stoi(argv[++i]);
                flag++;
            }
            else {
                std::cerr << "--nboxy parameter requires one integer argument." << std::endl;
                return 1;
            }  
        }
        else if (arg == "--nboxz") {
            if (i + 1 < argc) {
                nboxz = std::stoi(argv[++i]);
                flag++;
            }
            else {
                std::cerr << "--nboxz parameter requires one integer argument." << std::endl;
                return 1;
            }  
        }
        else if (arg == "--nyloc") {
            if (i + 1 < argc) {
                nyloc = std::stoi(argv[++i]);
                flag++;
            }
            else {
                std::cerr << "--nyloc parameter requires one integer argument." << std::endl;
                return 1;
            }  
        }
        else if (arg == "--nzloc") {
            if (i + 1 < argc) {
                nzloc = std::stoi(argv[++i]);
                flag++;
            }
            else {
                std::cerr << "--nzloc parameter requires one integer argument." << std::endl;
                return 1;
            }  
        }
    }
    return flag;
}