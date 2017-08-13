#include <iostream>

int main()
{
	int nboxy = 6;
	int nboxz = 3;
	int *map = new int [nboxy*nboxz];
	for (int iy=0; iy<nboxy; iy++)
	{
		for (int iz=0; iz<nboxz; iz++)
		{
			map[iy*nboxz+iz] = (nboxy-1-iy)*nboxz+iz;
			std::cout << iy*nboxz+iz << " maps to " << map[iy*nboxz+iz] << std::endl; 
		}
	}
	return 0;
}
