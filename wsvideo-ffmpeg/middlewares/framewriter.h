
/**********************************************************************************/
/* wsvideo-ffmpeg/middlewares/framewriter.h                                       */
/*                                                                                */
/* This file contains the details of a middleware based on FFMPEG that allows to  */
/* write the data frame by frame using OpenCV encoding, forwarding its data to a  */
/* broadcaster.                                                                   */
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

#include <opencv2/core/mat.hpp>

struct FFMPEGFrameWriterMiddleware : FFMPEGMiddleware {
private:
    cv::Mat image;

    int width, height;
    int fps; uint64_t expected_us_per_frame;

    int64_t last_frame_time = 0;
public:
    void init (VideoBroadcaster* broadcaster, int fps, int width, int height);

    int getWidth  ();
    int getHeight ();

    void setColor   (int x, int y, int r, int g, int b);
    void flushFrame (bool sleep = true);

    void waitForNextFrame ();
};
