cmake_minimum_required( VERSION 2.8.3 )
project( create_database )

set( CMAKE_CXX_FLAGS "-std=c++11" )

find_package( OpenCV 3.2.0 REQUIRED )

set( DBoW3_INCLUDE_DIRS "/usr/local/include" )
set( DBoW3_LIBS "/usr/local/lib/libDBoW3.so" )

include_directories(/usr/local/include/thrift)
set( THRIFT_LIBS "/usr/local/lib/libthrift-0.10.0.so" )

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ./../bin )

###########---create_database---#############
#add_executable( create_database
#        src/create_database_main.cpp
#        src/create_database.cpp
#        src/cJSON.c
#
#        src/test_constants.cpp
#        src/TestService.cpp
#        src/test_types.cpp
#        )
#
#target_link_libraries( create_database
#        ${OpenCV_LIBS}
#        ${DBoW3_LIBS}
#        ${THRIFT_LIBS}
#        )
##############################################

###########---ve_relocate---#############
add_executable( vo_relocate
        src/vo_relocate_main.cpp
        src/vo_relocate.cpp
        src/cJSON.c

        src/withReturn_constants.cpp
        src/WithReturnService.cpp
        src/withReturn_types.cpp
        )

target_link_libraries( vo_relocate
        ${OpenCV_LIBS}
        ${DBoW3_LIBS}
        ${THRIFT_LIBS}
        )
########################################


###########---update_database---#############
#add_executable( update_database
#        src/update_database_main.cpp
#        src/update_database.cpp
#        src/cJSON.c
#
#        src/updateDatabase_constants.cpp
#        src/UpdateService.cpp
#        src/updateDatabase_types.cpp
#        )
#
#target_link_libraries( update_database
#        ${OpenCV_LIBS}
#        ${DBoW3_LIBS}
#        ${THRIFT_LIBS}
#        )
##############################################

#install( TARGETS create_database RUNTIME DESTINATION bin )