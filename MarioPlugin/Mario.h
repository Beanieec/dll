#pragma once
#include "Games.h"
class Mario : public Games
{
public:
	Mario() {};
	virtual std::string GetStatus();
	virtual std::string GetName();
};

