/* 
* SerialControl.h
*
* Created: 24-5-2018 20:10:01
* Author: Wiebe
*/


#ifndef __SERIALCONTROL_H__
#define __SERIALCONTROL_H__

#include "ServoActuator.h"


class SerialControl
{
protected:
	ServoActuator &servoA, &servoB, &servoC;

public:
	SerialControl(ServoActuator &servoA, ServoActuator &servoB, ServoActuator &servoC) :
		servoA(servoA),
		servoB(servoB),
		servoC(servoC)
	{};
	
	void begin(const int baudrate = 115200);
	void loop();
	void resetServos();
	inline bool waitForBytes(const unsigned int amount, const unsigned int timeout = 20);


}; //SerialControl

#endif //__SERIALCONTROL_H__
