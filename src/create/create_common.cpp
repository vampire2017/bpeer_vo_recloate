/*
 * brief: come on
 * author: Created by bpeer on 17-11-21.
 */

#include "create_common.h"

CreateCommon::CreateCommon( char** argv ):
msaveData(1), mclientHbaseOperate(1)
{
	host = "q5";
	port = 9090;

	// robot id argv => vocab need to train
	vocab_path = "/home/bpeer/catkin_ws/src/bpeer_sj/database/bpeer_Vocabulary.bin";
	sprintf( db_path, "/home/bpeer/catkin_ws/src/bpeer_sj/database/%s_database.db", argv[1] );
	sprintf( img_pose_db_path, "/home/bpeer/catkin_ws/src/bpeer_sj/database/%s_id_locate.txt", argv[1] );

	table_name = "relocate_database";
	Text tmp_key = argv[1];
	// TODO get ‘member’,‘xiaofeng’,‘info:age’
	rowKeys.push_back( tmp_key );
	rowResult.clear();
	columnName.clear();
}

bool CreateCommon::init()
{
	/**
	 * @brief hbase init
 	 */
	bool open_flag = mclientHbaseOperate.connect( host, port );
	if ( !open_flag )
	{
		std::cout << "can not open port. " << std::endl;
		return false;
	}
	std::cout << "port open. " << std::endl;

	/**
	 * @brief create database init
	 */
	std::cout << "vocab_path: " << vocab_path << std::endl;
	DBoW3::Vocabulary vocab( vocab_path );
	if ( vocab.empty() )
	{
		std::cerr << "Vocabulary does not exist. " << std::endl;
		return false;
	}
	std::cout << "Vocabulary loaded !! " << std::endl;

	DBoW3::Database db_t( vocab, false, 0 );
	msaveData.db = db_t;

	return true;
}

void CreateCommon::process()
{
	mclientHbaseOperate.getData( rowResult, table_name, rowKeys, columnName );

	/**
	 * @brief deal with data
	 */
	pose2D map_pose;
	cv::Mat curr_image;
	/// 对rowResult进行操作;
	// TODO 需求Hbase的时间戳
	msaveData.saveCb( curr_image, map_pose );
	/**
	 * @brief save db
	 */
	msaveData.db.save( db_path );
	std::cout << "db save in: " << db_path << std::endl;

	msaveData.save_file( img_pose_db_path );
	std::cout << "img_pose_db save in: " << img_pose_db_path << std::endl;

	mclientHbaseOperate.disconnect();
}

