#ifndef __OPTIONS_PARSER_H
#define __OPTIONS_PARSER_H

#include <stdint.h>

#include <string>

#include "cxxopts.hpp"

typedef struct options
{
        std::string host;
        std::string topic;
        std::string username;
        std::string password;
        uint32_t port;
} options;

bool validate_opts(int argc, char** argv, options& options);

#endif