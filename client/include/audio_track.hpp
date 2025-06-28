#pragma once

#include <memory>
#include <iostream>
#include <thread>

class Audio_Track {
    using audio_track = Audio_Track; 
    using audio_track_ref = audio_track&;
public:
    /* size buffers raw-audio data*/
    static inline const size_t nbytes = 1 << 16;

    inline Audio_Track()
        : __track(std::make_shared<char[]>( nbytes ))
    {
    }

    /* for interconnection with libopus API */
    inline Audio_Track(const void* data)
        : Audio_Track()
    {
        std::memcpy(__track.get(), data, nbytes );
    }

    /* delete copy constructor */
    Audio_Track(const audio_track_ref obj) = delete;
   
    Audio_Track& operator=(const audio_track_ref obj) = delete; 

    Audio_Track(audio_track&& obj) noexcept
        : Audio_Track()
    {
        obj.__track.reset( );
    }

    /* put data on audio array  */
    inline audio_track_ref operator=(const void* data)  {
        std::memcpy(__track.get(), data, nbytes );
    }

    Audio_Track&& operator=(audio_track&& obj)
    {
        __track = std::move(obj.__track);
        obj.__track.reset();
    }

private:
    /* pointers raw-audio data */
    std::shared_ptr<char[]> __track;

    /* mutex for access for general data */
    std::mutex mutex_track;
};