//
// Created by bpeer on 2017年09月28日.
//
#ifndef UPDATE_DATABASE_UPDATE_DATABASE_H
#define UPDATE_DATABASE_UPDATE_DATABASE_H

#include <string>
#include <chrono>
#include <fstream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <map>
#include <DBoW3/DBoW3.h>
#include "timer.h"

using namespace cv;

struct pose2D
{
	float x;
	float y;
	float th;
	int stamp;
};

class UpdateData
{
public:
	UpdateData(){};
	UpdateData( const int i );
	DBoW3::Database db;

	void updateCb( Mat& image, pose2D odom );
	void load_file( const std::string &filename );
	void save_file( const std::string &filename ) const;

public:
	void saveImage( const Mat& image );

	Ptr< Feature2D > detector_;
	std::vector< KeyPoint > keypoints_;
	Mat descriptor_;
	Mat img;

	int index;
	int idImage;
	pose2D locate;

	float motion_value;
	float motion_thre;

	std::map<int, pose2D> map_pose_lib;
	std::map<int, pose2D> map_pose_new;

	pose2D data_in;
};


#endif  //UPDATE_DATABASE_UPDATE_DATABASE_H