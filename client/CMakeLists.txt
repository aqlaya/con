cmake_minimum_required(VERSION 3.16)


project( client VERSION 1.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD  11)

set(BOOST_PATH ${CMAKE_SOURCE_DIR}/lib/boost_1_86_0)

if (EXISTS ${BOOST_PATH}) 
    include_directories(${BOOST_PATH})
else()
    message(FATAL_ERROR "boost not found")
endif()

