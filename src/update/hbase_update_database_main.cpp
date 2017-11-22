/*
 * brief: come on
 * author: Created by bpeer on 17-11-22.
 */

#include "update_common.h"

int main( int argc, char** argv )
{
	if( argc !=2 )
	{
		std::cerr << "need robotID input!!!" << std::endl;
		return 1;
	}

	UpdateCommon updateCommon( argv );
	if ( !updateCommon.init() )
	{
		std::cout << "init failed.. " << std::endl;
		return 1;
	}

	updateCommon.process();

	return 0;
}