#ifndef __MQTT_CLIENT_H
#define __MQTT_CLIENT_H

#include <mutex>
#include <queue>

#include "coredefs.h"

struct mosquitto;

namespace PRISM
{
typedef struct queue_context
{
        std::queue<message>* queue = nullptr;
        std::mutex* mutex = nullptr;
} queue_context;

void init_queue(queue_context* qc);

void cleanup_queue(queue_context* qc);

bool init_mqtt(const options& opts, mosquitto** mosq, queue_context* qc);

void cleanup_mqtt(mosquitto* mosq);
}  // namespace PRISM

#endif