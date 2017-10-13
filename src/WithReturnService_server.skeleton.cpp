// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.

#include "WithReturnService.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using boost::shared_ptr;

class WithReturnServiceHandler : virtual public WithReturnServiceIf {
 public:
  WithReturnServiceHandler() {
    // Your initialization goes here
  }

  void resultReturn(std::string& _return, const std::string& img_, const double odom_x, const double odom_y, const double odom_th) {
    // Your implementation goes here
    printf("resultReturn\n");
  }

};

int main(int argc, char **argv) {
  int port = 9090;
  shared_ptr<WithReturnServiceHandler> handler(new WithReturnServiceHandler());
  shared_ptr<TProcessor> processor(new WithReturnServiceProcessor(handler));
  shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
  shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
  shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

  TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
  server.serve();
  return 0;
}

