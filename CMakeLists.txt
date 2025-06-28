cmake_minimum_required( VERSION 3.16)

project( con VERSION 1.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD  11)

set(BOOST_PATH ${CMAKE_SOURCE_DIR}/lib/boost_1_86_0)
set( OPUS_PATH ${CMAKE_SOURCE_DIR}/lib/opus/include )
set( CMAKE_PATH_MODULE ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

if ( EXISTS ${CMAKE_PATH_MODULE}/client.cmake ) 
    include(cmake/client.cmake)
else()
    message( WARNING "cmake/client.cmake not found")
endif()

if ( EXISTS ${CMAKE_PATH_MODULE}/server.cmake ) 
    include(cmake/server.cmake)
else()
    message( WARNING "cmake/client.cmake not found")
endif()


add_subdirectory(client)
add_subdirectory(server)


