#include <iostream>
#include <fstream>

using namespace std;
int main()
{
	streampos size;
	char* memblock;
	const int m = 2;
	const int n = 3;
	double** M = new double*[m];
	for (int i=0; i<m; i++)
		M[i] = new double[n];

	const double k = 1.1;
	for (int i=0; i<m; i++)
		for (int j=0; j<n; j++)
			M[i][j] = k*i+j;

	// first method
	ofstream file("test1.bin", ios::out|ios::binary);
	if(file)
	{
		file.write((char*) &M[0][0], m*n*sizeof(double));
	}
	file.close();
	// second method
	ofstream file2("test2.bin", ios::out|ios::binary);
	if (file2)
	{
		for (int i=0; i<m; i++)
			file2.write((char*) &M[i][0], n*sizeof(double));
	}	
	file2.close();
	// third method
	ofstream file3("test3.bin", ios::out|ios::binary);
	if (file3)
	{
		for (int i=0; i<m; i++)
			for (int j=0; j<n; j++)
				file3.write((char*) &M[i][j], sizeof(double));
	}
	file3.close();
	return 0;
}
