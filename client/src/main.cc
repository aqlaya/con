
#include <pulseaudio/iface.hpp>

int main( int argc, char** argv ) {
    audio::mainloop loop;
    audio::context context(&loop, "My first context");

    context.connect();

    audio::pulse_stream_record record( 
        &context, 
        "My first record", 
        audio::Ipulse_stream::set_sample_spec(PA_SAMPLE_S16BE, 44100, 2),
        audio::Ipulse_stream::set_channel_map(2, PA_CHANNEL_POSITION_FRONT_LEFT, PA_CHANNEL_POSITION_FRONT_RIGHT)
    );

    record.connect();

    loop.run();
    return 0;
}


