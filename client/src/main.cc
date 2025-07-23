#include <pulseaudio/iface.hpp>

static void stream_read_callback(pa_stream *stream, size_t nbytes, void *userdata) {
    const void *data;
    if (pa_stream_peek(stream, &data, &nbytes) != 0) {
        std::cerr << "pa_stream_peek() failed" << std::endl;
        return;
    }
    if ( nbytes > 0) {
        std::cerr << "Data stream is ready" << std::endl;
    }
}


void handle_callback( pa_context *context, void *userdata) {
    pa_context_state state = pa_context_get_state(context);
    if ( state == PA_CONTEXT_READY ) {
        std::cerr << "[PA CONTEXT] " << "READY\n" << std::endl;
    } 
}


void stream_callback(pa_stream* c, void* userdata) {
    if (pa_stream_get_state(c) != PA_STREAM_READY) {
        std::cerr << "Stream ready!" << std::endl;
    }
    else {
        std::cerr << "Stream can't ready" << std::endl;
    }
}


int main( int argc, char** argv ) {
    audio::mainloop loop;
    audio::context context(&loop, "My first context");

    context.connect();

    while ( !context.is_ready() ) {
        loop.iterate();
    }

    audio::pulse_stream_record record( 
        &context, 
        "My first record", 
        audio::Ipulse_stream::set_sample_spec(PA_SAMPLE_S16BE, 44100, 2),
        audio::Ipulse_stream::set_channel_map(2, PA_CHANNEL_POSITION_FRONT_LEFT, PA_CHANNEL_POSITION_FRONT_RIGHT)
    );

    record.connect();

    loop.run();


   // pa_mainloop *mainloop = pa_mainloop_new();
   // pa_context *context = pa_context_new(pa_mainloop_get_api(mainloop), "Audio Recorder");
   // pa_stream *stream;

   // pa_context_set_state_callback( context, handle_callback, NULL);

   // pa_context_connect(context, NULL, PA_CONTEXT_NOFLAGS, NULL);

   // pa_context_state_t state = pa_context_get_state(context);

   // pa_sample_spec sample_spec;
   // pa_channel_map channel_map;
   // std::memset( &channel_map, 0, sizeof(channel_map));
   // std::memset( &sample_spec, 0, sizeof(sample_spec));

   // sample_spec =  {PA_SAMPLE_S16BE, 44100, 2};

   // channel_map =  {2, PA_CHANNEL_POSITION_FRONT_LEFT, PA_CHANNEL_POSITION_FRONT_RIGHT};

   // // event-loop connect 
   // while (state != PA_CONTEXT_READY) {
   //     pa_mainloop_iterate(mainloop, 1, NULL);
   //     state = pa_context_get_state(context);
   // }

   // stream = pa_stream_new(context, "My Audio Stream", &sample_spec, &channel_map);

   // pa_stream_set_read_callback(stream, stream_read_callback, NULL);
   // pa_stream_set_state_callback(stream, stream_callback, NULL);
   // pa_buffer_attr size_buffer_attr;
   // std::memset(&size_buffer_attr, 0, sizeof(size_buffer_attr));

   // size_buffer_attr.maxlength = -1;
   // size_buffer_attr.fragsize  = (1 << 16);


   // if (pa_stream_connect_record(stream, NULL, &size_buffer_attr, PA_STREAM_NOFLAGS) != 0) {
   //     fprintf(stderr, "pa_stream_connect_record() failed\n");
   //     return -1;
   // }

   // // event-loop run
   // pa_mainloop_run(mainloop, NULL);

    return 0;
}


