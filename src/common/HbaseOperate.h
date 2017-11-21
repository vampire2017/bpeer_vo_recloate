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

#include "Hbase.h"

using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

using namespace apache::hadoop::hbase::thrift;

namespace {

	typedef std::vector<std::string> StrVec;
	typedef std::map<std::string,std::string> StrMap;
	typedef std::vector<ColumnDescriptor> ColVec;
	typedef std::map<std::string,ColumnDescriptor> ColMap;
	typedef std::vector<TCell> CellVec;
	typedef std::map<std::string,TCell> CellMap;


	static void
	printRow(const std::vector<TRowResult> &rowResult)
	{
		std::cout << "Row size of row:=  " << rowResult.size() << std::endl;
		for (size_t i = 0; i < rowResult.size(); i++) {
			std::cout << "size of columns in row:= " << rowResult[i].columns.size() << std::endl;
			std::cout << "row: " << rowResult[i].row << ", cols: ";
			for (CellMap::const_iterator it = rowResult[i].columns.begin();
			     it != rowResult[i].columns.end(); ++it) {
				std::cout << it->first << " => " << it->second.value << "; ";
			}
			std::cout << std::endl;
		}
	}

	static void
	printCell(const std::vector<TCell> &cellResult)
	{
		std::cout << "Cell size of row:=  " << cellResult.size() << std::endl;
		for (size_t i = 0; i < cellResult.size(); i++) {
			std::cout << "cellResult.value:= "<< " => " << cellResult[i].value << "; ";
			std::cout << std::endl;
		}
	}
}

class ClientHbaseOperate
{
public:
	ClientHbaseOperate( const int i );
	~ClientHbaseOperate();

private:
	boost::shared_ptr<TTransport> socket;
	boost::shared_ptr<TTransport> transport;
	boost::shared_ptr<TProtocol> protocol;

	HbaseClient *mpClient_;

	std::string msHbaseServiceHost_;
	int mnHbaseServicePort_;
	bool mbIsConnected_;

public:
	bool connect();
	bool connect( std::string host_, int port_);
	bool disconnect();

	bool getData(std::vector<TRowResult> & rowResult_,
	             const Text& table_name_,
	             const std::vector<Text> & rowKeys_,
	             const std::map<Text, Text> & columnName_);

};

#endif //CREATE_DATABASE_HBASEOPERATE_H
