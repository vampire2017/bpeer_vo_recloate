//
// Created by bpeer on 17-9-14.
//
#include "../create/create_database.h"
#include <dirent.h>
#include "../common/timer.h"

#include "CreateDbService.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>

#include <chrono>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using boost::shared_ptr;

class CreateDbServiceHandler : virtual public CreateDbServiceIf{
public:
	CreateDbServiceHandler():msaveData(1) {
		// Your initialization goes here
		std::chrono::steady_clock::time_point t100 = std::chrono::steady_clock::now();

		DBoW3::Vocabulary vocab("/home/bpeer/catkin_ws/src/bpeer_sj/database/vo_db_data/vocabulary_test.bin");

		std::chrono::steady_clock::time_point t101 = std::chrono::steady_clock::now();
		std::chrono::duration<double > time_used = std::chrono::duration_cast< std::chrono::duration<double > >( t101-t100 );
		std::cout << "加载 Voc 耗时：" << time_used.count() << std::endl;

		if ( vocab.empty() )
		{
			std::cerr << "Vocabulary does not exist. " << std::endl;
			return;
		}
		std::cout << "Vocabulary loaded !! " << std::endl;

		DBoW3::Database db_t( vocab, false, 0 );
		msaveData.db = db_t;
	}

	void createDb(const std::string& img_, const Data& data)
	{
		// Your implementation goes here
		std::vector<uchar> bytesImg( img_.begin(),img_.end());
		cv::Mat curr_image = cv::imdecode( bytesImg, CV_LOAD_IMAGE_COLOR );

		//@todo here need add time in...*****
		///input  map2odom
		msaveData.map2odom_.x = (float) data.x;
		msaveData.map2odom_.y = (float) data.y;
		msaveData.map2odom_.th = (float) data.th;
		msaveData.map2odom_.stamp = data.stamp;
		msaveData.saveCb( curr_image, msaveData.map2odom_ );

		std::cout << "callback  out..  " << std::endl;

		cv::imshow("img_decode", curr_image);
		test_save_data = cv::waitKey(1);
		printf("test-- odom: \n");
		std::cout << "odom_x:" << data.x
		          << " odom_y:" << data.y
		          << " odom_th:" << data.th << std::endl;

		//@todo  test save output
		if(test_save_data == 27)
		{
			msaveData.db.save("/home/bpeer/catkin_ws/src/bpeer_sj/database/database_test_2017.db");  //DBoW数据库
			msaveData.save_file("/home/bpeer/catkin_ws/src/bpeer_sj/database/test_id_locate_2017.txt");  //图片+位姿 数据库
//			while(1);
		}
		///output end
	}

public:
	SaveData msaveData;
	char test_save_data;

};

int main( int argc, char** argv )
{
	//@todo here need todo sth  -->  input img & odom;
	///thrift begin
	int port = 10087;
	shared_ptr<CreateDbServiceHandler> handler(new CreateDbServiceHandler());
	shared_ptr<TProcessor> processor(new CreateDbServiceProcessor(handler));
	shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
	shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
	shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

	TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
	server.serve();
	///thrift end

	/************************************************/
//	saveData.db.save("/home/bpeer/catkin_ws/src/bpeer_sj/database/database_test.db");  //DBoW数据库
//	saveData.save_file("/home/bpeer/catkin_ws/src/bpeer_sj/database/test_id_locate.txt");  //图片+位姿 数据库

	return 0;
}
