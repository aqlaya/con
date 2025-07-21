#pragma once

#include <pulse/pulseaudio.h>

#include <pulseaudio/real.hpp>

#include <iostream>
#include <functional>
#include <exception>

namespace audio {
    class Imainloop { 
        public:
            virtual void run() const = 0;
            virtual pa_mainloop_api* get_api() const = 0;
            virtual ~Imainloop() {
                 __pulse_debug_destruct( typeid(this).name() ); 
            }
    };

    class mainloop final: public Imainloop {
        public:

            mainloop(): loop( pa_mainloop_new() ) {
                __pulse_debug_construct(typeid(this).name());
            }

            void run() const override /* exception runtime_error */ {
                if ( pa_mainloop_run(loop, NULL) < 0 ) {    
                    throw std::runtime_error("Fails mainloop run");
                }
            }
            pa_mainloop_api* get_api() const override {
                return pa_mainloop_get_api(loop);
            }
            ~mainloop() override {
                __pulse_debug_destruct(typeid(this).name());
                pa_mainloop_free( loop);
            } 
        private:
            pa_mainloop* loop;
    };

}