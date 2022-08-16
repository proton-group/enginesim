#include <iostream>
#include <fstream>
#include "engineParser.hpp"
#include "engine.hpp"
#include "engineTestBench.hpp"
int main(int argc, char* argv[])
{
	std::cout << "Enter ambient temperature value:\n";
	double ambTemp;
	std::cin >> ambTemp;
	std::fstream in("params.json");
	if (!in)
	{
		std::cout << "JSON file not open\n";
		return -1;
	}
	EngineParser pars(in);
	EngineÑharacteristics params;
	try
	{
		params =
		{
			pars.getTorVelRel(),
			pars.getInertiaMoment(),
			pars.getTempSuperheat(),
			pars.getCoefHeatTorRel(),
			pars.getCoefHeatVelRel(),
			pars.getCoefCoolVelRel()
		};
	}
	catch (std::exception& ex)
	{
		std::cout << ex.what();
		return -1;
	}
	CombustionEngine testEngine(params, ambTemp);
	EngineTestBench testbench;
	double time = 0;
	try
	{
		time = testbench.getTimeFromHeatEngineTest(testEngine, ambTemp);
	}
	catch (std::exception& ex)
	{
		std::cout << ex.what();
		return -1;
	}
	std::cout << "Overheat time: " << time << "\n";
	system("pause");
	return 0;
}