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

int main(int argc, char** argv)
{
	boost::shared_ptr<TTransport> socket(new TSocket("q5", 9090));
	boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
	boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
	HbaseClient client(protocol);

	const std::map<Text, Text>  dummyAttributes; // see HBASE-6806 HBASE-4658
	std::cout << "come here. " << std::endl;
	try
	{
		transport->open();
		std::cout << "port open. " << std::endl;

		std::vector<TRowResult> rowResult;
		std::string table_name("relocate_database");
		std::string rowKey("myhid");
		std::vector<std::string> rowKeys;
		std::map<std::string, std::string> columnName;
//		columnName.insert( std::make_pair("train", "bpittt") );
		std::string columns("pose");
		// TODO get ‘member’,‘xiaofeng’,‘info:age’
		std::vector<TCell> ro;
		rowKeys.push_back("myhid");
		client.getRows(rowResult, table_name, rowKeys, columnName );
		client.get( ro, table_name, rowKey, columns, columnName );

		printRow(rowResult);
		std::cout << "-------------------" << std::endl;
		printCell(ro);
//		std::cout << "value:{ " << std::endl
//		          << rowResult[0].columns.begin()->second << std::endl
//		          << "}" << std::endl;
		transport->close();
		std::cout << "port close. " << std::endl;
	}
	catch (const TException &tx)
	{
		std::cerr << "ERROR: " << tx.what() << std::endl;
	}
	return  0;
}
