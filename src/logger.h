#pragma once

#include <fstream>
#include <unistd.h>
#include <iomanip>

using std::endl;

extern std::ofstream Log;

void InitLogger();
std::string hex(unsigned input);
std::string hex(void* input);
std::string hex0(unsigned input);
std::string hex0(void* input);
