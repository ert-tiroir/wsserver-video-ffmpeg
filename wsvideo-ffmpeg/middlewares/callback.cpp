
#include <wsvideo-ffmpeg/middlewares/callback.h>

void CallbackMiddleware::onData(std::string data) {
    this->callback(data);
}

void CallbackMiddleware::init (TwoPipe* pipe, Callback callback) {
    Middleware::init(pipe);

    this->callback = callback;
}
