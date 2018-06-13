#include "stdafx.h"
#include <stdint.h>
#include "SerialController.h"


SerialController::SerialController(const char *comPort)
{
	serial = new Serial(comPort, CBR_115200);
}

inline bool SerialController::isConnected() {
	return serial->IsConnected();
}

void SerialController::setMotor(Motor motor, char position) {
	char setCommand[4] = { (char) 0xAA, (char) 0x01 };

	switch (motor) {
	case Motor::A:
		setCommand[2] = 0x0A;
		break;
	case Motor::B:
		setCommand[2] = 0x0B;
		break;
	case Motor::C:
		setCommand[2] = 0x0C;
		break;
	}

	setCommand[3] = position;

	sendCommand(setCommand, 4);
}

void SerialController::setMotors(char positionA, char positionB, char positionC) {
	char setCommand[5] = { (char)0xAA, (char)0x04 };

	setCommand[2] = positionA;
	setCommand[3] = positionB;
	setCommand[4] = positionC;

	sendCommand(setCommand, 5);
	receiveResponse();
}

void SerialController::resetMotors() {
	char resetCommand[2] = { (char) 0xAA, (char) 0x02 };

	sendCommand(resetCommand, 2);
	//char response = receiveResponse();
}

void SerialController::sendCommand(const char *cmd, size_t length) {
	if (isConnected()) {
		serial->WriteData(cmd, length);
	}
}

char SerialController::receiveResponse() {
	char readResult[1];

	while (isConnected() && readResult[0] != 0x00) {
		readResult[0] = serial->ReadData(readResult, 1);
	}

	return readResult[0];
}
