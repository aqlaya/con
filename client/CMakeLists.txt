cmake_minimum_required(VERSION 3.16)


project( client VERSION 1.0 LANGUAGES CXX C)
set(PROJECT_NAME ConClient)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD  11)

set(BOOST_PATH ${CMAKE_SOURCE_DIR}/lib/boost_1_86_0)

if (EXISTS ${BOOST_PATH}) 
    include_directories(${BOOST_PATH})
else()
    message(FATAL_ERROR "boost not found")
endif()

find_package(Qt5 COMPONENTS REQUIRED Widgets Quick)

qt5_add_resources(AppResources App.qrc)

file(GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} Qt5::Widgets Qt5::Quick)
target_sources(${PROJECT_NAME} PRIVATE ${AppResources})