#pragma once

#include <pulse/pulseaudio.h>

#include <pulseaudio/context.hpp>


#include <iostream>
#include <functional>
#include <exception>
#include <memory>
#include <cstring>
#include <type_traits>

#include <cstdlib>

namespace audio {

    class Istream {
        protected:
            using func_callback_stream  = void(*)(pa_stream*, size_t, void*);
        public: 
            inline Istream(Icontext* context) 
                : _context(context)
            {
                __pulse_debug_construct( typeid(this).name() );
            } 
            virtual void connect() const = 0;
            virtual ~Istream() { __pulse_debug_destruct(typeid(this).name()); }
        private:  
            virtual void set_callback(func_callback_stream, void*) const noexcept = 0;
        private:
            Icontext* _context;
    };

    class Ipulse_stream: public Istream {
        public:
            static inline pa_sample_spec
            set_sample_spec(pa_sample_format_t format, 
                            uint32_t rate, uint8_t channels) noexcept
            {
                pa_sample_spec sample_spec;
                std::memset( &sample_spec, 0, sizeof(sample_spec) );
                sample_spec = {format, rate, channels};
                return sample_spec;
            }

            template <typename ... Args, 
            std::enable_if_t< std::conjunction_v< std::is_enum<Args>... >, int> = 0>
            static inline pa_channel_map
            set_channel_map(uint8_t n, Args ... args) noexcept
            {
                pa_channel_map channel_map;
                std::memset(&channel_map, 0, sizeof(channel_map));
                channel_map = {n, (... , args)}; 
                return channel_map;
            }

            template <typename ... Args, std::enable_if_t< 
                std::conjunction_v< std::is_enum<Args>...>, int > = 0>
            inline Ipulse_stream(Pulse_Icontext* context, const char* name,
                        pa_sample_format_t format, 
                        uint32_t rate, uint8_t channels,
                        uint8_t n, Args ... args)
            :  sample_spec( set_sample_spec(format, rate, channels) )
            ,  channel_map( set_channel_map(n, args...) ) 
            ,  pcm_stream( pa_stream_new( context->get_context(), name, &sample_spec, &channel_map ) )
            {
                __pulse_debug_construct( typeid(this).name() );
            }


            inline Ipulse_stream(Pulse_Icontext* context, const char* name, 
                pa_sample_spec sample_spec, pa_channel_map channel_map)
               : Istream(context)
               , sample_spec(sample_spec)
               , channel_map(channel_map)
               , pcm_stream( pa_stream_new( context->get_context(),
                                            name,  
                                            &sample_spec, &channel_map ) )
            {
                __pulse_debug_construct( typeid(this).name() );    
            }
        protected:
            pa_sample_spec sample_spec;
            pa_channel_map channel_map;
            pa_stream* pcm_stream;
    };


//    class pulse_stream_playback : public Ipulse_stream {
//        public:
//            pulse_stream_playback(): Ipulse_stream (/* do something */) {
//                __pulse_debug_construct( "pulse_stream_playback" );
//            }
//        private:
//    };

    class pulse_stream_record final: public Ipulse_stream {
        public:
            pulse_stream_record(Pulse_Icontext* context, const char* name, 
                pa_sample_spec sample_spec, pa_channel_map channel_map)
            : Ipulse_stream( context, name, sample_spec, channel_map)
            {
                __pulse_debug_construct( "pulse_stream_record" );

                /* neccesary, since field so match */
                std::memset( &size_buffer_attr, 0, sizeof( size_buffer_attr));

                /* set of deffault */
                size_buffer_attr.maxlength = -1;
                size_buffer_attr.fragsize = 1 << 16;
            }

            void connect() const override {
                set_callback( callback_stream_record, NULL);

                if ( pa_stream_connect_record(pcm_stream, 
                    NULL, &size_buffer_attr, PA_STREAM_NOFLAGS) != 0 ) {
                        throw std::runtime_error("***Record Stream can't connect***");
                } 
                __pulse_debug_log("[[Record stream]]", "Successfuly connect");
            }

        private:
            inline void
            set_callback( func_callback_stream func, void* pointer) const noexcept override 
            {
                /* link stream to function */
                pa_stream_set_read_callback( pcm_stream, func, pointer );

            }

            static void 
            callback_stream_record(pa_stream*, size_t nbytes, void* data) {
                if (nbytes == 0) {
                    __pulse_debug_log("[[Record stream]]", "Data sizes: 0");
                }
            }

        private:
            pa_buffer_attr size_buffer_attr;
    };
}

