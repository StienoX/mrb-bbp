#pragma once

#include "Serial.h"

/// Motors available.
enum class Motor {
	A, B, C
};

/**
 * @brief The SerialController class can be used to control the balancing platform.
 * 
 */
class SerialController
{
private:
	/**
	 * @brief Pointer to hardware serial library.
	 * 
	 */
	Serial *serial;

public:
	/**
	 * @brief Construct a new Serial Controller object.
	 * 
	 * @param comPort COM port.
	 */
	SerialController(const char* comPort);

	/**
	 * @brief Check if we are serially connected to the balancing platform.
	 * 
	 * @return true Connected.
	 * @return false Not connected.
	 */
	inline bool isConnected();

	/**
	 * @brief Close the serial connection to the balancing platform.
	 * 
	 */
	void close();

	/**
	 * @brief Set the the motor to a desired position.
	 * 
	 * @param motor Motor to control (see Motor enum).
	 * @param position Desired motor position.
	 */
	void setMotor(Motor motor, char position);

	/**
	 * @brief Set all three motors to their desired positions.
	 * 
	 * @param positionA Desired position motor A.
	 * @param positionB Desired position motor B.
	 * @param positionC Desired position motor C.
	 */
	void setMotors(char positionA, char positionB, char positionC);

	/**
	 * @brief Reset all motors back to their default position.
	 * 
	 */
	void resetMotors();
protected:

	/**
	 * @brief Send a serial command to the Arduino Due.
	 * 
	 * @param cmd Command.
	 * @param length Command packet length.
	 */
	void sendCommand(const char *cmd, size_t length);

	/**
	 * @brief Receive the command response from the Arduino Due.
	 * 
	 * @return char Command response.
	 */
	char receiveResponse();
};

