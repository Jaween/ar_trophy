#include <iostream>
#include <vector>
#include <string>

#include <opencv2/core/core.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>

const int WAIT_MILLIS = 10;
const cv::Scalar COLOUR = cvScalar(0, 204, 0);
const int RADIUS = 2;
const int THICKNESS = -1;


int main()
{
	// Initialises camera and exits on failure
	cv::VideoCapture stream = cv::VideoCapture(0);
	if (!stream.isOpened())
	{
		std::cout << "Could not start the camera" << std::endl;
		return 1;
	} 
	
	// Image frames
	cv::Mat camera_frame;
	cv::Mat gray_frame;
	
	// Feature points
	cv::FeatureDetector* detector = new cv::SurfFeatureDetector();
	std::vector<cv::KeyPoint> keypoints;
	
	while(true)
	{
		// Reads and grayscales a frame
		stream.read(camera_frame);
		cv::cvtColor(camera_frame, gray_frame, CV_BGR2GRAY);
		
		// Detects features and draws the frame
		detector->detect(gray_frame, keypoints);
		for (int i = 0; i < keypoints.size(); i++)
		{
			cv::circle(camera_frame, keypoints.at(i).pt, RADIUS, COLOUR, THICKNESS);
		}
		cv::imshow("Camera Feed", camera_frame);
		
		// Pauses for a keystroke in order to exit
		if (cv::waitKey(WAIT_MILLIS) >= 0)
			break;
	}
	delete detector;
	return 0;
}
