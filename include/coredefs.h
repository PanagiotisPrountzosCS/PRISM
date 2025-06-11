#ifndef __COREDEFS_H
#define __COREDEFS_H

#include <stdint.h>

#include <string>

namespace PRISM
{

typedef struct measurement
{
        float x;
        float y;
        float z;
        float timestamp;
} measurement;

template <typename T>
struct point
{
        T x;
        T y;
};

typedef struct options
{
        std::string host;
        std::string topic;
        std::string username;
        std::string password;
        std::string cert_path;
        uint32_t port;
} options;

#pragma pack(push, 1)
typedef struct message
{
        float x;
        float y;
        float z;
        uint32_t id;
        uint32_t timestamp;
} message;
#pragma pack(pop)

}  // namespace PRISM

#endif