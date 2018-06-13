/* 
* ServoActuator.cpp
*
* Created: 24-5-2018 20:12:44
* Author: Wiebe
*/


#include <Arduino.h>
#include "ServoActuator.h"

bool ServoActuator::move(const uint8_t pos) {
	// Check if the are in valid servo bounds
	if (pos > servoUpperBoundary || pos < servoLowerBoundary) {
		return false;
	}

	servo.write(pos);
	delay(10 * abs(servo.read() - pos));
	return true;
}

void ServoActuator::reset() {
	servo.write(resetPos);
}

uint8_t ServoActuator::getPosition() {
	return servo.read();
}