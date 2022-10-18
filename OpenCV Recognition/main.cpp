#include "main.hpp"

int i_getpicture = 0;
int i_getvideo = 0;
int i_getwebcam = 0;

int main()
{
	using std::string;
	string choiceimage = "image";
	string choicevideo = "video";
	string choicewebcam = "webcam";

	string choicemain;
	std::cout << "Image, Video or Webcam \n";
	std::cin >> choicemain;
	if (str_tolower(choicemain) == choiceimage)
	{
		getpicture();
		std::string currentmode = "image";
		getcurrentmode(currentmode);
	}
	if (str_tolower(choicemain) == choicevideo)
	{
		getvideo();
		std::string currentmode = "video";
		getcurrentmode(currentmode);
	}
	if (str_tolower(choicemain) == choicewebcam)
	{
		getwebcam();
		std::string currentmode = "webcam";
		getcurrentmode(currentmode);
	}
	else
	{
		main();
	}
}

void getcurrentmode(std::string &passed)
{
	while (true)
	{
		if (passed == "image")
		{
			std::cout << "current mode is image \n";
			askfunction(passed);
			break;
		}
		if (passed == "video")
		{
			std::cout << "current mode is video \n";
			askfunction(passed);
			break;
		}
		if (passed == "webcam")
		{
			std::cout << "current mode is webcam \n";
			askfunction(passed);
			break;
		}
	}
}

void askfunction(std::string& passed)
{
	BOOST_LOG_TRIVIAL(info) << "asking for function";
	int functionchoice;
	std::cout << "Function List \n";
	std::cout << "1. Picture to Gray \n";
	std::cout << "2. Color Detection \n";
	std::cout << "3. Shape Detection \n";
	std::cout << "4. Face Recognition \n";
	std::cin >> functionchoice;
	if (functionchoice == 1)
	{
		picturetogray(passed);
	}
	if (functionchoice == 2)
	{
		colordetection(passed);
	}
	if (functionchoice == 3)
	{
		shapedetection(passed);
	}
	if (functionchoice == 4)
	{
		facerecognition(passed);
	}
}


void facerecognition(std::string& passed)
{
	if (passed == "image")
	{
		BOOST_LOG_TRIVIAL(info) << "running picture to gray for video";
		std::string path = "C:/vs2022/OpenCV Recognition/TestImages/test.png";
		cv::Mat pic = cv::imread(path);
		cv::CascadeClassifier facec;
		facec.load("C:/vs2022/OpenCV Recognition/TestImages/cac.xml");
		std::vector<cv::Rect> faces;
		facec.detectMultiScale(pic, faces, 1.1, 10);
		for (int i = 0; i < faces.size(); i++)
		{
			rectangle(pic, faces[i].tl(), faces[i].br(), cv::Scalar(0, 255, 0), 2);
		}
		cv::imshow("image", pic);
		cv::waitKey(0);
	}
	if (passed == "video")
	{
		BOOST_LOG_TRIVIAL(info) << "running video for face recognition";
		std::string path = "C:/vs2022/OpenCV Recognition/TestImages/testvideo.mp4";
		cv::VideoCapture capturevideo(path);
		cv::Mat videocap;

		cv::CascadeClassifier facec;
		facec.load("C:/vs2022/OpenCV Recognition/TestImages/cac.xml");
		std::vector<cv::Rect> faces;
		while (true)
		{
			capturevideo.read(videocap);
			facec.detectMultiScale(videocap, faces, 1.1, 10);
			for (int i = 0; i < faces.size(); i++)
			{
				rectangle(videocap, faces[i].tl(), faces[i].br(), cv::Scalar(0, 255, 0), 2);

			}
			cv::imshow("Camera", videocap);
			cv::waitKey(20);

		}
	}
	if (passed == "webcam")
	{
	}
}

