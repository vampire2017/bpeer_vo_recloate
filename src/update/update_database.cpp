//
// Created by bpeer on 2017年09月28日.
//
#include "update_database.h"

UpdateData::UpdateData( const int i )
{
	index = 0;
	motion_thre = 0.02;
	detector_ = ORB::create();
	idImage = 0;
	map_pose_new.clear();
}

void UpdateData::load_file(const std::string &filename)
{
	map_pose_lib.clear();
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
		locate.stamp = (int) fw["stamp"];
		locate.x = (float) fw["x"];
		locate.y = (float) fw["y"];
		locate.th = (float) fw["th"];
		map_pose_lib.insert( std::make_pair(index, locate) );
	}
	std::cout << "读入数据的数量为 :  " << map_pose_lib.size() << std::endl;

	index = 0; //@todo to save new data;;;
}

void UpdateData::updateCb(Mat &image, pose2D g_pose)
{
	std::cout << "update in.. " << std::endl;
	float tmp_motion = 0;
	motion_value = 999;

	locate = g_pose;

	//@todo 时间戳直接client传输过来的时候,此处可以不用
	locate.stamp = internalTimer::Timer::now().ToDAY();

	if( map_pose_new.empty() )
	{
		saveImage( image );
	}
	else
	{
		for( auto p:map_pose_new )
		{
			tmp_motion = sqrt( (p.second.x - locate.x) * (p.second.x - locate.x) +
			                   (p.second.y - locate.y) * (p.second.y - locate.y) +
			                   10 * (p.second.th - locate.th) * (p.second.th - locate.th) );
			if( tmp_motion < motion_value )
				motion_value = tmp_motion;
		}
		std::cout << "tmp_motion****   " << tmp_motion << std::endl;
		if( motion_value > motion_thre )
		{
			saveImage( image );
		}
	}

	std::cout << "新增数据的大小：" << map_pose_new.size() << std::endl;

}

void UpdateData::saveImage(const Mat &image)
{
	img.release();
	img = image.clone();
	keypoints_.clear();
	descriptor_.release();

	detector_->detectAndCompute( img, Mat(), keypoints_, descriptor_ );

	map_pose_new.insert( std::make_pair(index, locate) );
	index++;

	bool flag_update = false;

	float disHM = 0;
	int disTime = 0;

	db.updateDb( descriptor_, index );

	//@todo 位置数据库的限制与更新
	for( auto p:map_pose_lib )
	{
		disHM = fabsf(p.second.x - locate.x)  +  fabsf(p.second.y - locate.y)  +  3 * fabsf(p.second.th - locate.th);

		if ( disHM < 0.02 )
		{
			disTime = locate.stamp - p.second.stamp;

			if ( disTime > 15 )  //day
			{
				///update  db  old2new
				db.updateDb( descriptor_, p.first );

				///update  poseLib  old2new
				map_pose_lib[p.first] = locate;
			}
			else
			{
				//@todo
				//图片的比较
				DBoW3::QueryResults ret;
				db.query( descriptor_, ret, 1 );
				if( !ret.empty() )
				{
					if( ret.begin()->Score  != 1 )
					{

						if( ret.begin()->Id == p.first )
						{
							///update db  old2new
							db.updateDb( descriptor_, p.first );
						}
					}
				}
			}

			flag_update = true;
			break;   //因为建图的时候考虑到keyframe,,所以此处只需要检车到一个即可
		}
	}
	std::cout << "5.. " << std::endl;

	if (  map_pose_new.size() != 1 )
	{
		if ( !flag_update )
		{
			///update   add new
			db.add( descriptor_ );
			idImage = db.size() - 1;
			map_pose_lib.insert( std::make_pair(idImage, locate) );
		}
	}
	else
	{
		std::cout << "the first update data..." << std::endl;
	}
}

void UpdateData::save_file(const std::string &filename) const
{
	int nNum = map_pose_lib.size();

	FileStorage fs( filename.c_str(), FileStorage::WRITE );
	if( !fs.isOpened() )
		throw std::string("Could not open file ") + filename;

	std::string name = "IdImageLocate";
	fs << name << "{";
	fs << "nNum" << nNum;
	fs << "idLocate" << "[";

	for( auto it:map_pose_lib )
	{
		fs << "{";
		fs << "IdImage" << it.first;
		fs << "locate";
		fs << "{"
		   << "stamp" << it.second.stamp
		   << "x" << it.second.x
		   << "y" << it.second.y
		   << "th" << it.second.th
		   << "}";
		fs << "}";
	}

	fs << "]"; // invertedIndex
	fs << "}"; // database
	fs.release();
	std::cout << "更新后图片ID和位置数据保存到" << filename << std::endl;
}
