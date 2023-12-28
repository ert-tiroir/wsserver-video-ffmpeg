
/**********************************************************************************/
/* wsvideo-ffmpeg/subprocess.cpp                                                  */
/*                                                                                */
/* This file contains the details on how to find the path of an executable        */
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

#include <wsvideo-ffmpeg/subprocess.h>

/**
 * Return the path of an executable
 * 
 * @param program the name of the executable
 * @return its path
 */
std::string which (std::string program) {
    std::string whichCommand = "which " + program;
    FILE* file = popen(whichCommand.c_str(), "r");

    const int BUFFER_SIZE = 1024;
    char buffer[BUFFER_SIZE];
    size_t size = fread(buffer, sizeof(*buffer), BUFFER_SIZE, file);
    fclose(file);

    if (size == -1) {
        std::cerr << "Warning, could not use the command \"" << whichCommand << "\".\n";
        return program;
    }
    // which always contains a '\n' at the end that we don't want
    size --;

    std::string result(size, '.');
    for (int i = 0; i < size; i ++) result[i] = buffer[i];

    if (size == BUFFER_SIZE - 1) {
        std::cerr << "Warning, the path of the program might have exceeded the buffer size.\n";
        std::cerr << "Input command : " << whichCommand << std::endl;
        std::cerr << "Output path : " << result << std::endl;
    }
    return result;
}
