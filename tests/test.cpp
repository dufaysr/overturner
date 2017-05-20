#include "AdvDiffProblem.hpp"

int main()
{
	OverturnerProblem prob("model");
	OverturnerProblemAdim probAdim(prob);
	prob.Display();
	std::cout << std::endl;
	probAdim.Display();
	std::cout << std::endl;
	OverturnerProblemAdim probadim("modeladim");
	probadim.Display();

	return 0;
}