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
#include "BalancingPlatform.h"




//	double Kp = 0.07;
//  double Kd = 1.2;
//  double Ki = 0.0005;

int main(int argc, char **argv) {
	//PID settings
	const double dt = 1;		// 1
	const double max = 110;		// 110
	const double min = 80;		// 80
	const double Kp = 0.08;		// 0.075
	const double Kd = 1.8;		// 1.5
	const double Ki = 0.0006;	// 0.0005


	std::string windowName = "cam";
	SerialController ctl("COM4");
	cv::VideoCapture main_camera(1);

	if (!main_camera.isOpened()) {
		std::cout << "Camera cannot be opened";
	}

	ctl.resetMotors();
	cv::namedWindow(windowName, CV_WINDOW_KEEPRATIO);
	cv::Mat cameraFrame;
	main_camera.read(cameraFrame);
	cv::imshow(windowName, cameraFrame);

	BalancingPlatform bp(windowName,ctl,dt,max,min,Kp,Kd,Ki);
	bp.initDialoge();

	while (true) {

		main_camera.read(cameraFrame);
		bp.update(cameraFrame);
			
		if (cv::waitKey(27) >= 0) break;
	}
	return 0;
}



