// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.

#include "WithReturnService.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>

#include "vo_relocate.h"

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using boost::shared_ptr;

class WithReturnServiceHandler : virtual public WithReturnServiceIf {
public:
	WithReturnServiceHandler():voRelocate(1) {
		// Your initialization goes here
		voRelocate.load_file( "/home/bpeer/catkin_ws/src/bpeer_sj/database/test_id_locate.txt" );

	}

	void resultReturn(std::string& _return, const std::string& img_, const std::string& ID_, const Data& data)
	{
		// TODO need to decide robotID
		// Your implementation goes here
		std::vector<uchar> bytesImg( img_.begin(),img_.end());
		cv::Mat curr_image = cv::imdecode( bytesImg, CV_LOAD_IMAGE_COLOR );
		std::cout <<"********************start**********************"<<std::endl;

		///input  baselink2odom_
		voRelocate.baselink2odom_.x = data.x;
		voRelocate.baselink2odom_.y = data.y;
		voRelocate.baselink2odom_.th = data.th;
		voRelocate.locateCb( curr_image, voRelocate.baselink2odom_ );

		std::cout << "callback  out..  " << std::endl;

//		cv::imshow("img decode", curr_image);
//		cv::waitKey(1);
		printf("test-- odom: \n");
		std::cout << "odom_x:" << data.x
		          << " odom_y:" << data.y
		          << " odom_th:" << data.th << std::endl;

		_return = voRelocate.result_;  //return

		std::cout << "res....: " << voRelocate.result_ << std::endl;
		voRelocate.result_.clear();  //清空上次值
		std::cout <<"*********************end*******************"<<std::endl;
	}

public:
	VoRelocate voRelocate;
};

int main(int argc, char **argv) {
	int port = 9080;
	shared_ptr<WithReturnServiceHandler> handler(new WithReturnServiceHandler());
	shared_ptr<TProcessor> processor(new WithReturnServiceProcessor(handler));
	shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
	shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
	shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

	TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
	server.serve();
	return 0;
}

