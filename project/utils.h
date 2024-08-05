#ifndef SERVER_FOR_OSCILLOSCOPE_UTILS_H_
#define SERVER_FOR_OSCILLOSCOPE_UTILS_H_

#include <iostream>
#include <tuple>
#include <string>

std::tuple<std::string, std::string, int> readArg( int argc, char* argv[] );

#endif   // SERVER_FOR_OSCILLOSCOPE_UTILS_H_
