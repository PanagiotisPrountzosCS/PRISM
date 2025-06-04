#ifndef __COREDEFS_H
#define __COREDEFS_H

#include <stdint.h>

typedef struct message
{
        float x;
        float y;
        float z;
        uint32_t id;
        uint32_t timestamp;
} message;

#endif