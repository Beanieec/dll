#pragma once
#include <string>

class Games
{
public:
	virtual std::string GetStatus() = 0;
	virtual std::string GetName() = 0;
	virtual ~Games() {};
};