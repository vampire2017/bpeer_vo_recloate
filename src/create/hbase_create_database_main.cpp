/*
 * brief: come on
 * author: Created by bpeer on 17-11-15.
 */

#include "create_common.h"

int main( int argc, char** argv )
{
	if( argc !=2 )
	{
		std::cerr << "need robotID input!!!" << std::endl;
		return 1;
	}

	CreateCommon createCommon( argv );
	if ( !createCommon.init() )
	{
		std::cout << "init failed.. " << std::endl;
		return 1;
	}

	createCommon.process();

	return 0;
}