#pragma once

#include <pulseaudio/real.hpp>

#include <pulse/pulseaudio.h>

namespace audio {

    /* interface some callback function, which defined to pulseaudioi api */
    template <typename ...Args>
    class callback {
    public:
        using Funcptr = void(*)(Args ...);
    public:
        inline void operator()(Args... args) {
            (*func)(args...);
        }
        explicit callback(Funcptr func ): func(func) {}

        inline Funcptr operator&() {
            return func;
        }
    protected:
        Funcptr func;
    };

    /* interface for Pulse_Obj */
    template <typename PulseObj >
    class state_callback: public callback<PulseObj*, void*> {
    public:
        /* plug function */
        inline void operator()(PulseObj* obj, void*) const override {} 
    };

    template <>
    class state_callback<pa_context>: public callback<pa_context*, void*> {
    public:
        state_callback(): callback( &first_startegy_state) {}
    private:
        static void first_startegy_state( pa_context* obj, void* userdata ) {
            if  ( pa_context_get_state( obj) != PA_CONTEXT_READY ) {
                __pulse_debug_log("[[Audio context]]", "connection is not ready");
            } 
            __pulse_debug_log("[[Audio context]]",  "connection is succesfuly");
        }
    };

    template <>
    class state_callback<pa_stream>: public callback<pa_stream*, void*> {
    public:
        state_callback(): callback( &first_startegy_state )  {}
    private:
        static void first_startegy_state( pa_stream* obj, void* userdata ) {
            switch ( pa_stream_get_state( obj ) ) {
                case PA_STREAM_UNCONNECTED:
                    __pulse_debug_log("[[Audio stream]]", "stream unconnected");
                    break;
                case PA_STREAM_CREATING:
                    __pulse_debug_log("[[Audio stream]]", "stream creating");
                    break;
                case PA_STREAM_READY:
                    __pulse_debug_log("[[Audio stream]]", "stream ready");
                    break;
                case PA_STREAM_FAILED:
                    __pulse_debug_log("[[Audio stream]]", "stream failed");
                    break;
                case PA_STREAM_TERMINATED:
                    __pulse_debug_log("[[Audio stream]]", "stream terminated");
                    break;
            }
        }
    };

    class playback_callback: public callback<pa_stream*, size_t, void*> {
    public:
        explicit playback_callback(Funcptr func): callback(func) {}
    };

    class record_callback: public callback<pa_stream*, size_t, void*> {
    public:
        explicit record_callback(Funcptr func): callback(func) {}
    };
};