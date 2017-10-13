//
// Created by bpeer on 17-9-14.
//

#ifndef VO_RELOCATE_VO_RELOCATE_H
#define VO_RELOCATE_VO_RELOCATE_H

#include <iostream>
#include <string>
#include <map>
#include <sys/time.h>
#include <chrono>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <DBoW3/DBoW3.h>

using namespace cv;

struct pose2D
{
	float x;
	float y;
	float th;
};

struct result_reloc
{
	pose2D odo_locate;
	std::map<int, pose2D> map_result;
};

class VoRelocate
{
public:
	VoRelocate(){};
	VoRelocate(const int i);
	void load_file( const std::string &filename );
	void locateCb( Mat& image, pose2D odom );
	pose2D baselink2odom_;

	std::string result_;
private:
	bool relocate();

	DBoW3::Database db;
	std::map<int, pose2D> map_pose;  //robot pose in img for reloc

	int index;
	pose2D locate;
	pose2D ref_locate;

	float common_threa;
	float motion_thre;
	bool new_flag;
	bool relocate_success;
	float err_linear;
	float err_angular;

	Mat img;
	Mat ref_img;

	result_reloc result;
	std::vector< result_reloc > results;

	pose2D imageLocate;  //cur pos2D 对应的重定位中筛选图像所带的位姿


	int test_num;
};


#endif //VO_RELOCATE_VO_RELOCATE_H
