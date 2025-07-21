#pragma once

#include <pulse/pulseaudio.h>

#include <pulseaudio/mainloop.hpp>

#include <iostream>
#include <functional>
#include <exception>
#include <chrono>

namespace audio {

    template <typename T, typename... Args>
    using func_callback = std::function<void(T, Args ...)>;

    class Icontext {
        protected:
            using func_callback_context = void(*)(pa_context*, void*);
        public:
            virtual ~Icontext() 
            { 
                __pulse_debug_destruct(typeid(this).name()); 
            }
            virtual void set_callback( func_callback_context) const noexcept = 0;
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

            bool is_ready() const noexcept {
                return get_state() == PA_CONTEXT_READY;
            }
        protected:
            pa_context* c;
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
                set_callback(callback_context);
                if ( pa_context_connect(c, NULL, PA_CONTEXT_NOFLAGS, NULL ) < 0 ) {
                    throw std::runtime_error( "***Audio Context don't connect to server***");
                }  

            }

            ~context() override {
               __pulse_debug_destruct( typeid(this).name());
            } 
        private:
            void set_callback( func_callback_context func) const noexcept override  {
                pa_context_set_state_callback( this->c, func, NULL);
            }     

            static void 
            callback_context(pa_context* c, void* userdata) /* runtime_error */ {
                const auto start_time  = std::chrono::steady_clock::now();
                if  ( pa_context_get_state(c) != PA_CONTEXT_READY ) {
                    __pulse_debug_log("[[Audio context]]", "connection is not ready");
                }    
                __pulse_debug_log("[[Audio context]]",  "connection is succesfuly");
            }   
    };

}