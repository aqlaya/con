#pragma once

#include <pulseaudio/real.hpp>

#include <pulse/pulseaudio.h>

namespace audio {

    /* interface some callback function, which defined to pulseaudioi api */
    template <typename ...Args>
    class Icallback {
    private:
        using Funcptr = void(*)(Args ...);
    public:
        virtual void operator()(Args... args)  = 0;  
        virtual Funcptr operator&() = 0;
    };

    /* interface for Pulse_Obj */
    template <typename PulseObj >
    class state_callback: public Icallback<PulseObj*, void*> {
    private:
        using Funcptr = void(*)(PulseObj*, void*);
    public:
        /* plug function */
        inline void operator()(PulseObj* obj, void*) const override {} 

        inline Funcptr operator&() override {
            return nullptr;
        }

    };

    template <>
    class state_callback<pa_context>: public Icallback<pa_context*, void*> {
    private:
        using Funcptr = void(*)(pa_context*, void*);
    public:
        inline void operator()(pa_context* obj, void* userdata)  override {
            first_startegy_state(obj, userdata);
        }

        inline Funcptr operator&() override {
            return &first_startegy_state;
        }
    private:
        static void first_startegy_state( pa_context* obj, void* userdata ) {
            if  ( pa_context_get_state( obj) != PA_CONTEXT_READY ) {
                __pulse_debug_log("[[Audio context]]", "connection is not ready");
            } 
            __pulse_debug_log("[[Audio context]]",  "connection is succesfuly");
        }
    };

    template <>
    class state_callback<pa_stream>: public Icallback<pa_stream*, void*> {
    private:
        using Funcptr = void(*)(pa_stream*, void*);
    public:
        inline void operator()(pa_stream* obj, void* userdata)  override {
            first_startegy_state(obj, userdata);
        }

        inline Funcptr operator&() override {
            return nullptr;
        }
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

    class playback_callback: public Icallback<pa_stream*, size_t, void*> {
    private:
        using Funcptr = void(*)(pa_stream*, size_t, void*);
    public:
        inline void operator()( pa_stream* obj, size_t nbytes, void* userdata)  override {
            first_stagery_playback( obj, nbytes, userdata);
        } 
        inline Funcptr operator&() override {
            return &first_stagery_playback;
        }
    private:
        static void first_stagery_playback(pa_stream* obj, size_t nbytes, void* userdata) {
            if (nbytes == 0) {
                __pulse_debug_log("[[Playback stream]]", "Zero nbytes");
            }
        }
    };

    class record_callback: public Icallback<pa_stream*, size_t, void*> {
    private:
        using Funcptr = void(*)(pa_stream*, size_t, void*);
    public:
        inline void operator()(pa_stream* obj, size_t nbytes, void* userdata)  override {
            first_stagery_record( obj, nbytes, userdata);
        }

        inline Funcptr operator&() override {
            return first_stagery_record;
        }
    private:
        static void first_stagery_record(pa_stream* obj, size_t nbytes, void* userdata) {
            if (nbytes == 0) {
                __pulse_debug_log("[[Record stream]]", "Data sizes: 0"); 
            } else {
                __pulse_debug_log("[[Record stream]]", "Data sizes: 10"); 
            }
        }
    };
};