void takecontours(cv::Mat imdil, cv::Mat pic)
{
	using namespace std;
	using namespace cv;
	vector<vector<Point>> contours;
	vector<Vec4i> hier;
	cv::findContours(imdil, contours, hier, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
	vector<vector<Point>> contourpoly(contours.size());
	vector<Rect> brect(contours.size());
	string objtype;
	for (int i = 0; i < contours.size(); i++)
	{
			float param = cv::arcLength(contours[i], true);
			cv::approxPolyDP(contours[i],contourpoly[i],0.02 * param, true);
			drawContours(pic, contourpoly, i, Scalar(255, 0, 255), 2);
			brect[i] = cv::boundingRect(contourpoly[i]);
			rectangle(pic, brect[i].tl(), brect[i].br(), Scalar(0, 255, 0), 5);
			int cornnumber = (int)contourpoly[i].size();

			if (cornnumber == 3) { objtype = "Triangle"; }
			if (cornnumber == 4) {

				float aspectratio = (float)brect[i].height / (float)brect[i].width;
				if (aspectratio >= 0.95 && aspectratio <= 1.05)
				{
					objtype = "Square";
				}
				else { objtype = "Rectangle"; }
			}
			if (cornnumber > 4) { objtype = "Circle"; }
			putText(pic, objtype, {brect[i].x, brect[i].y + 2}, FONT_HERSHEY_DUPLEX, 0.75, Scalar(0, 69, 255), 2);
	}
}
void shapedetection(std::string& passed)
{
	cv::Mat imgraysd, imblursd, imcannysd, imdilsd;
	if (passed == "image")
	{
		std::string path = "C:/vs2022/OpenCV Recognition/TestImages/testimageshapes.png";
		cv::Mat pic = cv::imread(path);

		cv::cvtColor(pic, imgraysd, cv::COLOR_BGR2GRAY);
		cv::GaussianBlur(pic, imblursd, cv::Size(3, 3), 3, 0);
		cv::Canny(pic, imcannysd, 25, 75);
		cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
		cv::dilate(imcannysd, imdilsd, kernel);
		takecontours(imdilsd, pic);
		cv::imshow("image", pic);
		cv::waitKey(0);
	}
}

//hard coded to detect red can be changed using the trackbar
void colordetection(std::string& passed) 
{
	cv::Mat picHSV, mask;
	int hmin = 0;
	int smin = 16;
	int vmin = 0;
	int hmax = 9;
	int smax = 255;
	int vmax = 255;
	if (passed == "image")
	{
		std::string path = "C:/vs2022/OpenCV Recognition/TestImages/testimagescolor.png";
		cv::Mat pic = cv::imread(path);

		cv::cvtColor(pic, picHSV, cv::COLOR_BGR2HSV);
		
		cv::namedWindow("Trackbars", (800, 250));
		cv::createTrackbar("H Min", "Trackbars", &hmin, 179);
		cv::createTrackbar("H Max", "Trackbars", &hmax, 179);
		cv::createTrackbar("S Min", "Trackbars", &smin, 255);
		cv::createTrackbar("S Max", "Trackbars", &smax, 255);
		cv::createTrackbar("V Min", "Trackbars", &vmin, 255);
		cv::createTrackbar("V Max", "Trackbars", &vmax, 255);

		while (true)
		{
			cv::Scalar lower(hmin, smin, vmin);
			cv::Scalar upper(hmax, smax, vmax);
			cv::inRange(picHSV, lower, upper, mask);
						
			cv::imshow("image", pic);
			// cv::imshow("image hsv", picHSV);
			cv::imshow("image mask", mask);
			cv::waitKey(1);
		}

	}
	if (passed == "video")
	{
	}
	if (passed == "webcam")
	{
	}
}

void picturetogray(std::string& passed)
{
	if (passed == "image")
	{
		BOOST_LOG_TRIVIAL(info) << "running picture to gray for image";
		std::string path = "C:/vs2022/OpenCV Recognition/TestImages/testimage1.png";
		cv::Mat pic = cv::imread(path);
		cv::Mat imgray;

		cv::cvtColor(pic, imgray, cv::COLOR_BGR2GRAY);
		cv::imshow("Image", pic);
		cv::imshow("Image Gray", imgray);
		cv::waitKey(0);

	}
	if (passed == "video")
	{
		BOOST_LOG_TRIVIAL(info) << "running picture to gray for video";
		std::string path = "C:/vs2022/OpenCV Recognition/TestImages/testvideo.mp4";
		cv::VideoCapture capturevideo(path);
		cv::Mat videocap;
		cv::Mat vdgray;
		while (true)
		{
			capturevideo.read(videocap);
			cv::cvtColor(videocap, vdgray, cv::COLOR_RGB2GRAY);
			cv::imshow("Camera", vdgray);
			cv::waitKey(20);

		}
	}
	if (passed == "webcam")
	{
		BOOST_LOG_TRIVIAL(info) << "running picture to gray for webcam";
		cv::VideoCapture capturecamera(0);
		cv::Mat cameracap;
		cv::Mat wbgray;
		while (true)
		{
			capturecamera.read(cameracap);
			cv::cvtColor(cameracap, wbgray, cv::COLOR_RGB2GRAY);
			cv::imshow("Camera", wbgray);
			cv::waitKey(20);

		}
	}
}

void getwebcam()
{
	cv::Mat cameracap;
	cv::VideoCapture capturecamera(0);
	BOOST_LOG_TRIVIAL(info) << "get webcam test success";
	int i_getwebcam = 1;

}
void getvideo()
{
	std::string path = "C:/vs2022/OpenCV Recognition/TestImages/testvideo.mp4";
	cv::Mat videocap;
	cv::VideoCapture capturevideo(path);
	BOOST_LOG_TRIVIAL(info) << "get video test success";
	int i_getvideo = 1;
}
void getpicture()
{
	BOOST_LOG_TRIVIAL(info) << "get picture test success";
	std::string path = "C:/vs2022/OpenCV Recognition/TestImages/testimage1.png";
	cv::Mat pic = cv::imread(path);
	int i_getpicture = 1;
}