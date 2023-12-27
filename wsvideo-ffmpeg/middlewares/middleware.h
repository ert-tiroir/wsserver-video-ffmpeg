
#include <wsvideo-ffmpeg/pipe.h>

#include <string>

struct Middleware {
private:
    TwoPipe* pipe = nullptr;
protected:
    void init    (TwoPipe* pipe);

    virtual void onData (std::string string);
public:
    size_t send (std::string string);
    size_t send (char* buffer, size_t size);

    void tick ();

    bool     hasPipe ();
    TwoPipe* getPipe();
};
