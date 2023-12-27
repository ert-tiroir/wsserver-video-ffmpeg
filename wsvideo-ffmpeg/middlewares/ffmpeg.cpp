
#include <wsvideo-ffmpeg/middlewares/ffmpeg.h>
#include <wsvideo-ffmpeg/subprocess.h>

#include <string.h>
#include <string>

void FFMPEGMiddleware::onData(std::string data) {
    this->broadcaster->sendPacket(data);
}

void FFMPEGMiddleware::init (VideoBroadcaster* broadcaster) {
    this->broadcaster = broadcaster;
    if (Middleware::hasPipe()) return ;

    this->initPipe();
}

void FFMPEGMiddleware::initPipe() {
    Middleware::init( 
        subprocess("ffmpeg", 
                   "ffmpeg", 
                   "-re", "-i", "-", "-g", "50",
                   "-movflags", "frag_keyframe+empty_moov+default_base_moof", "-f", "mp4",
                   "-")
    );
}
void FFMPEGMiddleware::initPipe (const char* format, int fps) {
    std::string  strfps = to_string(fps);
    const char* cstrfps = strfps.c_str();
    
    Middleware::init(
        subprocess("ffmpeg", 
                   "ffmpeg", 
                   "-re", "-f", strdup(format), "-r", strdup(cstrfps),
                   "-i", "-", "-g", "50",
                   "-movflags", "frag_keyframe+empty_moov+default_base_moof", "-f", "mp4",
                   "-")
    );
}