#pragma once

#include <wsvideo-ffmpeg/pipe.h>

#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/prctl.h>
#include <sys/signal.h>

std::string which (std::string program);

template<typename... Args>
TwoPipe* subprocess (const char* program, Args... args) {
    int infd[2], outfd[2];

    pipe(infd); pipe(outfd);

    std::string programPath = which(program);
    program = programPath.c_str();

    int pid = fork();

    switch (pid) {
        case -1:
          {
            std::cerr << "Fork did not return a valid pid\n";
            return new TwoPipe(-1, -1);
          }
        case 0:
          {
            dup2(outfd[0], STDIN_FILENO );
            dup2( infd[1], STDOUT_FILENO);

            prctl(PR_SET_PDEATHSIG, SIGTERM);

            close(outfd[1]);
            close( infd[0]);
            
            execl(program, args..., (char*) NULL);

            exit(1);
            break ;

            return new TwoPipe(-1, -1);
          }
    }

    close(outfd[0]);
    close(infd [1]);

    return new TwoPipe(outfd[1], infd[0]);
}