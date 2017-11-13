//
// Created by bpeer on 17-9-14.
//
#include "create_database.h"

//SaveData::SaveData(DBoW3::Database db_)
//{
//	index = 0;
//	motion_thre = 0.02;
//	detector_ = ORB::create();
//	idImage = 0;
////	db = db_;
//}

SaveData::SaveData(const int i)
{
	index = 0;
	motion_thre = 0.02;  //@todo -0.02  to test;  right value is 0.02
	detector_ = ORB::create();
	idImage = 0;
	std::cout << "init: " << i << std::endl;
}

void SaveData::saveCb(Mat &image, pose2D odom)
{
	std::cout << "image  in..  " << std::endl;
	float tmp_motion = 0;
	motion_value = 999;  //@todo 每次都重新赋值

//	internalTimer::Timer::now().ToDAY();

	locate = odom;
	locate.stamp = internalTimer::Timer::now().ToDAY();

	if( map_pose.empty() )
	{
		saveImage(image);
		index++;
	}
	else
	{
		for( auto p:map_pose )
		{
			tmp_motion = sqrt( (p.second.x - locate.x) * (p.second.x - locate.x) +
			                   (p.second.y - locate.y) * (p.second.y - locate.y) +
			                   10 * (p.second.th - locate.th) * (p.second.th - locate.th) );
			if( tmp_motion < motion_value )
				motion_value = tmp_motion;
//			std::cout << "-- motion_value :  " << motion_value << std::endl;
		}
		if( motion_value > motion_thre )
		{
			saveImage( image );
			index++;
		}
	}

	std::cout <<"数据集的大小："<< map_pose.size() <<std::endl;
}

void SaveData::saveImage(const Mat &image)
{
	std::cout << "save image  in..  " << std::endl;

	img.release();
	img = image.clone();
	keypoints_.clear();
	descriptor_.release();
//	imshow("1", img);
//	waitKey(1000);

	detector_->detectAndCompute( img, Mat(), keypoints_, descriptor_ );

	db.add( descriptor_ );

	idImage = db.size() - 1;
	map_pose.insert( std::make_pair(idImage, locate) );
	std::cout << "save image  out..  " << std::endl;

}

void SaveData::save_file(const std::string &filename) const
{
	std::cout << "saving file .txt ---------"  << std::endl;

	int nNum = map_pose.size();

	FileStorage fs( filename.c_str(), FileStorage::WRITE );///@todo FileStorage::APPEND，追加写
	if( !fs.isOpened() )
		throw std::string("Could not open file ") + filename;

	std::string name = "IdImageLocate";
	fs << name << "{";
	fs << "nNum" << nNum;
	fs << "idLocate" << "[";

	for( auto it:map_pose )
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
	std::cout << "图片ID和位置数据保存到" << filename << std::endl;
}