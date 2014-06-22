/**
* Description	Manages feature detection and calculating the 3D orientaion of the detected object
**/

#include "vision.h"

#include <iostream>
#include <vector>
#include <string>

#include <glm/glm.hpp>

// OpenCV drawing overlay constants
const int RADIUS = 2;
const int THICKNESS = -1;
const cv::Scalar COLOUR = cvScalar(0, 204, 0);

// OpenCV calculation constants
const int INITIAL_KEYPOINT_MIN_DISTANCE = 100;

Vision::~Vision()
{
	delete detector;
}

bool Vision::initialise(const char* path, const int device)
{
	// Initialises camera and exits on failure
	stream = cv::VideoCapture(device);
	if (!stream.isOpened())
	{
		std::cout << "Could not start the camera" << std::endl;
		return false;
	} 
	
	// Original image
	base_image = cv::imread(path, CV_LOAD_IMAGE_GRAYSCALE);
	
	// Feature points
	detector = new cv::SiftFeatureDetector();
	detector->detect(base_image, keypoints_base);
	std::cout << keypoints_base.size() << " keypoints in base image" <<  std::endl;
	
	// Calculting descriptors for base
	extractor.compute(base_image, keypoints_base, descriptors_base);
	
	return true;
}

bool Vision::calculateOrientation()
{
	// Reads a feed frame and detecets features
	cv::Mat gray_frame;
	stream.read(camera_frame);
	cv::cvtColor(camera_frame, gray_frame, CV_BGR2GRAY);
	std::vector<cv::KeyPoint> keypoints_feed;
	detector->detect(gray_frame, keypoints_feed);
	
	// Matches features of base with feed
	cv::Mat descriptors_feed;
	extractor.compute(gray_frame, keypoints_feed, descriptors_feed);
	std::vector<cv::DMatch> matches;
	cv::FlannBasedMatcher matcher;
	try
	{
		matcher.match(descriptors_base, descriptors_feed, matches);
	}
	catch (cv::Exception e)
	{
		return false;	
	}
	
	// Calculates minimum distance between keypoints
	double min_distance = INITIAL_KEYPOINT_MIN_DISTANCE;
	for (int i = 0; i < descriptors_base.rows; i++)
	{
		double distance = matches[i].distance;
		if(distance < min_distance)
			min_distance = distance;
	}
	
	// Finds the good matches (those with a small distance)
	std::vector<cv::DMatch> good_matches;
	for (int i = 0; i < descriptors_base.rows; i++)
		if (matches[i].distance < 3 * min_distance)
			good_matches.push_back(matches[i]);
	
	std::vector<cv::Point2f> points_base;
	std::vector<cv::Point2f> points_feed;
	
	for (int i = 0; i < good_matches.size(); i++)
	{
		points_base.push_back(keypoints_base[good_matches[i].queryIdx].pt);
		points_feed.push_back(keypoints_feed[good_matches[i].trainIdx].pt);
	}
	
	// Calculates transformation matrix from the base to the feed
	try
	{
		homography_matrix = findHomography(points_base, points_feed, CV_RANSAC);
	}
	catch (cv::Exception e)
	{
		return false;
	}
	
	// TODO: Calculates up vector 
	cv::Point tl, tr, bl;
	tl.x = 0;
	tl.y = 0;
	tr.x = base_image.cols;
	tr.y = 0;
	bl.x = 0;
	bl.y = base_image.rows;
	
	//glm::vec3 OA = glm::vec3(base_image.cols, 0, 1);
	//glm::mat3 matrix = glm::make_mat3(homography_matrix.ptr<double>(0));
	/*cv::Mat OA = (cv::Mat_<double>(1, 3) << base_image.cols, 0, 1);
	cv::Mat OB = (cv::Mat_<double>(1, 3) << 0, base_image.rows, 1);
	cv::Mat OA_dash = OA * homography_matrix;
	cv::Mat OB_dash = OB * homography_matrix;
	cv::Mat OA_dash_norm = OA_dash * (1/OA_dash.at<double>(1, 3));
	cv::Mat OB_dash_norm = OB_dash * (1/OB_dash.at<double>(1, 3));
	
	cv::Mat OA_dn_cross_OB_dn = OA_dash_norm.cross(OB_dash_norm);
	std::cout << "Cross is " << OA_dn_cross_OB_dn << std::endl;*/
	
	// Draws a quad around the object in the camera feed
	std::vector<cv::Point2f> base_corners(4);
	base_corners[0] = cvPoint(0,0);
	base_corners[1] = cvPoint(base_image.cols, 0);
	base_corners[2] = cvPoint(base_image.cols, base_image.rows);
	base_corners[3] = cvPoint(0, base_image.rows);
	
	std::vector<cv::Point2f> feed_corners(4);
	perspectiveTransform(base_corners, feed_corners, homography_matrix);
	
	line(camera_frame, feed_corners[0], feed_corners[1], COLOUR, 4);
	line(camera_frame, feed_corners[1], feed_corners[2], COLOUR, 4);
	line(camera_frame, feed_corners[2], feed_corners[3], COLOUR, 4);
	line(camera_frame, feed_corners[3], feed_corners[0], COLOUR, 4);
	
	return true;
}

void Vision::getOrientation()
{
	//return orientation;
}

void Vision::getDimensions(int &width, int &height)
{
	stream.read(camera_frame);
	width = camera_frame.cols;
	height = camera_frame.rows;
}

unsigned char* Vision::getFrameData()
{
	// TODO: Remove this stream.read call line when using calculateOrientation()
	// it'll already have gotten the image data for this frame
	stream.read(camera_frame);	
	unsigned char* data = (unsigned char*) camera_frame.ptr();
	return data;
}
