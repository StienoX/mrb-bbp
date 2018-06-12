// mrb.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "PID.h"
#include <opencv2/core/utility.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp> 
#include "SerialController.h"

enum class Color { red, green, blue, gray };

bool calibGiven = false;
int calibPointIndex = 0;
cv::Point servoCalib[3];
cv::Point setpoint(0, 0), middle(0,0);
int latestClickX = 0, latestClickY = 0;
cv::Mat cameraFrame;
cv::Point a(0, 0), b(0, 0), c(0, 0);
const double dt = 1;
const double maxx = 110; //110s
const double minx = 80; //80
const double Kp = 0.075;
const double Kd = 1.5; // 1.5
const double Ki = 0.0005;
//	double Kp = 0.07;
//  double Kd = 1.2;
//  double Ki = 0.0005;
PID aPid(dt, maxx, minx, Kp, Kd, Ki);
PID bPid(dt, maxx, minx, Kp, Kd, Ki);
PID cPid(dt, maxx, minx, Kp, Kd, Ki);

void locator(int event, int x, int y, int flags, void * userdate) {
	if (event == cv::EVENT_LBUTTONDOWN) {
		
		cv::imshow("cam", cameraFrame);

		std::cout << x << "," << y << '\n';
		latestClickX = x;
		latestClickY = y;
		setpoint.x = x;
		setpoint.y = y;
		middle = setpoint;
		aPid.resetID();
		bPid.resetID();
		cPid.resetID();
	}
}

void posFromBase(cv::Point & coords, const cv::Point & basePoint) {
	coords -= basePoint;
}

void baseFromPos(cv::Point & coords, const cv::Point & basePoint) {
	coords += basePoint;
}

cv::Point extendPos(const cv::Point & coords, const cv::Point & basePoint ) {
	return cv::Point(((coords.y / coords.x) * basePoint.y), basePoint.y);
}

double getLen(const cv::Point & coords) {
	return sqrt(((coords.x * coords.x) + (coords.y * coords.y)));
}

double calculateAngle(const cv::Point & a, const cv::Point & b) {
	//return cv::Point((((a.x * b.x) + (a.y * b.y)) * b) / (b.x*b.x+b.y*b.y));
	return ((a.x * b.x) + (a.y * b.y)) / ((sqrt((b.x * b.x) + (b.y * b.y))) * (sqrt((a.x * a.x)  + (a.y * a.y))));
}

