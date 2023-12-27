#include <wsvideo-ffmpeg/subprocess.h>

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
