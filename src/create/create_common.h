/*
 * brief: come on
 * author: Created by bpeer on 17-11-21.
 */

#ifndef CREATE_DATABASE_CREATE_COMMON_H
#define CREATE_DATABASE_CREATE_COMMON_H

#include "../common/HbaseOperate.h"
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
	ClientHbaseOperate mclientHbaseOperate;
	SaveData msaveData;

	std::vector<TRowResult> rowResult;
	Text table_name;
	std::vector<Text> rowKeys;
	std::map<Text, Text> columnName;

private:
	Text host;
	int port;

	char vocab_path[128];
	char db_path[128];
	char img_pose_db_path[128];



};


#endif //CREATE_DATABASE_CREATE_COMMON_H
