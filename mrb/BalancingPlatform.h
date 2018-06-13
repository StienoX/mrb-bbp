#pragma once
#include "stdafx.h"
#include <opencv2/core/utility.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include "PID.h"
#include "SerialController.h"
#include <string>

/// Color modes for detecting ball.
enum class Color { red, green, blue, gray };

/**
 * @brief Used to determine ball position using vision technologies & control PID and so control the servos.
 * 
 */
class BalancingPlatform {
private:

	/**
	 * @brief PID controller for servo A.
	 * 
	 */
	PID aPID;

	/**
	 * @brief PID controller for servo B.
	 * 
	 */
	PID bPID;

	/**
	 * @brief PID controller for servo C.
	 * 
	 */
	PID cPID;

	/**
	 * @brief Serial control interface for the communicating with a Arduino Due.
	 * 
	 */
	SerialController ctl;

	/**
	 * @brief Motor positions on a OpenCV frame.
	 * 
	 */
	cv::Point motorPosA, motorPosB, motorPosC;

	/**
	 * @brief Setpoint and the middle of the board for a OpenCV frame.
	 * 
	 */
	cv::Point setpoint, middle;

	/**
	 * @brief Calibration performed toggle.
	 * 
	 */
	bool calibGiven = false;

	/**
	 * @brief Calibration point index.
	 * 
	 * Index 0 - 3 (one position for each servo)
	 * 
	 */
	int calibPointIndex = 0;

	/**
	 * @brief Lists positions for each servo.
	 * 
	 */
	cv::Point servoCalib[3];

	/**
	 * @brief Drawable circles used to show calibration points after calibration.
	 * 
	 */
	std::vector<cv::Vec3f> circles;

	/**
	 * @brief Holds latest click position.
	 * 
	 */
	int latestClickX = 0, latestClickY = 0;

	/**
	 * @brief Latest edit frame.
	 * 
	 */
	cv::Mat editedFrame;

	/**
	 * @brief Latest cv window frame.
	 * 
	 */
	std::string windowName;

	/**
	 * @brief Return the openCV position from the board base point.
	 * 
	 * By default, the base point should be the setpoint.
	 * 
	 * @param coords Coordinate.
	 * @param basePoint Base point.
	 */
	void posFromBase(cv::Point & coords, const cv::Point & basePoint) {
		coords -= basePoint;
	}

	/**
	 * @brief Return the base position from a OpenCV window point.
	 * 
	 * By default, the base point should be the setpoint.
	 * 
	 * @param coords Coordinate.
	 * @param basePoint Base point.
	 */
	void baseFromPos(cv::Point & coords, const cv::Point & basePoint) {
		coords += basePoint;
	}

	/**
	 * @brief Extend a point to the base point.
	 * 
	 * Make a line from a point to the base point.
	 * 
	 * @param coords OpenCV frame position
	 * @param basePoint Base point.
	 * @return cv::Point OpenCV frame position extended using base point.
	 */
	inline cv::Point extendPos(const cv::Point & coords, const cv::Point & basePoint) {
		return cv::Point(((coords.y / coords.x) * basePoint.y), basePoint.y);
	}

	/**
	 * @brief Get the length of a line using Pythagoras law.
	 * 
	 * @param coords OpenCV frame point.
	 * @return double Length of imaginary line.
	 */
	inline double getLen(const cv::Point & coords) {
		return sqrt(((coords.x * coords.x) + (coords.y * coords.y)));
	}

	/**
	 * @brief Calculate the angle between two points.
	 * 
	 * @param a Coordinate one. 
	 * @param b Coordinate two.
	 * @return double Angle.
	 */
	inline double calculateAngle(const cv::Point & a, const cv::Point & b) {
		return ((a.x * b.x) + (a.y * b.y)) / ((sqrt((b.x * b.x) + (b.y * b.y))) * (sqrt((a.x * a.x) + (a.y * a.y))));
	}

	/**
	 * @brief Event triggered when a mouse press action has been performed on a OpenCV window.
	 * 
	 * @param event Type of mouse event.
	 * @param x Horizontal position of mouse.
	 * @param y Vertical position of mouse.
	 * @param flag Flag.
	 */
	void onMouse(int event, int x, int y, int flag);


	/**
	 * @brief Event triggered when a mouse press action has been performed on a OpenCV window.
	 * 
	 * This method is called by OpenCV when a event occures. The method has to be static.
	 * 
	 * @param event Type of mouse event.
	 * @param x Horizontal position of mouse.
	 * @param y Vertical position of mouse.
	 * @param flag Flag.
	 * @param me 
	 */
	static void cvOnMouse(int event, int x, int y, int flag, void* me) {
		// "me" is, what we fed into setMouseCallback() below:
		BalancingPlatform * myself = (BalancingPlatform *)(me); // cast back to proper class
		myself->onMouse(event, x, y, flag); // call class member
	}

public:


	/**
	 * @brief Construct a new Balancing Platform object
	 * 
	 * @param windowName Name of the OpenCV window.
	 * @param ctl Serial control interface for the balancing platform.
	 * @param dt Delta time constant for PID controller.
	 * @param max Max servo degree.
	 * @param min Min servo degree.
	 * @param Kp Propotional weight factor.
	 * @param Kd Derivative weight factor. 
	 * @param Ki Integral weight factor.
	 */
	BalancingPlatform(const std::string & windowName, const SerialController & ctl, const double dt, const double max, const double min, const double Kp, const double Kd, const double Ki);

	/**
	 * @brief New camera input frame to handle.
	 * 
	 * @param cameraFrame Camera input frame.
	 * @param mode Color seperation mode used.
	 * @return int Zero if executed successfully, something else meant it failed.
	 */
	int update(cv::Mat & cameraFame, const Color & mode = Color::gray);

	/**
	 * @brief Reset the PID controllers.
	 * 
	 */
	void resetOffset();

	/**
	 * @brief Set the new Setpoint.
	 * 
	 * @param setpoint Setpoint coordinate on OpenCV frame.
	 */
	void setNewSetpoint(const cv::Point & setpoint);

	/**
	 * @brief Init calibration dialoge.
	 * 
	 */
	void initDialoge();
};