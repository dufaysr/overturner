#include <iostream>
#include <chrono>
#include <random>


int main()
{
	unsigned seed(std::chrono::system_clock::now().time_since_epoch().count());
	unsigned seed2(std::chrono::system_clock::now().time_since_epoch().count());
	std::default_random_engine generator(seed);
	std::normal_distribution<double> wiener(0.,1.);

	std::default_random_engine generator2(seed2);
	std::normal_distribution<double> wiener2(0.,1.);


	for (int i=0; i<10; i++)
		std::cout << "(wiener, wiener2) = (" << wiener(generator) << ", " << wiener2(generator2) << ")\n";

}