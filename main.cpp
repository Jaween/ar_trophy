#include <iostream>

#include "vision.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

const int WAIT_MILLIS = 10;	

int main()
{	
	Vision* vision = new Vision();
	if (vision->initialise("feature_grid.jpg", 0) == false)
		return 1;
	
	while (true)
	{
		
		cv::Mat frame;
		bool homography_found = vision->calculateHomography(frame);
		if (homography_found)
		{
			cv::imshow("Camera Feed", frame);
			
			// Pauses for a keystroke in order to exit
			if (cv::waitKey(WAIT_MILLIS) >= 0)
				break;
		}
	}
	
	delete vision;
	
	return 0;
}
