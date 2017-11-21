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

#include "HbaseOperate.h"

ClientHbaseOperate::ClientHbaseOperate( const int i ):
socket( (TSocket*)NULL ), transport((TBufferedTransport*)NULL), protocol((TBinaryProtocol*)NULL),
mnHbaseServicePort_(9090), mbIsConnected_(false)
{}


ClientHbaseOperate::~ClientHbaseOperate()
{
	if ( mbIsConnected_ )
		disconnect();

	if ( NULL != mpClient_ )
	{
		delete mpClient_;
		mpClient_ = NULL;
	}
}

/**
 * Connect Hbase.
 */
bool ClientHbaseOperate::connect()
{
	if ( mbIsConnected_ )
	{
		std::cout << "Already connected, don't need to connect it again.. " << std::endl;
		return true;
	}

	try
	{
		socket.reset( new TSocket(msHbaseServiceHost_, mnHbaseServicePort_) );
		transport.reset( new TBufferedTransport(socket) );
		protocol.reset( new TBinaryProtocol(transport) );

		mpClient_ = new HbaseClient(protocol);
		transport->open();
	}
	catch (const TException &tx)
	{
		std::cerr << "Connect Hbase error : " << tx.what() << std::endl;
		return false;
	}

	mbIsConnected_ = true;
	return mbIsConnected_;
}

/**
 * Connect Hbase.
 * @param host
 * @param port
 */
bool ClientHbaseOperate::connect(std::string host_, int port_)
{
	msHbaseServiceHost_ = host_;
	mnHbaseServicePort_ = port_;

	return connect();
}

bool ClientHbaseOperate::disconnect()
{
	if (!mbIsConnected_)
	{
		std::cout << "Haven't connected to Hbase yet, can't disconnect from it" << std::endl;
		return false;
	}

	if ( NULL != transport )
	{
		try
		{
			transport->close();
		}
		catch (const TException &tx)
		{
			std::cout << "Disconnect Hbase error : " << tx.what() << std::endl;
			return false;
		}
	}
	else
	{
		return false;
	}
	mbIsConnected_ = false;
	return true;
}

bool ClientHbaseOperate::getData(std::vector<TRowResult> &rowResult_, const Text &table_name_,
                                 const std::vector<Text> &rowKeys_, const std::map<Text, Text> &columnName_)
{
	mpClient_->getRows( rowResult_, table_name_, rowKeys_, columnName_ );
	printRow( rowResult_ );
	std::cout << "-------------------" << std::endl;
}