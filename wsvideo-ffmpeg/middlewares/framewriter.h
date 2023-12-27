
#include <wsvideo-ffmpeg/middlewares/ffmpeg.h>

#include <opencv2/core/mat.hpp>

struct FFMPEGFrameWriterMiddleware : FFMPEGMiddleware {
private:
    cv::Mat image;
public:
    void init (VideoBroadcaster* broadcaster, int fps, int width, int height);

    int getWidth  ();
    int getHeight ();

    void setColor   (int x, int y, int r, int g, int b);
    void flushFrame ();
};
