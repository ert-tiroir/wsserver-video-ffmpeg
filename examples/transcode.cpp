
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/prctl.h>
#include <sys/wait.h>
#include <signal.h>
#include <string>
#include <fcntl.h>
#include <wsserver/server.h>
#include <wsvideo-ffmpeg/pipe.h>
#include <wsvideo-ffmpeg/subprocess.h>
#include <wsvideo-ffmpeg/middlewares/ffmpeg.h>

using namespace std;

WebSocketServer  server;
VideoBroadcaster broadcaster(nullptr, "");

int main () {
    server.init(5420);

    broadcaster = VideoBroadcaster(&server, "flux.mp4");

    while (!server.listen()) continue ;
    
    int total = 0;

    FFMPEGMiddleware middleware;
    middleware.init(&broadcaster);
    middleware.getPipe()->useBlocking(false);

    for (int i = 1; i < 2; i ++) {
        string str = "./test.mp4";

        FILE* file = fopen(str.c_str(), "rb");

        const int BUF_SIZE = 2000;
        char buffer[BUF_SIZE];

        ssize_t size;

        while ((size = fread(buffer, sizeof(*buffer), BUF_SIZE, file)) > 0) {
            middleware.send(buffer, size);

            middleware.tick();
            broadcaster.tick();
            
            usleep(100000);
        }

        fclose(file);
    }

    middleware.getPipe()->closeWriter();
    middleware.getPipe()->useBlocking(true);

    middleware.tick();
    broadcaster.tick();

    return 0;
}
