
#include <wsvideo-ffmpeg/middlewares/middleware.h>

typedef void (*Callback)(std::string string);

struct CallbackMiddleware : public Middleware {
private:
    Callback callback;
protected:
    void onData (std::string) override;
public:
    void init (TwoPipe* pipe, Callback callback);
};
