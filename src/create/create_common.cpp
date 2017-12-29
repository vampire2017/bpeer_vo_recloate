/*
 * brief: come on
 * author: Created by bpeer on 17-11-21.
 */

#include "create_common.h"

CreateCommon::CreateCommon( char** argv ):
mclientHbaseOperate(1), msaveData(1), mreadVideoData(1)
{
	host = "q5";
	port = 9090;

	// robot id argv => vocab need to train
	vocab_path = "/home/bpeer/catkin_ws/src/bpeer_sj/database/bpeer_Vocabulary.bin";
	sprintf( db_path, "/home/bpeer/catkin_ws/src/bpeer_sj/database/%s_database.db", argv[1] );
	sprintf( img_pose_db_path, "/home/bpeer/catkin_ws/src/bpeer_sj/database/%s_id_locate.txt", argv[1] );

//	 get video path
	sprintf( mcVideo_path, "/home/bpeer/catkin_ws/src/bpeer_sj/database/%s/video", argv[1]);
	files_video_name = mreadVideoData.getFiles( mcVideo_path );

	msTable_name = "pose";
	msRowKey = argv[1];  // "myhid"

	mvColumns.push_back("data");

	mvRowResult.clear();
	mmColumnName.clear();
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

	for ( auto n : mv_video_datas )
	{
		for ( auto iter_img : n )
		{
//			iter_img.second=>img; iter_img.first=>time(10位); (int64_t)(iter_img.first * 1000)=>hbase的时间戳是13位;
			pose_json = mclientHbaseOperate.getData( mvRowResult, msTable_name, msRowKey, mvColumns,
			                             (int64_t)(iter_img.first * 1000), mmColumnName);

			pose_st = (struct Pose_st*) pose_json.c_str();
//			std::cout << "** pose_st **" << std::endl;
//			std::cout << "x: " << pose_st->x << std::endl;
//			std::cout << "y: " << pose_st->y << std::endl;
//			std::cout << "th: " << pose_st->Rad << std::endl;
//			std::cout << "time: " << pose_st->time.seces << std::endl;
//			std::cout << "n_time: " << pose_st->time.nseces << std::endl;
//			std::cout << "** pose_st **" << std::endl;

			/// get need data for pose_json
			map_pose.x = pose_st->x;
			map_pose.y = pose_st->y;
			map_pose.th = pose_st->Rad;
			map_pose.stamp = pose_st->time.seces / (1000 * 86400LL); //day
			curr_image = iter_img.second.clone();
			msaveData.saveCb( curr_image, map_pose );
		}
	}

	/**
	 * @brief save db
	 */
	msaveData.db.save( db_path );
	std::cout << "db save in: " << db_path << std::endl;

	msaveData.save_file( img_pose_db_path );
	std::cout << "img_pose_db save in: " << img_pose_db_path << std::endl;

	mclientHbaseOperate.disconnect();
}

