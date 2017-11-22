/*
 * brief: come on
 * author: Created by bpeer on 17-11-22.
 */

#include "update_common.h"

UpdateCommon::UpdateCommon( char** argv ):
mclientHbaseOperate(1), mupdateData(1)
{
	host = "q5";
	port = 9090;

	sprintf( db_path, "/home/bpeer/catkin_ws/src/bpeer_sj/database/%s_database.db", argv[1] );
	sprintf( img_pose_db_path, "/home/bpeer/catkin_ws/src/bpeer_sj/database/%s_id_locate.txt", argv[1] );

	table_name = "relocate_database";
	Text tmp_key = argv[1];
	// TODO get ‘member’,‘xiaofeng’,‘info:age’
	rowKeys.push_back( tmp_key );
	rowResult.clear();
	columnName.clear();
}

bool UpdateCommon::init()
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
	 * @brief update database init
	 */
	mupdateData.db.load( db_path );
	std::cout << "db loaded !! " << std::endl;
	mupdateData.load_file( img_pose_db_path );
	std::cout << "img_pose_db loaded !! " << std::endl;

	return true;
}

void UpdateCommon::process()
{
	mclientHbaseOperate.getData( rowResult, table_name, rowKeys, columnName );

	/**
	 * @brief deal with data
	 */
	pose2D map_pose;
	cv::Mat curr_image;
	/// 对rowResult进行操作;

	mupdateData.updateCb( curr_image, map_pose );
	/**
	 * @brief save db
	 */
	mupdateData.db.save( db_path );
	std::cout << "db save in: " << db_path << std::endl;

	mupdateData.save_file( img_pose_db_path );
	std::cout << "img_pose_db save in: " << img_pose_db_path << std::endl;

	mclientHbaseOperate.disconnect();
}