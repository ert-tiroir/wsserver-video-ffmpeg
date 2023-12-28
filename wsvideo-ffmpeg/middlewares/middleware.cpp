
/**********************************************************************************/
/* wsvideo-ffmpeg/middlewares/middleware.cpp                                      */
/*                                                                                */
/* This file contains the implementation of an abstract stream middleware         */
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

#include <wsvideo-ffmpeg/middlewares/middleware.h>

#include <iostream>

/**
 * Init the middleware
 * 
 * @param pipe the program pipe
 */
void Middleware::init (TwoPipe* pipe) {
    this->pipe = pipe;
}

/**
 * Send a string to the external software
 * 
 * @param string the string to send
 * @return the size of the data that was sent
 */
size_t Middleware::send (std::string data) {
    char* buffer = new char[data.size()];

    for (int i = 0; i < data.size(); i ++)
        buffer[i] = data[i];

    size_t res = send(buffer, data.size());
    delete [] buffer;

    return res;
}
/**
 * Send a buffer to the external software
 * 
 * @param buffer the buffer to send
 * @param size the size of the content
 * @return the size of the data that was sent
 */
size_t Middleware::send (char* buffer, size_t size) {
    return this->pipe->write(buffer, size);
}

/**
 * Read standard output of the external software and sends its data to onData.
 */
void Middleware::tick () {
    std::string result = this->pipe->readAll();
    if (result.size() == 0) return ;

    this->onData(result);
}
/**
 * Do something when the middleware received data
 * 
 * @param string the data
 */
void Middleware::onData (std::string data) {
    std::cerr << "Received data for abstract middleware : " << data.size() << std::endl;
}
/**
 * @return whether the middleware has a pipe
 */
bool Middleware::hasPipe () {
    return pipe != nullptr;
}
/**
 * @return the pipe of the middleware
 */
TwoPipe* Middleware::getPipe () {
    return pipe;
}