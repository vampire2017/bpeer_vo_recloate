/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef CREATE_DATABASE_HBASEOPERATE_H
#define CREATE_DATABASE_HBASEOPERATE_H

#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <poll.h>

#include <iostream>

#include <boost/lexical_cast.hpp>
#include <protocol/TBinaryProtocol.h>
#include <transport/TSocket.h>
#include <transport/TTransportUtils.h>

//#include "Hbase.h"
#include "../common_thrift2/THBaseService.h"

using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

using namespace apache::hadoop::hbase::thrift2;

class ClientHbaseOperate
{
public:
	ClientHbaseOperate( const int i );
	~ClientHbaseOperate();

private:
	boost::shared_ptr<TTransport> socket;
	boost::shared_ptr<TTransport> transport;
	boost::shared_ptr<TProtocol> protocol;

	THBaseServiceClient *mpClient_;

	std::string msHbaseServiceHost_;
	int mnHbaseServicePort_;
	bool mbIsConnected_;

public:
	bool connect();
	bool connect( std::string host_, int port_);
	bool disconnect();

	// 返回 读取rowResult_中对应的value;
	std::string getData( TResult &rowResult_,
	             const std::string &table_name_,
	             TGet &tGet );

};

#endif //CREATE_DATABASE_HBASEOPERATE_H
