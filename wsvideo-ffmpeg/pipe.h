#pragma once

#include <cstddef>
#include <string>

struct TwoPipe {
private:
    int writefd;
    int readfd;
    
    bool gotErrorCodeWarning = false;
public:
    TwoPipe (int writefd, int readfd);

    size_t read  (char* buffer, size_t count);
    size_t write (char* buffer, size_t count);

    std::string readAll ();

    void useBlocking (bool blocking);
    void closeWriter ();
};
