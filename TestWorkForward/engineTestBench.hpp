#ifndef ENGINE_TEST_BENCH_HPP
#define ENGINE_TEST_BENCH_HPP
#include "engine.hpp"

class overheating_not_detected : public std::exception
{
	const char* what() const noexcept { return "Overheating not detected"; }
};

class EngineTestBench
{
public:
	double getTimeFromHeatEngineTest(Engine& testEngine, double ambientTemp);
};

#endif

