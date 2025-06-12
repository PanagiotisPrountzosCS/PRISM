#ifndef __APP_H
#define __APP_H

#include <memory>
#include <unordered_map>

#include "sensor.h"

struct mosquitto;
#if __APP_MODE == 2

struct SDL_Window;

#endif

namespace PRISM
{
struct queue_context;

void start_loop(mosquitto* mosq, queue_context& qc);

void poll_message_queue(
    queue_context& qc,
    std::unordered_map<uint32_t, std::shared_ptr<sensor>>& sensor_map);

void sleep_ms(uint32_t duration);

void export_and_clear(sensor s, uint32_t id);

#if __APP_MODE == 2

typedef struct gui_context
{
        SDL_Window* window;
        void* gl_context;
} gui_context;

void init_sdl(gui_context& gc);

void poll_sdl_events(volatile bool& should_run);

void generate_next_frame(
    gui_context& gc,
    std::unordered_map<uint32_t, std::shared_ptr<sensor>>& sensor_map);

void clear_sdl(gui_context& gc);

#endif
}  // namespace PRISM

#endif