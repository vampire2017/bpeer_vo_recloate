/*
 * brief: come on
 * author: Created by bpeer on 17-11-21.
 */

#ifndef CREATE_DATABASE_CREATE_COMMON_H
#define CREATE_DATABASE_CREATE_COMMON_H

#include "../common/HbaseOperate.h"
#include "../common/read_data_files.h"
#include "create_database.h"
#include "../common/cJSON.h"
#include <chrono>

class CreateCommon
{
public:
	CreateCommon( char** argv );

	bool init();

	void process();

public:
	struct Time_st{
		uint32_t seces;
		uint32_t nseces;
	};
	struct Pose_st{
		struct Time_st time;
		float x;
		float y;
		float Deg;
		float Rad;
	} *pose_st ;

public:
	ClientHbaseOperate mclientHbaseOperate;
	SaveData msaveData;
	ReadVideoData mreadVideoData;

	std::vector<TRowResult> mvRowResult;
	Text msTable_name;
	Text msRowKey;
	std::vector<Text> mvColumns;
	std::map<Text, Text> mmColumnName;

private:
	Text host;
	int port;

	std::string vocab_path;
	char db_path[128];
	char img_pose_db_path[128];

	// read video
	char mcVideo_path[128];
	std::vector< Text > files_video_name;
	std::vector< std::map<double, cv::Mat> > mv_video_datas; // 存储图片以及对应的时间戳;vector以视频个数划分
};


#endif //CREATE_DATABASE_CREATE_COMMON_H
