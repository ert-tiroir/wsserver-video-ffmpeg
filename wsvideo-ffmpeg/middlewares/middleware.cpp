
#include <wsvideo-ffmpeg/middlewares/middleware.h>

#include <iostream>

void Middleware::init (TwoPipe* pipe) {
    this->pipe = pipe;
}

size_t Middleware::send (std::string data) {
    char* buffer = new char[data.size()];

    for (int i = 0; i <= data.size(); i ++)
        buffer[i] = data[i];

    size_t res = send(buffer, data.size());
    delete [] buffer;

    return res;
}
size_t Middleware::send (char* buffer, size_t size) {
    return this->pipe->write(buffer, size);
}

void Middleware::tick () {
    std::string result = this->pipe->readAll();
    if (result.size() == 0) return ;

    this->onData(result);
}
void Middleware::onData (std::string data) {
    std::cerr << "Received data for abstract middleware : " << data.size() << std::endl;
}
bool Middleware::hasPipe () {
    return pipe != nullptr;
}
TwoPipe* Middleware::getPipe () {
    return pipe;
}