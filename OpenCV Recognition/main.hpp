#pragma once
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <boost/log/trivial.hpp>
#include <opencv2/objdetect.hpp>
#include <iostream>
#include "main.hpp"
#include <Windows.h>
#include <chrono>
#include <thread>
#include <string>
#include <cctype>

std::string str_tolower(std::string s) {
	std::transform(s.begin(), s.end(), s.begin(),
		[](unsigned char c) { return std::tolower(c); }
	);
	return s;
}
void picturetogray(std::string& passed);
void getwebcam();
void getvideo();
void getpicture();
void getcurrentmode(std::string &passed);
void askfunction(std::string& passed);
void colordetection(std::string& passed);
void shapedetection(std::string& passed);
void takecontours(cv::Mat pic);
void facerecognition(std::string& passed);
