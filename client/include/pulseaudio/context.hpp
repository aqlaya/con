#pragma once

#include <pulse/pulseaudio.h>


#include <iostream>
#include <functional>
#include <exception>

namespace audio {

    template <typename T, typename... Args>
    using func_callback = std::function<void(T, Args ...)>;

    class Icontext {
        protected:
            using func_callback_context = void(*)(pa_context*, void*);
        public:
            virtual ~Icontext() = 0;
            virtual void set_callback( func_callback_context)  = 0;
            virtual pa_context_state_t get_state() const = 0;
            virtual bool is_ready() const  = 0;
            virtual void connect() = 0;
    };

    Icontext::~Icontext() {
        std::cerr << "Context is deleted" << std::endl;
    }


    class context final: public Icontext  {
        public:
            context( pa_mainloop_api* mainloop, const char* name) noexcept:
                c( pa_context_new( mainloop, name ))
            {
                std::cerr << "Context created" << name << std::endl;
            }

            void connect() override /* exception std::runtime_error */ {
                if ( pa_context_connect( this->c, NULL, PA_CONTEXT_NOFLAGS, NULL ) < 0 ) {
                    throw std::runtime_error( "Audio Context don't connect to server");
                }  
            }

            void set_callback( func_callback_context func) noexcept override  {
                pa_context_set_state_callback( this->c, func, NULL);
            }

            pa_context_state_t get_state() const noexcept override {
                return pa_context_get_state( this->c ); 
            }

            bool is_ready() const noexcept {
                return get_state() == PA_CONTEXT_READY;
            }

            ~context()  {
               pa_context_unref( c );
            }
        private:
            pa_context* c;
    };

}