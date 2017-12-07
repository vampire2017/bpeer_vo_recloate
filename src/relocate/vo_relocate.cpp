#include "vo_relocate.h"

VoRelocate::VoRelocate( const int i )
{
//	std::chrono::steady_clock::time_point t100 = std::chrono::steady_clock::now();
//	db.load( "/home/bpeer/catkin_ws/src/bpeer_sj/database/database_test.db" );
//
//	std::chrono::steady_clock::time_point t101 = std::chrono::steady_clock::now();
//	std::chrono::duration<double > time_used = std::chrono::duration_cast< std::chrono::duration<double > >( t101-t100 );
//	std::cout << "加载数据库耗时：" << time_used.count() << std::endl;

	index = 0;
	common_threa = 0;
	motion_thre = 0.02;  //@todo   test value = -0.02; right value :0.02
	new_flag = 0;
	relocate_success =0;
	err_linear = 0.2;
	err_angular = 0.1;

	test_num = 1;  //test

	img = cv::Mat();
	ref_img = cv::Mat();

}

void VoRelocate::reset()
{
	result_.clear();
	map_pose.clear();
	results.clear();
	result.map_result.clear();

//	todo VoRelocate(1);
	index = 0;
	common_threa = 0;
	motion_thre = 0.02;  //@todo   test value = -0.02; right value :0.02
	new_flag = 0;
	relocate_success =0;
	err_linear = 0.2;
	err_angular = 0.1;

	test_num = 1;  //test

	img = cv::Mat();
	ref_img = cv::Mat();

	std::cout << "size of db: " << db.size() << std::endl;
	if ( db.size() )
		db.clear();
}

void VoRelocate::locateCb(Mat &image, pose2D odom)
{
	bool re_flag;
	test_num++;  //test
	img = image.clone();
	locate = odom;
	if( new_flag == 0 )
	{
		std::cout << std::endl << "保存第一帧图像" << std::endl << std::endl;
		new_flag = 1;
		ref_img = img.clone();
		ref_locate = locate;
	}
	else
	{
		float temp_motion = sqrt( (ref_locate.x - locate.x) * (ref_locate.x - locate.x)
		                        + (ref_locate.y - locate.y) * (ref_locate.y - locate.y)
						+ 10 * (ref_locate.th - locate.th) * (ref_locate.th - locate.th));
		//判断是否为关键帧
		if(temp_motion > motion_thre )
		{
			std::cout << test_num << std::endl;  //test
			std::cout << "当前帧是关键帧." << std::endl;
			//根据数据集进行匹配
			result.map_result.clear();
			re_flag = relocate();
			ref_img = img.clone();
			ref_locate = locate;

			std::cout << "re_flag : " << re_flag << std::endl;
			//匹配成功
			if(re_flag)
			{
				std::cout << "成功定位当前帧到数据库...  " << std::endl;
				result.odo_locate = locate;
				if ( results.empty() )
				{
					std::cout << "第一次定位成功...  " << std::endl;
					results.push_back( result );
				}
				else
					for(auto i =results.begin(); i != results.end() && relocate_success == 0; ++i )
					{
						std::cout << "待验证的候选结果的次数:  " << results.size() << std::endl;
						float odo_linear, odo_angular;
						float map_linear, map_angular;

						odo_linear = sqrt( (result.odo_locate.x - i->odo_locate.x) * (result.odo_locate.x - i->odo_locate.x)
						                 + (result.odo_locate.y - i->odo_locate.y) * (result.odo_locate.y - i->odo_locate.y) );
						odo_angular = (result.odo_locate.th - i->odo_locate.th) * (result.odo_locate.th - i->odo_locate.th);

						//k -> cur frame result；  map_result 匹配到的图像序列
						//i -> last frame results：  j -> map_result 上面几帧对应的图像序列
						///最终使用k和j的匹配-->即 cur的匹配img  与  上几帧的匹配的img  进行比较
						for (auto k = result.map_result.begin(); k != result.map_result.end() && relocate_success == 0; ++k)
							for ( auto j = i->map_result.begin(); j != i->map_result.end() && relocate_success == 0; ++j )
							{
								map_linear = sqrt( (k->second.x - j->second.x) * (k->second.x - j->second.x)
								                   + (k->second.y - j->second.y) * (k->second.y - j->second.y) );
								map_angular = (k->second.th - j->second.th) * (k->second.th - j->second.th);

								if ( fabs(odo_linear - map_linear) < err_linear && fabs(odo_angular - map_angular) < err_angular )
								{
									imageLocate = k->second;  //cur pos2D in Database
									std::cout << "重定位成功： "<<"x: "<<imageLocate.x << " y: "<<imageLocate.y << " th: "<<imageLocate.th<<std::endl;
									result_ = "{\"x\" : "
											+ std::to_string(imageLocate.x)
											+ ",\"y\" : "
											+ std::to_string(imageLocate.y)
											+ ",\"th\" : "
											+ std::to_string(imageLocate.th)
									        + ",\"odomX\" : "
									        + std::to_string(odom.x)
									        + ",\"odomY\" : "
									        + std::to_string(odom.y)
									        + ",\"odomTh\" : "
									        + std::to_string(odom.th)
											+ "}";

									//@todo  here need to do sth   -->  output;
									relocate_success = 1;
								}   ///end if
							}   /// end for
					}   /// end else --> for
				if( !relocate_success )
				{
					results.push_back( result );
				}
				else
				{
					results.clear();
					relocate_success = 0;
				}
			}   ///end if
		}   ///end if
	}

	std::cout <<"*************"<<std::endl;
}

