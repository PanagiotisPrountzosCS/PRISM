#ifndef __COREDEFS_H
#define __COREDEFS_H

#include <stdint.h>

#include <string>

#define TICK_MS 25

namespace PRISM
{

typedef struct options
{
        std::string host;
        std::string topic;
        std::string username;
        std::string password;
        std::string cert_path;
        uint32_t port;
} options;

typedef struct message
{
        float x;
        float y;
        float z;
        uint32_t id;
        uint32_t timestamp;
} message;

}  // namespace PRISM

#endif