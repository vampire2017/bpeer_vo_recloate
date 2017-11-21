//
// Created by bpeer on 17-9-11.
//
#include "update_database.h"
#include <dirent.h>
#include "cJSON.h"

#include "UpdateService.h"
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

struct Time
{
	uint32_t sec;
	uint32_t nsec;
};

class UpdateServiceHandler : virtual public UpdateServiceIf{
public:
	UpdateServiceHandler():mupdateData(1) {
		// Your initialization goes here
		std::cout << "in..  " << std::endl;
		std::chrono::steady_clock::time_point t100 = std::chrono::steady_clock::now();
		mupdateData.db.load( "/home/bpeer/catkin_ws/src/bpeer_sj/database/database_test_update.db" );
		//@todo 文件不存在时的检测

		//	if( db.size() )
		std::chrono::steady_clock::time_point t101 = std::chrono::steady_clock::now();
		std::chrono::duration<double > time_used = std::chrono::duration_cast< std::chrono::duration<double > >( t101-t100 );
		std::cout << "加载数据库耗时： " << time_used.count() << std::endl;
		std::cout << "加载的数据集大小 ： " << mupdateData.db.size() << std::endl;

		mupdateData.load_file( "/home/bpeer/catkin_ws/src/bpeer_sj/database/test_id_locate_update.txt" );

	}

	void update( const std::string& img_, const Pose& odom_data)
	{
		/// Your implementation goes here
		std::vector<uchar> bytesImg( img_.begin(),img_.end());
		cv::Mat curr_image = cv::imdecode( bytesImg, CV_LOAD_IMAGE_COLOR );

		///input odom + time
		mupdateData.data_in.x = odom_data.x;
		mupdateData.data_in.y = odom_data.y;
		mupdateData.data_in.th = odom_data.th;

		printf("test-- odom: \n");
		std::cout << "odom_x:" << odom_data << std::endl;

		mupdateData.updateCb( curr_image, mupdateData.data_in );

		cv::imshow("img_decode", curr_image);
		test_update_data = cv::waitKey(1);

		if(test_update_data == 27)
		{
			mupdateData.db.save("/home/bpeer/catkin_ws/src/bpeer_sj/database/database_test_update.db");  //DBoW数据库  update
			mupdateData.save_file("/home/bpeer/catkin_ws/src/bpeer_sj/database/test_id_locate_update.txt");  //图片+位姿 数据库 update
//			while(1);
		}

	}

public:
	UpdateData mupdateData;
	char test_update_data;

};

int main( int argc, char** argv )
{
	//@todo here need todo sth  -->  input img & odom;
	///thrift begin
	int port = 9040;
	shared_ptr<UpdateServiceHandler> handler(new UpdateServiceHandler());
	shared_ptr<TProcessor> processor(new UpdateServiceProcessor(handler));
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
