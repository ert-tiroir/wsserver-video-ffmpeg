
#include <wsvideo-ffmpeg/pipe.h>

#include <unistd.h>
#include <fcntl.h>
#include <iostream>

const auto __write = write;
const auto __read  = read;

TwoPipe::TwoPipe (int writefd, int readfd) {
    this->writefd = writefd;
    this->readfd  = readfd;
}

size_t TwoPipe::read (char* buffer, size_t count) {
    return __read(readfd, buffer, count);
}
size_t TwoPipe::write (char* buffer, size_t count) {
    return __write(writefd, buffer, count);
}

const int READ_BUFFER_SIZE = 2048;
// static char readBuffer[READ_BUFFER_SIZE];

std::string TwoPipe::readAll () {
    std::string result;

    char readBuffer[READ_BUFFER_SIZE];

    long long size;
    while ((size = read(readBuffer, READ_BUFFER_SIZE)) > 0) {
        std::cerr << "received : " << size << std::endl;
        std::string local(size, '.');
        for (int i = 0; i < size; i ++) local[i] = readBuffer[i];

        result += local;
    }

    if (size < 0 && !gotErrorCodeWarning) {
        std::cerr << "Got error code from the pipe while trying to read all the data, got the error code : " << size << std::endl;
        gotErrorCodeWarning = true;
    }

    return result;
}

void TwoPipe::useBlocking (bool blocking) {
    int flags = fcntl(readfd, F_GETFL);
    flags    ^= flags & O_NONBLOCK;

    if (!blocking) flags |= O_NONBLOCK;
    fcntl(readfd, F_SETFL, flags);
}
void TwoPipe::closeWriter () {
    close(writefd);
}
