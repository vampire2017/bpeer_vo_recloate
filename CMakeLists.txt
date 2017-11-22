cmake_minimum_required( VERSION 2.8.3 )
project( vo_relocate )

set( CMAKE_CXX_FLAGS "-std=c++11" )

find_package( OpenCV 3.2.0 REQUIRED )

set( DBoW3_INCLUDE_DIRS "/usr/local/include" )
set( DBoW3_LIBS "/usr/local/lib/libDBoW3.so" )

include_directories(/usr/local/include/thrift)
set( THRIFT_LIBS "/usr/local/lib/libthrift-0.10.0.so" )

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ./../bin )

###########---create_database---#############
add_executable( create_database_hbase
        src/create/hbase_create_database_main.cpp
        src/create/create_database.cpp
        src/create/create_common.cpp

        src/common/HbaseOperate.cpp
        src/common/cJSON.c

        src/common/Hbase.cpp
        src/common/Hbase_constants.cpp
        src/common/Hbase_types.cpp
        )

target_link_libraries( create_database_hbase
        ${OpenCV_LIBS}
        ${DBoW3_LIBS}
        ${THRIFT_LIBS}
        )
##############################################

###########---ve_relocate---#############
add_executable( vo_relocate
        src/relocate/vo_relocate_main.cpp
        src/relocate/vo_relocate.cpp
        src/common/cJSON.c

        src/relocate/withReturn_constants.cpp
        src/relocate/WithReturnService.cpp
        src/relocate/withReturn_types.cpp
        )

target_link_libraries( vo_relocate
        ${OpenCV_LIBS}
        ${DBoW3_LIBS}
        ${THRIFT_LIBS}
        )
########################################


###########---update_database---#############
add_executable( update_database_hbase
        src/update/hbase_update_database_main.cpp
        src/update/update_database.cpp
        src/update/update_common.cpp

        src/common/HbaseOperate.cpp
        src/common/cJSON.c

        src/common/Hbase.cpp
        src/common/Hbase_constants.cpp
        src/common/Hbase_types.cpp
        )

target_link_libraries( update_database_hbase
        ${OpenCV_LIBS}
        ${DBoW3_LIBS}
        ${THRIFT_LIBS}
        )
##############################################

##################################
###########---test---#############
#add_executable( test_hbase
#        src/common/client_test_hbase.cpp
#
#        src/common/Hbase.cpp
#        src/common/Hbase_constants.cpp
#        src/common/Hbase_types.cpp
#        )
#
#target_link_libraries( test_hbase
#        ${OpenCV_LIBS}
#        ${DBoW3_LIBS}
#        ${THRIFT_LIBS}
#        )
########################################

#install( TARGETS create_database RUNTIME DESTINATION bin )
