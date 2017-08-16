#include <iostream>

int main()
{
	int ncelly = 6;
	int ncellz = 3;
	int *map = new int [ncelly*ncellz];
	for (int iy=0; iy<ncelly; iy++)
	{
		for (int iz=0; iz<ncellz; iz++)
		{
			map[iy*ncellz+iz] = (ncelly-1-iy)*ncellz+iz;
			std::cout << iy*ncellz+iz << " maps to " << map[iy*ncellz+iz] << std::endl; 
		}
	}
	return 0;
}
