/*
 * brief: come on
 * author: Created by bpeer on 17-11-30.
 */

#ifndef VO_RELOCATE_READ_DATA_FILES_H
#define VO_RELOCATE_READ_DATA_FILES_H

#include <iostream>
#include <dirent.h>
#include <vector>
#include <cstring>
#include <algorithm>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <boost/timer.hpp>

class ReadVideoData
{
public:
	ReadVideoData(){};
	ReadVideoData(const int i);

	std::vector< std::string > getFiles( std::string cate_dir );
	void getImgAndTimeFormVideo( const std::string video_path_, const std::string video_name_ );

	std::map<double, cv::Mat> mm_video_data;

private:
	std::string m_video_begin_stamp;
	std::string m_video_sum_time;
	int m_video_detect_name;

	int m_num_frame;

};


#endif //VO_RELOCATE_READ_DATA_FILES_H
