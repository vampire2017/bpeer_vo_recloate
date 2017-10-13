//
// Created by bpeer on 17-9-18.
//
#include <stdio.h>
#include <string>
#include "transport/TSocket.h"
#include "protocol/TBinaryProtocol.h"
#include "server/TSimpleServer.h"
#include "transport/TServerSocket.h"
#include "transport/TBufferTransports.h"
#include "test_types.h"
#include "TestService.h"

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;
using boost::shared_ptr;

int main(int argc, char** argv)
{
	///img
	cv::VideoCapture video_cap;
	video_cap.open(0);
	cv::Mat image;

	///odom
	double x = 1.1, y= 1.2, th = 0.3;

	///thrift
	shared_ptr<TTransport> socket(new TSocket("localhost", 9060));
	shared_ptr<TTransport> transport(new TBufferedTransport(socket));
	shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
	TestServiceClient client(protocol);
	std::string test1 = "---tst ok---";

	try
	{
		transport->open();
while(1){

		///img begin
		video_cap >> image;
		cv::resize(image, image, cv::Size(640, 480));

		std::vector<uchar> buf_img;
		cv::imencode( ".jpg", image, buf_img);
		std::string str_encode(buf_img.begin(), buf_img.end());
		///img end

		///odom begin
		std::string odom = "odom_ok";
		///odom end
		client.test( str_encode, x, y, th );

}

		transport->close();
	}
	catch(TException& tx)
	{
		printf("ERROR:%s\n",tx.what());
	}
}

