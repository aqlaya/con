#pragma once

#include <type_traits>

#include <pulse/pulseaudio.h>

namespace audio {

    /* needle concepts */
    namespace detail {
        template < typename T>
        concept PulseStream = std::is_same_v<T, pa_stream>;

        template <typename T>
        concept PulseContext = std::is_same_v<T, pa_context>;
    }
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
    protected:
        inline void get_state_for_context(PulseObj* obj, void*) {
            if  ( pa_context_get_state( obj) != PA_CONTEXT_READY ) {
                __pulse_debug_log("[[Audio context]]", "connection is not ready");
            } 
            __pulse_debug_log("[[Audio context]]",  "connection is succesfuly");
        }

        inline void get_state_for_stream(PulseObj* obj, void*) {
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
    public:
        inline void operator()(PulseObj* obj, void*) const override {
            if constexpr ( detail::PulseStream<PulseObj> ) {
                get_state_for_context();
            }
            else {
                get_state_for_stream();
            }
        } 
        inline decltype(auto) operator*() const {
            return &operator();
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