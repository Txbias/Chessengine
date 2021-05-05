#include "logger.h"

void Logger::debug(std::string s) {
    std::ofstream file;

    file.open("debug.log", std::ios::app);
    file << s << std::endl;
    file.close();
}
