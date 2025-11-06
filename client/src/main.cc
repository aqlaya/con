#include <pulseaudio/iface.hpp>
#include <cstdlib>
#include <thread>
#include <chrono>

audio::pulse_stream_record* p_record;
audio::pulse_stream_playback* p_playback;


char* pbuffer;

void callback_read( pa_stream* stream, size_t nbytes, void* data) {

    char* rbuffer;
    size_t bytes;


    std::cout << "Nbytes: " << nbytes << std::endl;
    std::cout << "Bytes " << bytes << std::endl;

    p_record->read(rbuffer, bytes);

    if (bytes != 0) p_record->drop();
}

void callback_write( pa_stream* stream, size_t nbytes, void* data)  { 

}


int main( int argc, char** argv ) {
    audio::mainloop loop;
    audio::context context(&loop, "My context");

    context.connect();

    while ( !context.is_ready() ) {
        loop.iterate();
    }

    audio::pulse_stream_record record (
        &context, 
        "My first record", 
        audio::Ipulse_stream::set_sample_spec(PA_SAMPLE_S16BE, 44100, 2),
        audio::Ipulse_stream::set_channel_map(2, PA_CHANNEL_POSITION_FRONT_LEFT, PA_CHANNEL_POSITION_FRONT_RIGHT),
        &callback_read
    );

    audio::pulse_stream_playback playback(
        &context,
        "My first playback",
        audio::Ipulse_stream::set_sample_spec(PA_SAMPLE_S16BE, 44100, 2),
        audio::Ipulse_stream::set_channel_map(2, PA_CHANNEL_POSITION_FRONT_LEFT, PA_CHANNEL_POSITION_FRONT_RIGHT),
        &callback_write
    );

    p_playback = &playback;
    p_record = &record;

    record.connect(); 
   // playback.connect();

    loop.run();
    return 0;
}
