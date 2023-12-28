
/**********************************************************************************/
/* examples/static_image.cpp                                                      */
/*                                                                                */
/* This file contains an example on how to broadcast a video stream containing    */
/* the same frame infinitely                                                      */
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

#include <stdlib.h>
#include <string>
#include <iostream>
#include <wsserver/server.h>
#include <wsvideo-ffmpeg/middlewares/framewriter.h>

using namespace std;

WebSocketServer  server;
VideoBroadcaster broadcaster(nullptr, "");

/**
 * Read the bytes contained in the image and store it into a string
 */
std::string readImageBytes () {
    const int BUFFER_SIZE = 2048;
    char buffer[BUFFER_SIZE];

    std::string result = "";

    FILE*  file = fopen("test.jpg", "r");
    size_t size;

    // read BUFFER_SIZE blocks
    while ((size = fread(buffer, sizeof(*buffer), BUFFER_SIZE, file)) > 0) {
        std::string local(size, '.');
        for (int i = 0; i < size; i ++) local[i] = buffer[i];

        result += local;
    }

    return result;
}

int main () {
    // create the server and broadcaster
    server.init(5420);

    broadcaster = VideoBroadcaster(&server, "flux.mp4");

    // listen for a single client
    while (!server.listen()) continue ;

    // create the middleware
    FFMPEGFrameWriterMiddleware middleware;
    // 25 fps, width, height is useless as the image data is already encoded
    middleware.init(&broadcaster, 25, 0, 0);
    // Use non blocking I/O for the ffmpeg pipe
    middleware.getPipe()->useBlocking(false);

    // read the image bytes
    std::string buffer = readImageBytes();

    while (true) {
        // send the image data
        middleware.send(buffer);
        // retrieve output of ffmpeg and forward it to the broadcaster
        middleware.tick();
        // send old packets to the new clients and sets up the broadcast for the new clients
        broadcaster.tick();

        // wait for the next frame to sync with the fps
        middleware.waitForNextFrame();
    }

    // close the writer
    middleware.getPipe()->closeWriter();
    middleware.getPipe()->useBlocking(true);
    // read the last bytes from the reader
    middleware.tick();
    broadcaster.tick();

    return 0;
}
