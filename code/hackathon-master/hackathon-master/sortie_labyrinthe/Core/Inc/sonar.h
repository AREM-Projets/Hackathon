#include "tim.h"

#pragma once

enum directionSonar
{
	left,
	front,
	right
};


class Sonar
{
	public :
		Sonar();
		~Sonar();
		void update();
		void lookDir(int dirSonar);
		float getValue(int dirSonar);
	private :
		float leftdist;
		float frontdist;
		float rightdist;
		int timestamp;

};
