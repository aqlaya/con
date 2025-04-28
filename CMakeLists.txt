cmake_minimum_required( VERSION 3.16)

project( con VERSION 1.0 LANGUAGES CXX C)

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

