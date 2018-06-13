/*
 * BalancingBot.ino
 *
 * Created: 5/24/2018 7:54:20 PM
 * Author: Wiebe
 */

 #include "SerialControl.h"

 ServoActuator servoA(9), servoB(10), servoC(11);
 SerialControl serialControl(servoA, servoB, servoC);

void setup()
{
	serialControl.begin();
	delay(200);

	serialControl.resetServos();
}

void loop()
{
	serialControl.loop();
}
