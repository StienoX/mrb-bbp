/* 
* SerialControl.h
*
* Created: 24-5-2018 20:10:01
* Author: Wiebe
*/

#ifndef __SERIALCONTROL_H__
#define __SERIALCONTROL_H__

#include "ServoActuator.h"

/**
 * @brief This class is used to handle serial commands to control the board.
 * 
 */
class SerialControl
{
  protected:
	/**
	 * @brief Servo control objects
	 * 
	 */
	ServoActuator &servoA, &servoB, &servoC;

  public:
	/**
	 * @brief Construct a new Serial Control object.
	 * 
	 * @param servoA Servo A.
	 * @param servoB Servo B.
	 * @param servoC Servo C.
	 */
	SerialControl(ServoActuator &servoA, ServoActuator &servoB, ServoActuator &servoC) : servoA(servoA),
																						 servoB(servoB),
																						 servoC(servoC){};

	/**
	 * @brief Begin a serial connection on a desired baudrate.
	 * 
	 * @param baudrate Serial baudrate.
	 */
	void begin(const int baudrate = 115200);

	/**
	 * @brief Polls for new commands, and handles them.
	 * 
	 */
	void loop();

	/**
	 * @brief Reset the servos to the reset position.
	 * 
	 */
	void resetServos();

	/**
	 * @brief Wait for a specified amount of bytes.
	 * 
	 * @param amount Desired amount of bytes to receive.
	 * @param timeout Receive timeout.
	 * @return true Bytes succesfully received.
	 * @return false Timeout triggered, bytes are not received.
	 */
	inline bool waitForBytes(const unsigned int amount, const unsigned int timeout = 20);

}; //SerialControl

#endif //__SERIALCONTROL_H__
