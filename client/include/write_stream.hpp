#pragma once

#include <pulse/pulseaudio.h>

/* interface to writeStreamThread */
class IWriteStreamThread {
    virtual ~IWriteStreamThread() = 0;
    virtual create_thread();
};



pa_mainloop* event_loop;