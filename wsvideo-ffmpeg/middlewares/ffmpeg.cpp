
/**********************************************************************************/
/* wsvideo-ffmpeg/middlewares/callback.cpp                                        */
/*                                                                                */
/* This file contains the implementation of a middleware based on FFMPEG          */
/* forwarding the result to a broadcaster.                                        */
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

#include <wsvideo-ffmpeg/middlewares/ffmpeg.h>
#include <wsvideo-ffmpeg/subprocess.h>

#include <string.h>
#include <string>

/**
 * When data is received, forward it to the broadcaster
 * 
 * @param data the data received
 */
void FFMPEGMiddleware::onData(std::string data) {
    this->broadcaster->sendPacket(data);
}
/**
 * Init the middleware
 * 
 * @param broadcaster the broadcaster
 */
void FFMPEGMiddleware::init (VideoBroadcaster* broadcaster) {
    this->broadcaster = broadcaster;
    if (Middleware::hasPipe()) return ;

    // if the middleware has no pipe, use the default one
    this->initPipe();
}

/**
 * Create the default FFMPEG pipe
 */
void FFMPEGMiddleware::initPipe() {
    Middleware::init( 
        subprocess("ffmpeg", 
                   "ffmpeg", 
                   "-re", "-i", "-", "-g", "50",
                   "-movflags", "frag_keyframe+empty_moov+default_base_moof", "-f", "mp4",
                   "-")
    );
}
/**
 * Create a more detailed FFMPEG pipe
 */
void FFMPEGMiddleware::initPipe (const char* format, int fps) {
    std::string  strfps = to_string(fps);
    const char* cstrfps = strfps.c_str();
    
    Middleware::init(
        subprocess("ffmpeg", 
                   "ffmpeg", 
                   "-re", "-f", strdup(format), "-r", strdup(cstrfps),
                   "-i", "-", "-g", "50",
                   "-movflags", "frag_keyframe+empty_moov+default_base_moof", "-f", "mp4",
                   "-")
    );
}