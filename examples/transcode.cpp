
/**********************************************************************************/
/* examples/transcode.cpp                                                         */
/*                                                                                */
/* This file contains an example on how to broadcast a video stream that reflects */
/* a local video.                                                                 */
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

#include <unistd.h>
#include <string>
#include <wsserver/server.h>
#include <wsvideo-ffmpeg/middlewares/ffmpeg.h>

using namespace std;

WebSocketServer  server;
VideoBroadcaster broadcaster(nullptr, "");

int main () {
    // create the server and broadcaster
    server.init(5420);
    broadcaster = VideoBroadcaster(&server, "flux.mp4");

    while (!server.listen()) continue ;

    // setup the ffmpeg middleware
    FFMPEGMiddleware middleware;
    middleware.init(&broadcaster);
    middleware.getPipe()->useBlocking(false);

    {
        // read the mp4 file and send blocks of 2000 bytes every 100ms
        string str = "./test.mp4";

        FILE* file = fopen(str.c_str(), "rb");

        const int BUF_SIZE = 2000;
        char buffer[BUF_SIZE];

        ssize_t size;

        while ((size = fread(buffer, sizeof(*buffer), BUF_SIZE, file)) > 0) {
            // send the buffer
            middleware.send(buffer, size);
            // retrieve ffmpeg's output and send it to the broadcaster
            middleware.tick();
            // send old packets to new clients and setups broadcast for new clients
            broadcaster.tick();
            
            usleep(100000);
        }

        fclose(file);
    }

    // close the pipe
    middleware.getPipe()->closeWriter();
    middleware.getPipe()->useBlocking(true);
    // read the last bytes
    middleware.tick();
    broadcaster.tick();

    return 0;
}
