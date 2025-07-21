

set(CMAKE_INCLUDE_GUI OFF)
set(CMAKE_INCLUDE_OPUS ON)

set(INCLUDE_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(PATH_TEST ${CMAKE_CURRENT_SOURCE_DIR}/test)



#tests (Google test)
include( test/CMakeLists.txt)

#boost 
if (EXISTS ${BOOST_PATH}) 
    include_directories(${BOOST_PATH})
else()
    message(FATAL_ERROR "boost not found")
endif()

#qt project
if ( CMAKE_INCLUDE_GUI )
    find_package(Qt5 COMPONENTS REQUIRED Widgets Quick)
    qt5_add_resources(AppResources App.qrc)
    file(GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
    add_executable(${PROJECT_NAME} ${SOURCES})

    target_link_libraries(${PROJECT_NAME} Qt5::Widgets Qt5::Quick)
    target_sources(${PROJECT_NAME} PRIVATE ${AppResources})
endif()

#libopus
if ( CMAKE_INCLUDE_OPUS )
    file(GLOB SOURCES_OPUS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc")
    add_executable(voice_record ${INCLUDE_SOURCE}/main.cc)
    target_compile_definitions(voice_record PUBLIC PULSE_AUDIO_LOG)
    target_link_libraries(voice_record PRIVATE pulse)
    target_include_directories(voice_record PUBLIC ${INCLUDE_PATH})    

endif()



 