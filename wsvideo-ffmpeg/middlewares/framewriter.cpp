
/**********************************************************************************/
/* wsvideo-ffmpeg/middlewares/framewriter.cpp                                     */
/*                                                                                */
/* This file contains the implementation of a middleware based on FFMPEG that     */
/* allows to write the data frame by frame using OpenCV encoding, forwarding its  */
/* data to a broadcaster.                                                         */
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

#include <wsvideo-ffmpeg/middlewares/framewriter.h>

#include <chrono>
#include <unistd.h>
#include <iostream>

#include <opencv2/imgcodecs.hpp>

/**
 * Init the middleware
 * 
 * @param broadcaster the broadcaster
 * @param fps the number of frames per second
 * @param width the width of the image
 * @param height the height of the image
*/
void FFMPEGFrameWriterMiddleware::init (VideoBroadcaster* broadcaster, int fps, int width, int height) {
    FFMPEGMiddleware::initPipe("image2pipe", fps);

    FFMPEGMiddleware::init(broadcaster);

    this->width  = width;
    this->height = height;

    this->fps = fps;

    expected_us_per_frame = 1000000 / fps;

    image = cv::Mat::zeros(cv::Size(width, height), CV_8UC3);
}

/**
 * @return the width of the image
 */
int FFMPEGFrameWriterMiddleware::getWidth  () { return width;  }
/**
 * @return the height of the image
 */
int FFMPEGFrameWriterMiddleware::getHeight () { return height; }

/**
 * @return the the RGB color of the image at a pixel
 */
void FFMPEGFrameWriterMiddleware::setColor (int x, int y, int r, int g, int b) {
    cv::Vec3b color = cv::Vec3b(r, g, b);

    image.at<cv::Vec3b>(cv::Point(x, y)) = color;
}

/**
 * Flush the current frame image, and sleeps until it reaches the next frame time
 * 
 * @param sleep whether to sleep
 */
void FFMPEGFrameWriterMiddleware::flushFrame (bool sleep) {
    vector<uchar> chrs;
    bool success = cv::imencode(".jpg", image, chrs);

    std::string bytes(chrs.size(), '.');
    for (int i = 0; i < bytes.size(); i ++)
        bytes[i] = chrs[i];

    int res = send(bytes);

    if (sleep) waitForNextFrame();
}
/**
 * Sleep until it is time to send the next frame.
 */
void FFMPEGFrameWriterMiddleware::waitForNextFrame () {
    int64_t time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    if (last_frame_time != 0) {
        int64_t expected_time = last_frame_time + expected_us_per_frame;
        int64_t duration      = expected_time - time;

        if (duration > 0) usleep(duration);
    }

    last_frame_time = time;
}
