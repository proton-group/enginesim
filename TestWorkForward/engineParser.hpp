#ifndef ENGINE_PARSER_HPP
#define ENGINE_PARSER_HPP

#include <vector>
#include <algorithm>
#include <exception>
#include <string>
#include <istream>

struct TorqVelPair final
{
	double torque;
	double velocity;
};

class InvalidData : public std::exception {};
class InvalidInertia: public InvalidData 
{
	const char* what() const noexcept { return "Invalid inertial param\n"; }
};
class InvalidTemp : public InvalidData
{
	const char* what() const noexcept { return "Invalid temp param\n"; }
};
class InvalidCoef : public InvalidData
{
	const char* what() const noexcept { return "Invalid coef\n"; }
};
class InvalidTorqueVelocityRel : public InvalidData
{
	const char* what() const noexcept { return "Invalid torque\n"; }
};

class EngineParser final
{
	std::istream& in;
public:
	EngineParser(std::istream& _in) : in(_in) {};
	std::vector<TorqVelPair> getTorVelRel();
	double getInertiaMoment();
	double getTempSuperheat();
	double getCoefHeatTorRel();
	double getCoefHeatVelRel();
	double getCoefCoolVelRel();
};

#endif