bool VoRelocate::relocate()
{
	common_threa = 0;
	bool relocate_flag = false;
	//detect ORB features
	std::cout << "detecting ORB features ... " << std::endl;
	Ptr< Feature2D > detector = ORB::create();
	Mat ref_descriptor;  //参考帧  last
	Mat vdescriptor;     //用于定位的图像  cur

	std::vector< KeyPoint > ref_keypoints;
	std::vector< KeyPoint > vkeypoints;
	detector->detectAndCompute( ref_img, Mat(), ref_keypoints, ref_descriptor );
	detector->detectAndCompute( img, Mat(), vkeypoints, vdescriptor );

	std::cout<<"comparing image with images...  "<<std::endl;

	DBoW3::BowVector v_image;
	db.getVoc_change()->transform( vdescriptor, v_image );

	DBoW3::BowVector v_ref;
	db.getVoc_change()->transform( ref_descriptor, v_ref );

	// s(vt, vt-δt)=>当前帧与前一帧的相似度=>先验相似度
	double score = db.getVoc_change()->score( v_image, v_ref );
	std::cout << "分值为：  " << score << std::endl;

	if( score > common_threa )
		common_threa = score;
	common_threa *= 0.3;   //common_threa *= 0.4;
	if( common_threa < 0.001 )
		common_threa = 0.001;

	if ( fabs(score) < 1e-6 )
	{
		common_threa = 10;  //kill socore = 0;
		score = 1;
	}
	std::cout << "阈值为：  " << common_threa << std::endl;

	//图片的比较
	DBoW3::QueryResults ret;
//  // here test ok
//	std::cout << "!!!!!test!!!!!! =>  " << db.size() << std::endl;
	db.query( vdescriptor, ret, 6 );
	std::cout << "searching for image returns " << ret << std::endl;


	for(auto i:ret)
	{
		if( i.Score > common_threa || i.Score > 3 * score )  //考虑大于3*score
		{
			result.map_result[i.Id] = map_pose[i.Id];  //pose
			relocate_flag = true;
			std::cout << "ratio:  " << i.Score / score << std::endl;
		}
	}

	std::cout << std::endl << std::endl;
	return relocate_flag;
}

void VoRelocate::load_file(const std::string &filename)
{
	map_pose.clear();
	int nNum;

	FileStorage fs( filename.c_str(), FileStorage::READ );
	if( !fs.isOpened() )
		throw std::string("Could not open file ") + filename;

	std::string name = "IdImageLocate";
	FileNode fdb = fs[name];
	nNum = (int) fdb["nNum"];

	FileNode fn = fdb["idLocate"];
	for (int i = 0; i != nNum; ++i)
	{
		FileNode fnn = fn[i];
		index = (int) fnn["IdImage"];
		FileNode fw = fnn["locate"];
		locate.x = (float) fw["x"];
		locate.y = (float) fw["y"];
		locate.th = (float) fw["th"];
		map_pose.insert( std::make_pair(index, locate) );
	}
	std::cout << "读入数据的数量为 :  " << map_pose.size() << std::endl;
}