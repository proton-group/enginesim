#include "engine.hpp"

CombustionEngine::CombustionEngine(EngineСharacteristics& parameters, double envTemp) : param(parameters), currentEngineTemp(envTemp), ambientTemp(envTemp)
{
	//проверки на нуль
	for (int i = 0; i < parameters.torqueVelocityRelation.size()-1; i++)
	{
		const auto& torqRight = parameters.torqueVelocityRelation[i + 1].torque;
		const auto& torqLeft = parameters.torqueVelocityRelation[i].torque;
		const auto& velRight = parameters.torqueVelocityRelation[i + 1].velocity;
		const auto& velLeft = parameters.torqueVelocityRelation[i].velocity;
		if(velRight - velLeft == 0)
		{
			throw std::overflow_error("Divide by zero exception");
		}
		tanTorVel.push_back((torqRight - torqLeft) / (velRight - velLeft));
	}
	workTime = 0;
	velocity = 0;
}

void CombustionEngine::engineOn(size_t time, double step_size, double envTemp)
{
	if(time < workTime)
	{
		workTime = 0;
	}
	int linearCounter = 0;
	//Интеграл представил как сумму: w = sigma(M(w)/I*dT)
	for(; workTime <= time; workTime += step_size)
	{
		while(param.torqueVelocityRelation[linearCounter+1].velocity <= velocity)
		{
			linearCounter++;
			if (linearCounter + 1 > param.torqueVelocityRelation.size())
			{
				throw std::out_of_range("The dependence of torque on speed is not known");
			}
		}
		const auto& torqueLastPointValue = param.torqueVelocityRelation[linearCounter].torque;
		const auto& velocityLastPointValue = param.torqueVelocityRelation[linearCounter].velocity;
		const auto& currentTorque = torqueLastPointValue + (velocity - velocityLastPointValue) * tanTorVel[linearCounter];
		if (param.inertia_moment == 0)
		{
			throw std::overflow_error("Divide by zero exception");
		}
		velocity += currentTorque / param.inertia_moment * step_size;
		const auto& heatVel = currentTorque * param.coefHeatTorqueRelation + velocity * velocity * param.coefHeatVelocityRelation;
		const auto& coolVel = param.coefCoolingVelocityRelation * (envTemp - currentEngineTemp);
		currentEngineTemp += (heatVel + coolVel) * step_size;
	}
}

double CombustionEngine::getWorkTime() const
{
	return workTime;
}

double CombustionEngine::getVelocity() const
{
	return velocity;
}
double CombustionEngine::getEngineTemp() const
{
	return currentEngineTemp;
}

double CombustionEngine::getOverHeatTemp() const
{
	return param.tempSuperheat;
}