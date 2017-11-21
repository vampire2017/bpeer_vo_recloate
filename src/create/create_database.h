//
// Created by bpeer on 17-9-14.
//

#ifndef CREATE_DATABASE_CREATE_DATABASE_H
#define CREATE_DATABASE_CREATE_DATABASE_H

#include <iostream>
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <map>
#include <DBoW3/DBoW3.h>
#include "../common/timer.h"

using namespace cv;

struct pose2D
{
	float x;
	float y;
	float th;
	int stamp;
};

class SaveData
{
public:
	SaveData(){ };
//	SaveData( DBoW3::Database db_ );
	SaveData( const int i );

	DBoW3::Database db;
	void save_file( const std::string &filename ) const;
	void saveCb( Mat& image, pose2D odom );

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

	std::map< int, pose2D > map_pose;  //存储图像序列已经对应的坐标

	pose2D map2odom_;

};

#endif //CREATE_DATABASE_CREATE_DATABASE_H
