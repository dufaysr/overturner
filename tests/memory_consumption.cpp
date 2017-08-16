#include <iostream>

int main()
{
	std::cout << "sizeof(double) = " << sizeof(double) 
		<< " bytes" << std::endl;
	int	ncelly = 15;
	int ncellz = 10;
	int nyloc = 100;
	int nzloc = 100;
	std::cout << "Approximate memory consumption : "
		<< (double) (ncelly*ncellz*nyloc*nzloc*sizeof(double))/1e6 
		<< " megabytes." << std::endl;

	return 0;
}
