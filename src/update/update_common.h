/*
 * brief: come on
 * author: Created by bpeer on 17-11-22.
 */

#ifndef VO_RELOCATE_UPDATE_COMMON_H
#define VO_RELOCATE_UPDATE_COMMON_H


#include "../common/HbaseOperate.h"
#include "../common/read_data_files.h"
#include "update_database.h"
#include "../common/cJSON.h"
#include <chrono>

class UpdateCommon
{
public:
	UpdateCommon( char** argv );

	bool init();

	void process();

public:
	ClientHbaseOperate mclientHbaseOperate;
	UpdateData mupdateData;
	ReadVideoData mreadVideoData;

	std::vector<TRowResult> mvRowResult;
	Text msTable_name;
	Text msRowKey;
	std::vector<Text> mvColumns;
	std::map<Text, Text> mmColumnName;

private:
	Text host;
	int port;

	char db_path[128];
	char img_pose_db_path[128];

	// read video
	char mcVideo_path[128];
	std::vector< Text > files_video_name;
	std::vector< std::map<double, cv::Mat> > mv_video_datas; // 存储图片以及对应的时间戳;vector以视频个数划分

};

#endif //VO_RELOCATE_UPDATE_COMMON_H
