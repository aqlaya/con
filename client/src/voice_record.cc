#include <iostream>
#include <cstdlib>
#include <memory>
#include <pulse/pulseaudio.h>
#include <cstring>
#include <thread>


#include "audio_track.hpp"

static Audio_Track track;

pa_sample_spec my_voice_spec  =  {PA_SAMPLE_S16BE, 4410, 2};
pa_channel_map my_voice_map;

static void write_track(pa_mainloop* event_loop, Audio_Track& track)
{
    pa_context* playback_context = pa_context_new( pa_mainloop_get_api( event_loop), "Playback Context" ); 

    pa_context_connect( playback_context, NULL, PA_CONTEXT_NOFLAGS, NULL);

    pa_stream* playback_stream = pa_stream_new(  playback_context,   
                                                "My playback voice", 
                                                &my_voice_spec,
                                                &my_voice_map
    );

    pa_stream_connect_playback( playback_sream, NULL, NULL, );
}

static void stream_state_callback(pa_stream* stream, void *userdata) {

    std::cerr << "[AUDIO STREAM] " << "Connect succesfully\n";

    track = userdata;

    /* wait buff full */
    std::this_thread::sleep_for(std::chrono::seconds(10));

    std::thread write_track_thread( write_track, ... ); /* TODO */

    write_track_thread.join();
    return ;
}

int main() {

    pa_mainloop* event_loop_for_audio = pa_mainloop_new();

    pa_mainloop_api* mainloop_api = pa_mainloop_get_api( event_loop_for_audio);

    pa_context* audio_context = pa_context_new( mainloop_api, "My_Voice_Stream");

    pa_context_connect( audio_context, NULL, PA_CONTEXT_NOFLAGS, NULL );


    std::memset(&my_voice_map, 0, sizeof(my_voice_map) );

    my_voice_map = {2, PA_CHANNEL_POSITION_TOP_FRONT_LEFT, PA_CHANNEL_POSITION_TOP_FRONT_RIGHT};

    pa_stream* pcm_stream = pa_stream_new( audio_context, 
        "My voice stream",
        &my_voice_spec, 
        &my_voice_map
    );

    pa_stream_set_state_callback( pcm_stream, stream_state_callback, NULL );
    pa_stream_connect_record( pcm_stream, NULL, NULL, PA_STREAM_NOFLAGS);

    // event-loop run
    int error_callback;
    pa_mainloop_run( event_loop_for_audio, &error_callback);



    // free resource
    pa_context_unref( audio_context);
    pa_mainloop_free( event_loop_for_audio );

    return 0;
}
