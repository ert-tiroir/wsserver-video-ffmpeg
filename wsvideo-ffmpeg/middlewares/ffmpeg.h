
#include <wsvideo-ffmpeg/middlewares/middleware.h>
#include <wsvideo/broadcaster.h>

struct FFMPEGMiddleware : public Middleware {
private:
    VideoBroadcaster* broadcaster;
protected:
    void onData (std::string) override;

    void initPipe (const char* inputFormat, int fps);
    void initPipe ();
public:
    void init (VideoBroadcaster* broadcaster);
};
