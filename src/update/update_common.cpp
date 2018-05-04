/*
 * brief: come on
 * author: Created by bpeer on 17-11-22.
 */

#include "update_common.h"

UpdateCommon::UpdateCommon( char** argv ):
mclientHbaseOperate(1), mupdateData(1), mreadVideoData(1)
{
	host = "192.168.1.6";
	port = 9090;

	sprintf( db_path, "/home/bpeer/catkin_ws/src/bpeer_sj/database/%s_database.db", argv[1] );
	sprintf( img_pose_db_path, "/home/bpeer/catkin_ws/src/bpeer_sj/database/%s_id_locate.txt", argv[1] );

	// video path
	sprintf( mcVideo_path, "/home/bpeer/catkin_ws/src/bpeer_sj/database/%s/video", argv[1]);
	files_video_name = mreadVideoData.getFiles( mcVideo_path );

	mtable_name = "Q_pose";

	mtGet.__set_row( "Q_1" );
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
	for (int j = 0; j < files_video_name.size(); ++j)
	{
		std::string video_path = mcVideo_path;
		video_path = video_path + "/" + files_video_name[j];
		std::cout << "video path: " << video_path << std::endl;
		std::cout << "video num => " << j + 1 << std::endl;
		boost::timer t_1;
		mreadVideoData.getImgAndTimeFormVideo( video_path, files_video_name[j] );
		mv_video_datas.push_back( mreadVideoData.mm_video_data );

		std::cout << "num " << j+1 << " using time=> " << t_1.elapsed() << std::endl;
	}

//	// TODO test
//	for ( auto n:mv_video_datas ) {
//		for ( auto tst_img:n ) {
//			cv::imshow("test img", tst_img.second );
//			cv::waitKey(10);
//		}
//		std::cout << "-- " << std::endl;
//		cvWaitKey(0);
//	}
//	std::cout << "test ok " << std::endl <<std::endl;

	/**
	 * @brief deal with data
	 */
	pose2D map_pose;
	cv::Mat curr_image;
	std::string pose_json; // json {"x":1, "y":2}
	TTimeRange stampRange;

	for ( auto n : mv_video_datas )
	{
		for ( auto iter_img : n )
		{
			/// iter_img.second=>img; iter_img.first=>time(10位); (int64_t)(iter_img.first * 1000)=>hbase的时间戳是13位;
			stampRange.minStamp = (int64_t)(iter_img.first * 1000) - 10;
			stampRange.maxStamp = (int64_t)(iter_img.first * 1000) + 10;
			pose_json = mclientHbaseOperate.getData( mtResult, mtable_name, mtGet );

			if ( pose_json.empty() )
				continue;
			else
				pose_st = (struct Pose_st*) pose_json.c_str();

			/// get need data for pose_json
			// TODO here need to add decide the status of arm
			map_pose.x = pose_st->x;
			map_pose.y = pose_st->y;
			map_pose.th = pose_st->Rad;
			map_pose.stamp = pose_st->time.seces / (1000 * 86400LL); //day
			curr_image = iter_img.second.clone();
			mupdateData.updateCb( curr_image, map_pose );
		}
	}

	/**
	 * @brief save db
	 */
	mupdateData.db.save( db_path );
	std::cout << "db save in: " << db_path << std::endl;

	mupdateData.save_file( img_pose_db_path );
	std::cout << "img_pose_db save in: " << img_pose_db_path << std::endl;

	mclientHbaseOperate.disconnect();
}