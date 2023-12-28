
/**********************************************************************************/
/* wsvideo-ffmpeg/pipe.h                                                          */
/*                                                                                */
/* This file contains the description of a bi-directional pipe between a program  */
/* and another one                                                                */
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

#pragma once

#include <cstddef>
#include <string>

/**
 * This structure represents a two pipe object
 */
struct TwoPipe {
private:
    // file descriptor to forward to the stdin of the other program
    int writefd;
    // file descriptor to retrieve the stdout of the other program
    int readfd;
    
    // the pipe will display an error message when the read returns -1
    // but to avoid spamming, it will only send it once per pipe instance
    bool gotErrorCodeWarning = false;
public:
    TwoPipe (int writefd, int readfd);

    size_t read  (char* buffer, size_t count);
    size_t write (char* buffer, size_t count);

    std::string readAll ();

    void useBlocking (bool blocking);
    void closeWriter ();
};
