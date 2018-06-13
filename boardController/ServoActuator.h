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
	/**
	 * @brief Servo init/reset position.
	 * 
	 */
	const uint8_t resetPos = 110;

	/**
	 * @brief Servo upper boundary degrees limit.
	 * 
	 */
	const uint8_t servoUpperBoundary = 110;

	/**
	 * @brief Servo lower boundary degrees limit.
	 * 
	 */
	const uint8_t servoLowerBoundary = 40;

	/**
	 * @brief Arduino PWM controllable servo object.
	 * 
	 */
	Servo servo;

  public:
	/**
	 * @brief Construct a new Servo Actuator object
	 * 
	 * @param pin Arduino pin.
	 */
	ServoActuator(const int pin)
	{
		servo.attach(pin);
	};

	/**
	 * @brief Move the servo to a desired position.
	 * 
	 * @param pos Position.
	 * @return true Servo successfully moved.
	 * @return false Servo did not move.
	 */
	bool move(const uint8_t pos);

	/**
	 * @brief Reset the servo to the reset position.
	 * 
	 */
	void reset();

	/**
	 * @brief Get the current position of the robot.
	 * 
	 * @return uint8_t Motor position.
	 */
	uint8_t getPosition();
}; //ServoActuator

#endif //__SERVOACTUATOR_H__
