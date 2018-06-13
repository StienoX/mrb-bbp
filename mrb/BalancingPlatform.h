#pragma once
#include "stdafx.h"
#include <opencv2/core/utility.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include "PID.h"
#include "SerialController.h"
#include <string>

//color modes for detecting ball
enum class Color { red, green, blue, gray };

class BalancingPlatform {
private:

	PID aPID;
	PID bPID;
	PID cPID;
	SerialController ctl;
	cv::Point motorPosA, motorPosB, motorPosC;
	cv::Point setpoint, middle;
	bool calibGiven = false;
	int calibPointIndex = 0;
	cv::Point servoCalib[3];
	std::vector<cv::Vec3f> circles;
	int latestClickX = 0, latestClickY = 0;
	cv::Mat editedFrame;
	std::string windowName;


	void posFromBase(cv::Point & coords, const cv::Point & basePoint) {
		coords -= basePoint;
	}

	void baseFromPos(cv::Point & coords, const cv::Point & basePoint) {
		coords += basePoint;
	}

	inline cv::Point extendPos(const cv::Point & coords, const cv::Point & basePoint) {
		return cv::Point(((coords.y / coords.x) * basePoint.y), basePoint.y);
	}

	inline double getLen(const cv::Point & coords) {
		return sqrt(((coords.x * coords.x) + (coords.y * coords.y)));
	}

	inline double calculateAngle(const cv::Point & a, const cv::Point & b) {
		return ((a.x * b.x) + (a.y * b.y)) / ((sqrt((b.x * b.x) + (b.y * b.y))) * (sqrt((a.x * a.x) + (a.y * a.y))));
	}

	void onMouse(int event, int x, int y, int flag);

	static void cvOnMouse(int event, int x, int y, int flag, void* me) {
		// "me" is, what we fed into setMouseCallback() below:
		BalancingPlatform * myself = (BalancingPlatform *)(me); // cast back to proper class
		myself->onMouse(event, x, y, flag); // call class member
	}

public:


	BalancingPlatform(const std::string & windowName, const SerialController & ctl, const double dt, const double max, const double min, const double Kp, const double Kd, const double Ki);
	int update(cv::Mat & cameraFame, const Color & mode = Color::gray);
	void resetOffset();
	void setNewSetpoint(const cv::Point & setpoint);
	void initDialoge();


	
};