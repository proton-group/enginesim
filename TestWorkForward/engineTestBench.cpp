#include "engineTestBench.hpp"

double EngineTestBench::getTimeFromHeatEngineTest(Engine& testEngine, double ambientTemp)
{
	size_t timeCounter = 0;
	const int step_size = 0.5;
	const double error = 0.5;
	const double maxTime = 5000000;
	while (testEngine.getOverHeatTemp() > testEngine.getEngineTemp() + error)
	{
		testEngine.engineOn(timeCounter, 0.5, ambientTemp);
		timeCounter++;
		if (timeCounter > maxTime)
		{
			throw overheating_not_detected();
		}
	}
	return timeCounter;
}