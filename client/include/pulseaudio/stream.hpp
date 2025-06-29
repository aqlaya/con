//#pragma once

#include <pulse/pulseaudio.h>

#include "mainloop.hpp"
#include "context.hpp"

#include <iostream>
#include <functional>
#include <exception>
#include <memory>
#include <cstring>
#include <type_traits>

namespace audio {
    class Istream {
        protected:
           using func_cb_stream = func_callback<void, size_t, void*>;
        public:    
            virtual ~Istream() = 0;
            virtual void connect() = 0;
            virtual void set_callback() const noexcept;
        private:
            std::shared_ptr<Icontext> _context;
    };

    class Ipulse_stream /*: public Istream */{
        public:
            inline void 
            set_sample_spec(pa_sample_format_t format, 
                            uint32_t rate, uint8_t channels) noexcept
            {
                std::memset(&sample_spec, 0, sizeof(sample_spec) );
                sample_spec = {PA_SAMPLE_S16BE, 44100, 2};
            }

            template <typename ... Args, 
                    typename = std::enable_if_t< std::conjunction_v< std::is_enum<Args>... > > > 
            inline void set_channel_map(uint8_t n, Args ... args) 
            {
                std::memset(channel_map, 0, sizeof(pa_channel_map);
                channel_map = {n, (args, ...) };
            }

        private:
            pa_sample_spec sample_spec;
            pa_channel_map channel_map;
    };

    Istream::~Istream() {
        std::cerr << "Stream is deleted" << std::endl;
    }

}

enum class R {
    G, S, Y
};

