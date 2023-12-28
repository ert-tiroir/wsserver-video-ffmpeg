
/**********************************************************************************/
/* examples/dynamic_image.cpp                                                     */
/*                                                                                */
/* This file contains an example on how to broadcast a video stream containing    */
/* user made images programatically                                               */
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

#include <string>
#include <wsserver/server.h>
#include <wsvideo-ffmpeg/middlewares/framewriter.h>

using namespace std;

WebSocketServer  server;
VideoBroadcaster broadcaster(nullptr, "");

/**
 * Fill the image of the middleware
 * 
 * @param middleware the middleware containing the image
 * @param frame the index of the frame
 */
void fillImage (FFMPEGFrameWriterMiddleware& middleware, int frame) {
    for (int x = 0; x < middleware.getWidth(); x ++) {
        for (int y = 0; y < middleware.getHeight(); y ++) {
            // (Y, Cb, Cr) format
            int Y  = (x + y   + frame * 3) & 255;
            int Cb = (128 + y + frame * 2) & 255;
            int Cr = (64  + x + frame * 5) & 255;

            // (R, G, B) format
            int r = std::min(255, (int) std::max(0.0, 1.164 * (Y - 16) + 0     * (Cb - 128) + 1.596 * (Cr - 64)));
            int g = std::min(255, (int) std::max(0.0, 1.164 * (Y - 16) - 0.392 * (Cb - 128) - 0.813 * (Cr - 64)));
            int b = std::min(255, (int) std::max(0.0, 1.164 * (Y - 16) + 2.017 * (Cb - 128) + 0     * (Cr - 64)));

            middleware.setColor(x, y, r, g, b);
        }
    }
}

/**
 * Run the server
 */
int main () {
    server.init(5420);

    // create the broadcaster on channel "flux.mp4"
    broadcaster = VideoBroadcaster(&server, "flux.mp4");

    // wait for one client to connect (technically more is possible but this is a simple example)
    while (!server.listen()) continue ;

    // create the middleware
    FFMPEGFrameWriterMiddleware middleware;
    // 25fps, width = height = 300
    middleware.init(&broadcaster, 25, 300, 300);
    // Use non blocking I/O for the ffmpeg middleware
    middleware.getPipe()->useBlocking(false);

    int frame = 0;
    while (true) {
        // fill the image
        fillImage(middleware, frame);

        // flush the frame and sleeps until time has reached the next frame
        middleware.flushFrame();
        // read the output of the middleware and forward it to the broadcaster
        middleware.tick();

        // send packets to new clients (here only useful at frame 0)
        broadcaster.tick();

        frame ++;
    }

    // close the pipe
    middleware.getPipe()->closeWriter();
    middleware.getPipe()->useBlocking(true);

    // read the last bits of information from the pipe
    middleware.tick();
    broadcaster.tick();

    return 0;
}
