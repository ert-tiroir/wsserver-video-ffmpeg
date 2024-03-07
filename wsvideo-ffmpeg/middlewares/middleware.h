
/**********************************************************************************/
/* wsvideo-ffmpeg/middlewares/middleware.h                                        */
/*                                                                                */
/* This file contains the details of an abstract stream middleware                */
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
#include <wsvideo-ffmpeg/pipe.h>

#include <string>

/**
 * A middleware is an external software to which this program can communicate,
 * and is used in a chain.
*/
struct Middleware {
private:
    // the bi directional pipe of the external software
    TwoPipe* pipe = nullptr;
protected:
    void init (TwoPipe* pipe);

    virtual void onData (std::string string);
public:
    size_t send (std::string string);
    size_t send (char* buffer, size_t size);

    void tick ();

    bool     hasPipe ();
    TwoPipe* getPipe ();
};
