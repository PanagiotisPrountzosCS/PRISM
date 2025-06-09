// WHY THE FUCK DOES CLANG FORMAT PUT STDLIB INBETWEEN MY OWN FUCKING
// FILESSSSS??????????????
// caps, retard
#include "app.h"

#include <chrono>
#include <csignal>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <thread>

#include "mosquitto.h"
#include "mqtt_client.h"

#define TICK_MS 50

namespace
{

volatile std::sig_atomic_t should_run = 1;

void sigint_handler(int) { should_run = 0; }

const size_t max_vector_size = 10;

}  // namespace

namespace PRISM
{

void poll_message_queue(
    queue_context& qc,
    std::unordered_map<uint32_t, std::shared_ptr<sensor>>& sensor_map)
{
        std::lock_guard<std::mutex> lock(*(qc.mutex));
        while (!qc.queue->empty())
        {
                message m = qc.queue->front();
                // look for the messages id in the map. if it doesnt exist, make
                // a new sensor;
                if (!sensor_map.count(m.id))
                {
                        sensor new_sensor{m.id, {}};
                        sensor_map.insert(
                            {m.id, std::make_shared<sensor>(new_sensor)});
                }
                measurement new_point{m.x, m.y, m.z, m.timestamp};

                // before pushing back, we should probably clear if the vector
                // has a size greater than a fixed constant
                if (sensor_map[m.id]->data.size() > max_vector_size)
                        export_and_clear(sensor_map[m.id]->data, m.id);
                std::cout << sensor_map[m.id]->data.size();

                sensor_map[m.id]->data.push_back(new_point);

                // fuckass logging but I really need to submit this shit + a
                // thesis in 4 days lol
                std::cout << "Sensor#" << m.id << " (" << m.x << " " << m.y
                          << " " << m.z << ") @ " << m.timestamp << "\n";
                qc.queue->pop();
        }
}

void sleep_ms(uint32_t duration)
{
        std::this_thread::sleep_for(std::chrono::milliseconds(duration));
}

void start_loop(mosquitto* mosq, queue_context& qc)
{
        std::signal(SIGINT, sigint_handler);
        // #if __APP_MODE == 2
        // init_gui();
        // #endif

        std::unordered_map<uint32_t, std::shared_ptr<sensor>> sensor_map;
        mosquitto_loop_start(mosq);

        while (should_run)
        {
                // #if __APP_MODE == 2
                // poll_sdl_events();
                // #endif

                poll_message_queue(qc, sensor_map);
                sleep_ms(TICK_MS);

                // #if __APP_MODE == 2
                // render_frame();
                // #endif
        }

        std::cout << "\nExiting...\n";
        for (auto& [id, sensor_ptr] : sensor_map)
        {
                if (!sensor_ptr->data.empty())
                {
                        export_and_clear(sensor_ptr->data, id);
                }
        }

        mosquitto_loop_stop(mosq, true);
}

void export_and_clear(std::vector<measurement>& vec, uint32_t id)
{
        static std::string log_filename;
        if (log_filename.empty())
        {
                std::filesystem::create_directories("./logs");
                // get unixtime at first call
                std::time_t now = std::time(nullptr);
                log_filename = "./logs/run_" + std::to_string(now) + ".txt";
        }

        std::ofstream ofs(log_filename, std::ios::app);
        if (!ofs)
        {
                std::cerr << "Failed to open log file: " << log_filename
                          << "\n";
                return;
        }

        for (const auto& m : vec)
        {
                ofs << id << " " << m.x << " " << m.y << " " << m.z << " "
                    << m.timestamp << "\n";
        }
        ofs.close();
        vec.clear();
}

#if __APP_MODE == 1
void hello() { std::cout << "clearly cli mode\n"; }
#endif

#if __APP_MODE == 2
void hello() { std::cout << "clearly gui mode\n"; }
#endif
}  // namespace PRISM