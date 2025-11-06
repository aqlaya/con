#pragma once


#include <pulseaudio/mainloop.hpp>
#include <pulseaudio/callback.hpp>

#include <iostream>
#include <functional>
#include <exception>
#include <chrono>

namespace audio {
    class Icontext {
        public:
            virtual ~Icontext() 
            { 
                __pulse_debug_destruct(typeid(this).name()); 
            }
            virtual pa_context_state_t get_state() const = 0;
            virtual bool is_ready() const  = 0;
            virtual void connect() = 0;
    };

    class Pulse_Icontext: public Icontext {
        public:
            Pulse_Icontext( Imainloop* mainloop, const char* name) noexcept:
                c( pa_context_new( mainloop->get_api(), name ))
            {
                __pulse_debug_construct(typeid(this).name());
            }
            virtual pa_context* get_context() const noexcept {
                return c;
            }
            virtual ~Pulse_Icontext() {
                pa_context_unref(c);
                __pulse_debug_destruct(typeid(this).name());
            }
            pa_context_state_t get_state() const noexcept override {
                return pa_context_get_state( this->c ); 
            }

            bool is_ready() const noexcept override {
                return get_state() == PA_CONTEXT_READY;
            }
        protected:
            pa_context* c;
            mutable state_callback<pa_context> _state_callback;
    };

    class context final: public Pulse_Icontext  {
        public:
            context(Imainloop* mainloop, const char* name) noexcept
                : Pulse_Icontext(mainloop, name) 
            {
                __pulse_debug_construct( typeid(this).name() );
            }

            void connect() override /* exception std::runtime_error */ {
                __pulse_debug_log("[[Audio context]]", "begin to conntect");
                pa_context_set_state_callback( this->c, &_state_callback, NULL);
                if ( pa_context_connect(c, NULL, PA_CONTEXT_NOFLAGS, NULL ) < 0 ) {
                    throw std::runtime_error( "***Audio Context don't connect to server***");
                }  

            }

            ~context() override {
               __pulse_debug_destruct(typeid(this).name());
            } 
    };

}