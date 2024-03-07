
/**********************************************************************************/
/* wsvideo-ffmpeg/middlewares/callback.cpp                                        */
/*                                                                                */
/* This file contains the details of a middleware based on FFMPEG forwarding the  */
/* result to a broadcaster.                                                       */
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

#include <wsvideo-ffmpeg/middlewares/middleware.h>
#include <wsvideo/broadcaster.h>

struct FFMPEGMiddleware : public Middleware {
private:
    VideoBroadcaster* broadcaster;
protected:
    void onData (std::string) override;

    void initPipe (const char* inputFormat, int fps);
    void initPipe ();
public:
    void init (VideoBroadcaster* broadcaster);
};
