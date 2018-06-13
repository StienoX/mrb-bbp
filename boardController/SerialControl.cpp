/* 
* SerialControl.cpp
*
* Created: 24-5-2018 20:10:01
* Author: Wiebe
*/


#include "SerialControl.h"
#include <Arduino.h>

void SerialControl::begin(const int baudrate) {
	Serial.begin(baudrate);
}

void SerialControl::loop() {
	if (Serial.available() > 0) {
		uint8_t startBit = Serial.read();
		
		if (startBit != 0xAA) {
			return;
		}

		// Wait for the command type byte
		while (Serial.available() == 0);

		uint8_t cmdType = Serial.read();

		if (cmdType == 0x01) { // Move servo to position command
			if (!waitForBytes(2)) {
				return;
			}

			uint8_t servoToControl = Serial.read();
			uint8_t desiredPos = Serial.read();

			bool moveOK = false;

			switch (servoToControl) {
				case 0x0A:
					moveOK = servoA.move(desiredPos);
					break;
				case 0x0B:
					moveOK = servoB.move(desiredPos);
					break;
				case 0x0C:
					moveOK = servoC.move(desiredPos);
					break;
				default:
					// Unable to locate servo.
					break;
			}

			(moveOK) ? Serial.write(0xA0) : Serial.write(0xF0);
		} else if (cmdType == 0x02) { // Reset motors command
			resetServos();
			Serial.write(0xA0);
		} else if (cmdType == 0x03) { // Get motor position
			if (!waitForBytes(1)) {
				return;
			}

			uint8_t servo = Serial.read();
			uint8_t pos = 255;

			switch (servo) {
				case 0x0A:
					pos = servoA.getPosition();
					break;
				case 0x0B:
					pos = servoB.getPosition();
					break;
				case 0x0C:
					pos = servoC.getPosition();
					break;
				default:
					// Unable to locate servo.
					break;
			}

			Serial.write(pos);
		} else if (cmdType == 0x04) {
      if (!waitForBytes(3)) {
        return;
      }

      bool moveA = servoA.move(Serial.read());
      bool moveB = servoB.move(Serial.read());
      bool moveC = servoC.move(Serial.read());

      if (moveA && moveB && moveC) {
        Serial.write(0xA0); 
      } else {
        Serial.write(0xF0);
      }
		}


	}
}

void SerialControl::resetServos() {
	servoA.reset();
	servoB.reset();
	servoC.reset();

	delay(2000); // The motors need some time to move.
}

inline bool SerialControl::waitForBytes(const unsigned int amount, const unsigned int timeout) {
	int curTime = millis();

	while (Serial.available() < amount) {
		if ((millis() - curTime) >= timeout) {
			return false;
		}
	}

	return true;
}
