#ifndef VISION_H
#define VISION_H

#include <opencv2/core/core.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class Vision
{
	public:
	~Vision();
		bool initialise(const char* path, const int device);
		bool calculateHomography(cv::Mat &camera_frame);
		
	private:
		cv::VideoCapture stream;
		
		cv::Mat base_image;
		std::vector<cv::KeyPoint> keypoints_base;
		cv::Mat descriptors_base;
		
		cv::SiftDescriptorExtractor extractor;
		cv::FeatureDetector* detector;
};
#endif 
