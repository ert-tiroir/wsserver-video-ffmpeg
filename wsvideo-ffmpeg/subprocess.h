
/**********************************************************************************/
/* wsvideo-ffmpeg/subprocess.h                                                    */
/*                                                                                */
/* This file contains the details on how to create a subprocess with a            */
/* bi-directional pipe attached to the fork                                       */
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

#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/prctl.h>
#include <sys/signal.h>

/**
 * Return the path of an executable
 * 
 * @param program the name of the executable
 * @return its path
 */
std::string which (std::string program);

/**
 * Create a subprocess and return the bi directional pipe
 * 
 * @param program the name of the program
 * @param args a list of const char* representing the arguments of the program
 * @return the bi-directional pipe for this subprocess
 */
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