#include <iostream>



int main(int argc, char *argv[])
{
	if (argc < 6)
	{
    	show_usage(argv[0]);
    	return 1;
  	}
  	int flag = 0;
  	std::string studycase = argv[1];
  	if ((studycase == "-t") || (studycase == "--trajectories"))
  	{
  		std::string model;
    	bool adim = true;
    	int Nloc;
  		flag = get_args_traj(argc, argv, model, Nloc, adim);
  		std::cout << "adim = " << adim << std::endl;
    	std::cout << "model : " << model << std::endl;
    	std::cout << "Nloc = " << Nloc << std::endl;
	}
	else if ((studycase == "-c") || (studycase == "--concentration"))
	{
		std::string model;
		int Nloc, dimy, dimz;
		flag = get_args_conc(argc, argv, model, Nloc, dimy, dimz);
    	std::cout << "model : " << model << std::endl;
    	std::cout << "Nloc = " << Nloc << std::endl;
    	std::cout << "dimy = " << dimy << std::endl;
    	std::cout << "dimz = " << dimz << std::endl;
	}
	else if ((studycase == "-M") || (studycase == "--transition_proba"))
	{
		std::string model;
		int nybox, nzbox, dimy, dimz;
		flag = get_args_tp(argc, argv, model, dimy, dimz, nybox, nzbox);
    	std::cout << "model : " << model << std::endl;
    	std::cout << "nybox = " << nybox << std::endl;
    	std::cout << "nzbox = " << nzbox << std::endl;
    	std::cout << "dimy = " << dimy << std::endl;
    	std::cout << "dimz = " << dimz << std::endl;
	}
	if (flag == 1)
		return 1;
	if (flag == 2) // -h or --help option
		return 0;
	if (flag < 0){
		std::cerr << "Not enough parameters specified.\n" << std::endl;
		show_usage(argv[0]);
	}

    return 0;
}