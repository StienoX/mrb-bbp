/* 
* ServoActuator.h
*
* Created: 24-5-2018 20:12:44
* Author: Wiebe
*/


#ifndef __SERVOACTUATOR_H__
#define __SERVOACTUATOR_H__

#include <Servo.h>

class ServoActuator
{
private:
	const uint8_t resetPos = 110;
	const uint8_t servoUpperBoundary = 110;
	const uint8_t servoLowerBoundary = 40;

	Servo servo;

public:
	ServoActuator(const int pin) {
		servo.attach(pin);
	};

	bool move(const uint8_t pos);
	void reset();

	uint8_t getPosition();
}; //ServoActuator

#endif //__SERVOACTUATOR_H__
