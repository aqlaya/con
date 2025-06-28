#include <gtest/gtest.h>

#include <pulseaudio/context.hpp>
#include <pulseaudio/mainloop.hpp>

class MainloopGeneral: public ::testing::Test {
protected:
  void SetUp() override { 
  }
  void TearDown() override {
  }

  audio::mainloop event_loop;
};

TEST_F( MainloopGeneral, ContextContruct ) {  
  audio::context io( event_loop.get_api(), "Test Context"); 
}