
/**********************************************************************************/
/* wsvideo-ffmpeg/pipe.cpp                                                        */
/*                                                                                */
/* This file contains the implementation of a bi-directional pipe between a       */
/* program and another one                                                        */
/**********************************************************************************/
/*                  This file is part of the ERT-Tiroir project                   */
/*                  github.com/ert-tiroir/wsserver-video-ffmpeg                   */
/**********************************************************************************/
/* MIT License                                                                    */
/*                                                                                */
/* Copyright (c) 2023 ert-tiroir                                                  */
/*                                                                                */
/* Permission is hereby granted, free of charge, to any person obtaining a copy   */
/* of this software and associated documentation files (the "Software"), to deal  */
/* in the Software without restriction, including without limitation the rights   */
/* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      */
/* copies of the Software, and to permit persons to whom the Software is          */
/* furnished to do so, subject to the following conditions:                       */
/*                                                                                */
/* The above copyright notice and this permission notice shall be included in all */
/* copies or substantial portions of the Software.                                */
/*                                                                                */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     */
/* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       */
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    */
/* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         */
/* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  */
/* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  */
/* SOFTWARE.                                                                      */
/**********************************************************************************/

#include <wsvideo-ffmpeg/pipe.h>

#include <unistd.h>
#include <fcntl.h>
#include <iostream>

// create saves of stdlib write and read functions
const auto __write = write;
const auto __read  = read;

/**
 * Create a bi-directional pipe
 * 
 * @param writefd the file descriptor representing the standard input of the other program
 * @param readfd  the file descriptor representing the standard output of the other program
 */
TwoPipe::TwoPipe (int writefd, int readfd) {
    this->writefd = writefd;
    this->readfd  = readfd;
}

/**
 * Read the data from stdout
 * @param buffer the buffer on which to write the data
 * @param count the size of the buffer
 * @return the byte count that has been stored in the buffer
 */
size_t TwoPipe::read (char* buffer, size_t count) {
    return __read(readfd, buffer, count);
}
/**
 * Write data to stdin
 * @param buffer the content to write
 * @param count the size of the content
 * @return the byte count that has been succesfully written to the stdin
 */
size_t TwoPipe::write (char* buffer, size_t count) {
    return __write(writefd, buffer, count);
}

/**
 * Read all the data contained in stdout
 * @return all the data in stdout
 */
std::string TwoPipe::readAll () {
    std::string result;

    const int READ_BUFFER_SIZE = 2048;
    char readBuffer[READ_BUFFER_SIZE];

    long long size;
    // read chunk by chunk and if there is no more data return what you found
    while ((size = read(readBuffer, READ_BUFFER_SIZE)) > 0) {
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

/**
 * Sets up blocking / non-blocking i/o
 * 
 * @param blocking whether reading from the standard output should block the thread
 */
void TwoPipe::useBlocking (bool blocking) {
    int flags = fcntl(readfd, F_GETFL);
    flags    ^= flags & O_NONBLOCK;

    if (!blocking) flags |= O_NONBLOCK;
    fcntl(readfd, F_SETFL, flags);
}
/**
 * Closes the writer file descriptor to end the program or to flush the last data
 */
void TwoPipe::closeWriter () {
    close(writefd);
}
