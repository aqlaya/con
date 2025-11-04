#pragma once

#include <type_traits>

#include <pulse/pulseaudio.h>

namespace audio {

    /* interface some callback function, which defined to pulseaudioi api */
    template <typename ...Args>
    class Icallback {
    public:
        virtual void operator()(Args... args) const = 0;
        /* expilicitly declare default contsturcor (not touch) */
        Icallback() = default; 
    };


    /* interface for Pulse_Obj */
    template <typename PulseObj >
    class state_callback: public Icallback<PulseObj*, void*> {
    public:

        inline void operator()(PulseObj* obj, void*) const override;
        inline decltype(auto) operator*() const {
            return &operator();
        }
        /* expilicitly declare default contsturcor (not touch) */
        state_callback() = default;
    };

    template <typename PulseObj>
    inline void state_callback<PulseObj>::operator()(PulseObj* obj, void*) const {

    }

    template <>
    class state_callback<pa_stream>: public Icallback<pa_stream*, void*> {
    public:
        /* expilicitly declare default contsturcor (not touch) */
        state_callback() = default;
    };

    namespace detail {
        template < typename T>
        concept PulseStream = std::is_same_v<T, pa_stream>;

        template <typename T>
        concept PulseContext = std::is_same_v<T, pa_context>;
    }

    template <>
    class state_callback<pa_context>: public Icallback<pa_context*, void*> {
    public:
        inline void operator()( pa_context* obj, void* userdata) const override {
            if  ( pa_context_get_state( obj) != PA_CONTEXT_READY ) {
                __pulse_debug_log("[[Audio context]]", "connection is not ready");
            } 
            __pulse_debug_log("[[Audio context]]",  "connection is succesfuly");
        }


        /* expilicitly declare default contsturcor (not touch) */
        state_callback() = default;
    };

    template <typename PulseObj>
    class playback_callback: public Icallback<PulseObj, size_t, void*> {
    public:
        inline void operator()( PulseObj* obj, size_t nbytes, void* userdata) {
            char* data = new char[nbytes];

            if (nbytes == 0) {
                __pulse_debug_log("[[Playback stream]]", "Zero nbytes");
            }
        }
        
        playback_callback() = default;
    };

    template <typename PulseObj>
    class record_callback: public Icallback<PulseObj, size_t, void*> {
    public:
        inline void operator()(PulseObj* obj, size_t nbytes, void* userdata) {
            if (nbytes == 0) {
                __pulse_debug_log("[[Record stream]]", "Data sizes: 0"); 
            }
        }

        /* expilicitly declare default contsturcor (not touch) */
        record_callback() = default;
    };
};