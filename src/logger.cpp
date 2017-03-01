#include "logger.h"

//std::ofstream Log;

void InitLogger()
{
    /*
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    Log.open(std::string(cwd) + "/instux.log", std::ofstream::app);
    Log << "Logger::InitLogger" << std::endl;
    */
}

std::string hex(unsigned input)
{
    std::stringstream ss;
    ss << std::hex << input;
    return std::string(ss.str());
}

std::string hex(void* input)
{
    return hex((unsigned) input);
}

std::string hex0(unsigned input)
{
    return std::string("0x") + hex(input);
}

std::string hex0(void* input)
{
    return std::string("0x") + hex(input);
}
