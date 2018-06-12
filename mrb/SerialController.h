#pragma once

#include "Serial.h"

enum class Motor {
	A, B, C
};

class SerialController
{
private:
	Serial * serial;

public:
	SerialController(const char* comPort);
	~SerialController();

	inline bool isConnected();

	void close();

	void setMotor(Motor motor, char position);
	void setMotors(char positionA, char positionB, char positionC);

	void resetMotors();
protected:
	void sendCommand(const char *cmd, size_t length);
	char receiveResponse();
};

