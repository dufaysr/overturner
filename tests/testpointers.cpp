#include <iostream>
#include "../src/Matrix.hpp"

int main()
{
	Matrix *m;
	Matrix m1(2,2);
	m1(0,0) = 1; m1(1,0) = -1; m1(1,1) = 2;

	for (int i=0; i<5; i++){
		m = new Matrix(m1);
		m->Display();
		m1 += 1;
		delete m;
	} 
}