int main(int argc, char **argv) {
	std::cout << "Test !\n";
	SerialController ctl("COM4");
	ctl.resetMotors();
	cv::VideoCapture main_camera(1);
	Color mode = Color::gray;

	if (!main_camera.isOpened()) {
		std::cout << "Camera cannot be opened";
	}
	cv::namedWindow("cam", CV_WINDOW_KEEPRATIO);
	cv::setMouseCallback("cam",locator,NULL);
	cv::Rect2d circle;



	//camera test
	/*while (true) {
		main_camera.read(cameraFrame);
		cv::imshow("cam", cameraFrame);
		if (cv::waitKey(27) >= 0) break;
	}*/
	cv::Mat grayCameraFrame;
	cv::Mat hsv_image;
	// Threshold the HSV image, keep only the red pixels
	cv::Mat hue_range;
	std::vector<cv::Vec3f> circles;

	main_camera.read(cameraFrame);

	cv::imshow("cam", cameraFrame);
	//cv::imshow("cam_edit", grayCameraFrame);

	if (!calibGiven) {
		std::cout << "Click on servo A and press enter\n";

		cv::waitKey(0);

		servoCalib[0].x = latestClickX;
		servoCalib[0].y = latestClickY;

		a = cv::Point(latestClickX, latestClickY);

		std::cout << "Click on servo B and press enter\n";

		cv::waitKey(0);
		servoCalib[1].x = latestClickX;
		servoCalib[1].y = latestClickY;

		b = cv::Point(latestClickX, latestClickY);

		std::cout << "Click on servo C and press enter\n";
		cv::waitKey(0);
		servoCalib[2].x = latestClickX;
		servoCalib[2].y = latestClickY;

		c = cv::Point(latestClickX, latestClickY);

		std::cout << "Please click on setpoint and press enter\n";
		cv::waitKey(0);

		setpoint.x = latestClickX;
		setpoint.y = latestClickY;
		middle = setpoint;
	}
	while (true) {
		
		main_camera.read(cameraFrame);

		switch (mode) {
		case Color::red:

			cv::cvtColor(cameraFrame, hsv_image, cv::COLOR_BGR2HSV);
			cv::inRange(hsv_image, cv::Scalar(159, 80, 80), cv::Scalar(199, 255, 255), hue_range);
			//cv::HoughCircles(hue_range, circles, CV_HOUGH_GRADIENT, 1, 12, 200, 220, 0, 0);
			cv::HoughCircles(hue_range, circles, CV_HOUGH_GRADIENT, 2, 12, 100, 100, 0, 0);
			break;
		case Color::green:

			cv::cvtColor(cameraFrame, hsv_image, cv::COLOR_BGR2HSV);
			cv::inRange(hsv_image, cv::Scalar(100, 40, 100), cv::Scalar(255, 179, 255), hue_range);
			cv::HoughCircles(hue_range, circles, CV_HOUGH_GRADIENT, 1, 12, 200, 220, 0, 0);
			break;
		case Color::blue:
			cv::cvtColor(cameraFrame, hsv_image, cv::COLOR_BGR2HSV);
			cv::inRange(hsv_image, cv::Scalar(100, 100, 100), cv::Scalar(130, 255, 255), hue_range);
			cv::HoughCircles(hue_range, circles, CV_HOUGH_GRADIENT, 1, 12, 200, 220, 0, 0);
			break;
		case Color::gray:
			cv::cvtColor(cameraFrame, grayCameraFrame, CV_BGR2GRAY);
			cv::GaussianBlur(grayCameraFrame, grayCameraFrame, cv::Size(9, 9),1.3);
			cv::HoughCircles(grayCameraFrame, circles, CV_HOUGH_GRADIENT, 2, grayCameraFrame.rows/4, 100, 40, 13, 22);
			break;
		default:
			std::cout << "unkown type";
			break;
		}

		for(int i = 0; i < circles.size(); i++)
		{
			cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
			if (center.x == 0 && center.y == 0) {
				std::cout << "no ball found\n";
			}
			else {
				int radius = cvRound(circles[i][2]);
				cv::circle(cameraFrame, center, 3, cv::Scalar(0, 255, 0), -1, 8, 0);
				cv::circle(cameraFrame, center, radius, cv::Scalar(0, 0, 255), 3, 8, 0);
				posFromBase(a, middle);
				posFromBase(b, middle);
				posFromBase(c, middle);
				posFromBase(center, middle);
				posFromBase(setpoint, middle);
				double a2 = calculateAngle(center, a);
				double b2 = calculateAngle(center, b);
				double c2 = calculateAngle(center, c);
				double error = (getLen(center) - getLen(setpoint));
				ctl.setMotors((int)aPid.calculate(a2*error),(int)bPid.calculate(b2*error),(int)cPid.calculate(c2*error));
				std::cout << a2 << std::endl;
				baseFromPos(center, middle);
				baseFromPos(setpoint, middle);
				baseFromPos(a, middle);
				baseFromPos(b, middle);
				baseFromPos(c, middle);
			}

			cv::circle(cameraFrame, a, 3, cv::Scalar(0, 255, 0), -1, 8, 0);
			cv::circle(cameraFrame, b, 3, cv::Scalar(0, 255, 0), -1, 8, 0);
			cv::circle(cameraFrame, c, 3, cv::Scalar(0, 255, 0), -1, 8, 0);
			cv::circle(cameraFrame, setpoint, 3, cv::Scalar(255, 0, 0), -1, 8, 0);

			cv::imshow("cam", cameraFrame);
			cv::imshow("cam_edit", grayCameraFrame);
			//cv::imshow("cam_edit", hue_range);
			}
			



		if (cv::waitKey(27) >= 0) break;
	}
	return 0;
}



