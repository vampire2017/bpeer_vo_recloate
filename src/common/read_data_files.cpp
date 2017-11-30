/*
 * brief: come on
 * author: Created by bpeer on 17-11-29.
 */
#include "read_data_files.h"

bool stringCompare( std::string s1, std::string s2 )
{
	if( s1.size() < s2.size() )
		return 1;
	else if( s1.size() > s2.size() )
		return 0;
	else
	{
		if( s1 < s2 )
			return 1;
		else
			return 0;
	}
}

ReadVideoData::ReadVideoData(const int i)
{}

std::vector< std::string > ReadVideoData::getFiles( std::string cate_dir )
{
	std::vector< std::string > files;
	DIR *pDir;
	struct dirent *ptr;
	char base[1000];

	if( (pDir = opendir(cate_dir.c_str())) == NULL )
	{
		perror("open dir error..  ");
		exit(1);
	}

	while( (ptr = readdir(pDir)) != NULL )
	{
		if ( strcmp(ptr->d_name, ".") == 0 || strcmp(ptr->d_name, "..") == 0 )
			continue;
		else if(ptr->d_type == 8)   ///file
			files.push_back(ptr->d_name);
		else if(ptr->d_type == 10)   //file
			continue;
		else if(ptr->d_type == 4)  ///dir
		{
			files.push_back(ptr->d_name);
		}
	}
	closedir(pDir);

	std::sort( files.begin(), files.end(), stringCompare );
	return files;
}

/**
 * @brief : 输入路径,以及文件名 => 获取 img 以及相对应的时间戳;
 * @param video_path_
 * @param video_name_
 */
void ReadVideoData::getImgAndTimeFormVideo(const std::string video_path_, const std::string video_name_)
{
	// ie: s_666_4_1506504125_60.ts
	m_video_detect_name = 0;
	m_video_begin_stamp.clear();
	m_video_sum_time.clear();
	m_num_frame = 0;
	mm_video_data.clear();

	// 获取视频名字中的 起始时间以及视频时长
	char c;
	for ( auto s_iter:video_name_ )
	{
		c = s_iter;
		if ( c == '_' || c == '.' )
			m_video_detect_name++;
		if ( 3 == m_video_detect_name && c != '_' )
			m_video_begin_stamp += c;
		if ( 4 == m_video_detect_name && c != '.' && c != '_' )
			m_video_sum_time += c;
	}
	std::cout << "video_start: " << m_video_begin_stamp << " s"<< std::endl;
	std::cout << "video_sum_time: " << m_video_sum_time << " s" << std::endl;

	cv::Mat frame;
	std::vector<cv::Mat> video_img;
	int get_interval = 10; // 每get_interval帧 取 1帧

	// 读取视频, 提取img并打上时间戳
	cv::VideoCapture capture( video_path_ );
	while ( capture.read( frame ) )
	{
		if ( 0 == m_num_frame % get_interval )
			video_img.push_back( frame.clone() );
		m_num_frame++;
	}

	double tmp_time = atoll( m_video_begin_stamp.c_str() );
	// interval: 1/30 => 1/(video_sum_stamp/num_frame)
	double time_interval = get_interval * m_num_frame * 1.0 / (1.0 * atoi( m_video_sum_time.c_str() ));
	for (int j = 0; j < video_img.size(); ++j)
	{
		tmp_time += j * time_interval;
		mm_video_data.insert( std::make_pair(tmp_time, video_img[j].clone()) );
	}

//	// TODO test
//	for ( auto tst_img:mm_video_data )
//	{
//		cv::imshow("test img", tst_img.second );
//		cv::waitKey(10);
//	}

	std::cout << "get nums of img from video:  " << video_img.size() << std::endl;
	capture.release();
}