#include "engineParser.hpp"

namespace
{
	int findPosition(std::istream& in, std::string& line, std::string search_name)
	{
		in.seekg(0);
		//исключение, если имя не найдено
		std::string name;
		int startpos, endpos = 0;
		while (name != search_name)
		{
			std::getline(in, line);
			startpos = line.find_first_of('"', 0);
			if (startpos == std::string::npos)
			{
				continue;
			}
			endpos = line.find_first_of('"', startpos + 1);
			name = line.substr(startpos + 1, endpos - (startpos + 1));
			if (in.eof())
			{
				throw InvalidData();
			}
		}
		endpos = line.find_first_of(':', endpos);
		return line.find_first_not_of(' ', endpos + 1);
	}
	void setRelData(std::istream& in, std::vector<TorqVelPair>& rel, std::string quantity)
	{
		std::string line;
		bool exit = true;
		int counter = 0;
		int startpos = 0;
		for (int endpos = 0; exit; counter++)
		{
			std::getline(in, line);
			startpos = line.find_first_not_of(' ', 0);
			endpos = line.find_first_of(',', startpos);
			if (endpos == std::string::npos)
			{
				endpos = line.size();
				exit = false;
			}
			double physdata = std::stod(line.substr(startpos, endpos - startpos));
			if (counter >= rel.size())
			{
				TorqVelPair newPair;
				if (quantity == "torque")
				{
					newPair.torque = physdata;
				}
				else
				{
					newPair.velocity = physdata;
				}
				rel.push_back(newPair);
			}
			else
			{
				if (quantity == "torque")
				{
					rel[counter].torque = physdata;
				}
				else
				{
					rel[counter].velocity = physdata;
				}
			}
		}
		return;
	}
	double getEngineData(std::istream& in, std::string quantity_name, std::exception& exc)
	{
		std::string line;
		int startpos = findPosition(in, line, quantity_name);
		if (line[startpos] == '[')
		{
			throw exc;
		}
		else
		{
			int endpos = line.find_first_of(',', startpos);
			double test = std::stod(line.substr(startpos, endpos - startpos));
			return std::stod(line.substr(startpos, endpos - startpos));
		}
	}
}

double EngineParser::getInertiaMoment()
{
	InvalidInertia exc;
	return getEngineData(in, "inertia_moment", exc);
}

double EngineParser::getTempSuperheat()
{
	InvalidTemp exc;
	return getEngineData(in, "superheat_temp", exc);
}

double EngineParser::getCoefHeatTorRel()
{
	InvalidCoef exc;
	return getEngineData(in, "coefHeatTorqueRelation", exc);
}

double EngineParser::getCoefHeatVelRel()
{
	InvalidCoef exc;
	return getEngineData(in, "coefHeatVelocityRelation", exc);
}

double EngineParser::getCoefCoolVelRel()
{
	InvalidCoef exc;
	return getEngineData(in, "coefCoolingVelocityRelation", exc);
}

std::vector<TorqVelPair> EngineParser::getTorVelRel()
{
	std::string line;
	std::vector<TorqVelPair> torqueVelocityRel;
	int startpos = findPosition(in, line, "torque");
	if (line[startpos] == '[')
	{
		setRelData(in, torqueVelocityRel, "torque");
	}
	else
	{
		throw InvalidTorqueVelocityRel();
	}
	startpos = findPosition(in, line, "velocity");
	if (line[startpos] == '[')
	{
		setRelData(in, torqueVelocityRel, "velocity");
	}
	else
	{
		throw InvalidTorqueVelocityRel();
	}
	return torqueVelocityRel;
}