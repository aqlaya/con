


include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

FetchContent_MakeAvailable(googletest) 

enable_testing()


add_executable(
  test_module_context
  ${PATH_TEST}/context.cpp
)

target_include_directories(test_module_context PUBLIC ${INCLUDE_PATH} )

target_link_libraries(
  test_module_context PUBLIC
  GTest::gtest_main
  pulse
)

include(GoogleTest)
gtest_discover_tests(test_module_context)

