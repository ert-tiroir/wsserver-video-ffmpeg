
#include <wsvideo-ffmpeg/middlewares/framewriter.h>

void FFMPEGFrameWriterMiddleware::init (VideoBroadcaster* broadcaster, int fps, int width, int height) {
    FFMPEGMiddleware::initPipe("image2pipe", fps);

    FFMPEGMiddleware::init(broadcaster);
}
