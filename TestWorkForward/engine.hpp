// engine.hpp
// Designed by Anatoly Alexandrov
// code is available only for non-commercial use

#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <vector>
#include "engineParser.hpp"

struct Engine—haracteristics final
{
	std::vector<TorqVelPair> torqueVelocityRelation;
	double inertia_moment;
	double tempSuperheat;
	double coefHeatTorqueRelation;
	double coefHeatVelocityRelation;
	double coefCoolingVelocityRelation;
};
//TODO: module testing with googletest
class Engine
{
public:
	//time - current work time, step_size - precision parameter, envTemp - environment temp
	virtual void engineOn(size_t time, double step_size, double envTemp) = 0;
	virtual double getVelocity() const = 0;
	virtual double getWorkTime() const = 0;
	virtual double getEngineTemp() const = 0;
	virtual double getOverHeatTemp() const = 0;
	virtual ~Engine() {};
};

class CombustionEngine final : public Engine
{
	Engine—haracteristics param;
	std::vector<double> tanTorVel;
	double workTime;
	double velocity;
	double currentEngineTemp;
	double ambientTemp;
public:
	CombustionEngine(Engine—haracteristics& parameters, double envTemp);
	void engineOn(size_t time, double step_size, double envTemp) override;
	double getVelocity() const override;
	double getWorkTime() const override;
	double getEngineTemp() const override;
	double getOverHeatTemp() const override;
};
#endif
