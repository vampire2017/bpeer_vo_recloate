/*
 * brief: come on
 * author: Created by bpeer on 17-11-22.
 */

#ifndef VO_RELOCATE_UPDATE_COMMON_H
#define VO_RELOCATE_UPDATE_COMMON_H


#include "../common/HbaseOperate.h"
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

	std::vector<TRowResult> rowResult;
	Text table_name;
	std::vector<Text> rowKeys;
	std::map<Text, Text> columnName;

private:
	Text host;
	int port;

	char db_path[128];
	char img_pose_db_path[128];
};

#endif //VO_RELOCATE_UPDATE_COMMON_H
