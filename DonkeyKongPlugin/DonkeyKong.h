#pragma once
#include "Games.h"
class DonkeyKong : public Games
{
public:
	DonkeyKong() {};
	virtual std::string GetStatus();
	virtual std::string GetName();
};

