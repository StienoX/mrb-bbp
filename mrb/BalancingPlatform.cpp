#include "BalancingPlatform.h"


BalancingPlatform::BalancingPlatform(const std::string & windowName, const SerialController & ctl, const double dt, const double max, const double min, const double  Kp, const double  Kd, const double Ki):
	windowName(windowName),
	ctl(ctl),
	aPID(dt, max, min, Kp, Kd, Ki),
	bPID(dt, max, min, Kp, Kd, Ki),
	cPID(dt, max, min, Kp, Kd, Ki) {
	cv::setMouseCallback(windowName, cvOnMouse, this);
}

int BalancingPlatform::update(cv::Mat & cameraFrame, const Color & mode) {
	if (!calibGiven) {
		initDialoge();
	}

	switch (mode) {
	case Color::red:

		cv::cvtColor(cameraFrame, editedFrame, cv::COLOR_BGR2HSV);
		cv::inRange(editedFrame, cv::Scalar(159, 80, 80), cv::Scalar(199, 255, 255), editedFrame);
		cv::HoughCircles(editedFrame, circles, CV_HOUGH_GRADIENT, 2, 12, 100, 100, 0, 0);
		break;
	case Color::green:

		cv::cvtColor(cameraFrame, editedFrame, cv::COLOR_BGR2HSV);
		cv::inRange(editedFrame, cv::Scalar(100, 40, 100), cv::Scalar(255, 179, 255), editedFrame);
		cv::HoughCircles(editedFrame, circles, CV_HOUGH_GRADIENT, 1, 12, 200, 220, 0, 0);
		break;
	case Color::blue:
		cv::cvtColor(cameraFrame, editedFrame, cv::COLOR_BGR2HSV);
		cv::inRange(editedFrame, cv::Scalar(100, 100, 100), cv::Scalar(130, 255, 255), editedFrame);
		cv::HoughCircles(editedFrame, circles, CV_HOUGH_GRADIENT, 1, 12, 200, 220, 0, 0);
		break;
	case Color::gray:
		cv::cvtColor(cameraFrame, editedFrame, CV_BGR2GRAY);
		cv::GaussianBlur(editedFrame, editedFrame, cv::Size(9, 9), 1.3);
		cv::HoughCircles(editedFrame, circles, CV_HOUGH_GRADIENT, 2, editedFrame.rows / 4, 100, 40, 13, 22);
		break;
	default:
		std::cout << "Unknown type\n";
		return -1;
		break;
	}

	for (unsigned int i = 0; i < circles.size(); i++)
	{
		cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
		if (center.x == 0 && center.y == 0) {
			std::cout << "No ball found\n";
			return -1;
		}
		else {
			int radius = cvRound(circles[i][2]);
			cv::circle(cameraFrame, center, 3, cv::Scalar(0, 255, 0), -1, 8, 0);
			cv::circle(cameraFrame, center, radius, cv::Scalar(0, 0, 255), 3, 8, 0);
			posFromBase(motorPosA, middle);
			posFromBase(motorPosB, middle);
			posFromBase(motorPosC, middle);
			posFromBase(center, middle);
			posFromBase(setpoint, middle);
			double a2 = calculateAngle(center, motorPosA);
			double b2 = calculateAngle(center, motorPosB);
			double c2 = calculateAngle(center, motorPosC);
			double error = (getLen(center) - getLen(setpoint));
			ctl.setMotors((int)aPID.calculate(a2*error), (int)bPID.calculate(b2*error), (int)cPID.calculate(c2*error));
			baseFromPos(center, middle);
			baseFromPos(setpoint, middle);
			baseFromPos(motorPosA, middle);
			baseFromPos(motorPosB, middle);
			baseFromPos(motorPosC, middle);
		}

		cv::circle(cameraFrame, motorPosA, 3, cv::Scalar(0, 255, 0), -1, 8, 0);
		cv::circle(cameraFrame, motorPosB, 3, cv::Scalar(0, 255, 0), -1, 8, 0);
		cv::circle(cameraFrame, motorPosC, 3, cv::Scalar(0, 255, 0), -1, 8, 0);
		cv::circle(cameraFrame, setpoint, 3, cv::Scalar(255, 0, 0), -1, 8, 0);

		cv::imshow(windowName, cameraFrame);
	}
	return 0;
}

void BalancingPlatform::resetOffset() {
	aPID.resetID();
	bPID.resetID();
	cPID.resetID();
}

void BalancingPlatform::setNewSetpoint(const cv::Point & setpoint) {
	middle = setpoint;
}

void BalancingPlatform::onMouse(int event, int x, int y, int flag) {
	if (event == cv::EVENT_LBUTTONDOWN) {

		//cv::imshow("cam", cameraFrame);

		//std::cout << x << "," << y << '\n';
		latestClickX = x;
		latestClickY = y;
		setpoint.x = x;
		setpoint.y = y;
		middle = setpoint;
		resetOffset();
	}
}

void BalancingPlatform::initDialoge() {
	std::cout << "Click on servo A and press enter\n";

	cv::waitKey(0);

	servoCalib[0].x = latestClickX;
	servoCalib[0].y = latestClickY;

	motorPosA = cv::Point(latestClickX, latestClickY);

	std::cout << "Click on servo B press enter\n";

	cv::waitKey(0);
	servoCalib[1].x = latestClickX;
	servoCalib[1].y = latestClickY;

	motorPosB = cv::Point(latestClickX, latestClickY);

	std::cout << "Click on servo C and press enter\n";
	cv::waitKey(0);
	servoCalib[2].x = latestClickX;
	servoCalib[2].y = latestClickY;

	motorPosC = cv::Point(latestClickX, latestClickY);

	std::cout << "Please click on setpoint and press enter\n";
	cv::waitKey(0);

	setpoint.x = latestClickX;
	setpoint.y = latestClickY;
	middle = setpoint;
	resetOffset();
	calibGiven = true;
	std::cout << "Please put the ball on the platform\n";
}
