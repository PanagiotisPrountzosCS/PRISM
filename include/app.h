#ifndef __APP_H
#define __APP_H

#include <memory>
#include <unordered_map>

#include "sensor.h"

struct mosquitto;

namespace PRISM
{
struct queue_context;

void start_loop(mosquitto* mosq, queue_context& qc);

void poll_message_queue(
    queue_context& qc,
    std::unordered_map<uint32_t, std::shared_ptr<sensor>>& sensor_map);

void sleep_ms(uint32_t duration);

void export_and_clear(sensor s, uint32_t id);

#if __APP_MODE == 1
void hello();
#endif

#if __APP_MODE == 2
void hello();
#endif
}  // namespace PRISM

#endif