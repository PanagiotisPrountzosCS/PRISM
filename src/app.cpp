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

#if __APP_MODE == 2
// gui includes
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl2.h"
#include "implot.h"

#endif

#define TICK_MS 10

namespace
{

volatile bool should_run = 1;

void sigint_handler(int) { should_run = 0; }

constexpr size_t max_vector_size = 10000;

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
                        sensor_map.insert(
                            {m.id, std::make_shared<sensor>(m.id)});
                }
                measurement new_point{m.x, m.y, m.z,
                                      static_cast<float>(m.timestamp)};

                // before pushing back, we should probably clear if the vector
                // has a size greater than a fixed constant
                if (sensor_map[m.id]->data->size() > max_vector_size)
                        export_and_clear(*sensor_map[m.id], m.id);

                sensor_map[m.id]->data->push_back(new_point);
                sensor_map[m.id]->predictor->step_window();

                // fuckass logging but I really need to submit this shit + a
                // thesis in 4 days lol
                std::cout << "Sensor#" << m.id << " (" << m.x << " " << m.y
                          << " " << m.z << ") @ " << m.timestamp * 1.0 / 1000
                          << " x_a = " << sensor_map[m.id]->predictor->get_x_a()
                          << " y_a = " << sensor_map[m.id]->predictor->get_y_a()
                          << " z_a = " << sensor_map[m.id]->predictor->get_z_a()
                          << " x_b = " << sensor_map[m.id]->predictor->get_x_b()
                          << " y_b = " << sensor_map[m.id]->predictor->get_y_b()
                          << " z_b = " << sensor_map[m.id]->predictor->get_z_b()
                          << '\n';
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

        std::unordered_map<uint32_t, std::shared_ptr<sensor>> sensor_map;
        mosquitto_loop_start(mosq);
#if __APP_MODE == 2
        gui_context gc;
        PRISM::init_sdl(gc);
#endif

        while (should_run)
        {
                poll_message_queue(qc, sensor_map);

#if __APP_MODE == 2
                poll_sdl_events(should_run);
                generate_next_frame(gc, sensor_map);
#endif

                sleep_ms(TICK_MS);
        }

#if __APP_MODE == 2
        clear_sdl(gc);
#endif

        std::cout << "\nExiting...\n";
        for (auto& [id, sensor_ptr] : sensor_map)
        {
                if (!sensor_ptr->data->empty())
                {
                        export_and_clear(*sensor_ptr, id);
                }
        }

        mosquitto_loop_stop(mosq, true);
}

void export_and_clear(sensor s, uint32_t id)
{
        auto vec = s.data;
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
                std::cout << "Failed to open log file: " << log_filename
                          << "\n";
                return;
        }

        for (const auto& m : *vec)
        {
                ofs << id << " " << m.x << " " << m.y << " " << m.z << " "
                    << m.timestamp << "\n";
        }
        ofs.close();
        vec->clear();
        s.predictor->clear();
}

#if __APP_MODE == 2

void init_sdl(gui_context& gc)
{
        SDL_Init(SDL_INIT_VIDEO);
        gc.window =
            SDL_CreateWindow("Live Sinewave Plot", SDL_WINDOWPOS_CENTERED,
                             SDL_WINDOWPOS_CENTERED, 800, 600,
                             SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
        gc.gl_context = SDL_GL_CreateContext(gc.window);
        SDL_GL_MakeCurrent(gc.window, gc.gl_context);
        SDL_GL_SetSwapInterval(1);

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImPlot::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        (void)io;
        ImGui::StyleColorsDark();

        ImGui_ImplSDL2_InitForOpenGL(gc.window, gc.gl_context);
        ImGui_ImplOpenGL3_Init("#version 130");
}

void poll_sdl_events(volatile bool& should_run)
{
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
                ImGui_ImplSDL2_ProcessEvent(&event);
                if (event.type == SDL_QUIT) should_run = false;
        }
}

void generate_next_frame(
    gui_context& gc,
    std::unordered_map<uint32_t, std::shared_ptr<sensor>>& sensor_map)
{
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGuiIO& io = ImGui::GetIO();
        ImGui::SetNextWindowSize(io.DisplaySize);
        ImGui::Begin("MainWindow", nullptr,
                     ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                         ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
                         ImGuiWindowFlags_NoSavedSettings |
                         ImGuiWindowFlags_NoScrollbar);

        static std::string selected_signal;
        static uint32_t selected_id = -1;
        static char selected_orientation =
            'a';  // default value, used for debugging
        std::vector<std::string> widget_options = {"0x", "42y"};
        for (const auto& pair : sensor_map)
        {
                auto x = std::to_string(pair.first) + 'x';
                auto y = std::to_string(pair.first) + 'y';
                auto z = std::to_string(pair.first) + 'z';
                widget_options.push_back(x);
                widget_options.push_back(y);
                widget_options.push_back(z);
        }
        ImGui::BeginChild("TabWidget", ImVec2(150, 0), true);
        ImGui::Text("Select Signal:");
        ImGui::Separator();
        for (const auto& option : widget_options)
        {
                if (ImGui::Selectable(option.c_str(),
                                      selected_signal == option))
                {
                        selected_signal = option;
                        // completely hacky but anyway
                        selected_orientation = option.back();
                        selected_id = std::stoul(option);
                }
        }
        ImGui::EndChild();

        ImGui::SameLine();

        ImGui::BeginChild("PlotArea", ImVec2(0, 0), false);

        std::vector<float> x = {0};
        std::vector<float> y = {0};
        std::shared_ptr<std::vector<measurement>> data;

        if (selected_id != -1 && selected_orientation != 'a')
        {
                // data = sensor_map[selected_id]->data;
        }

        if (ImPlot::BeginPlot("PRISM", ImVec2(-1, -1)))
        {
                ImPlot::PlotLine("sin(x + t)", x.data(), y.data(), x.size());
                ImPlot::EndPlot();
        }

        ImGui::EndChild();

        ImGui::End();

        ImGui::Render();
        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(gc.window);
}

void clear_sdl(gui_context& gc)
{
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImPlot::DestroyContext();
        ImGui::DestroyContext();
        SDL_GL_DeleteContext(gc.gl_context);
        SDL_DestroyWindow(gc.window);
        SDL_Quit();
}

#endif

}  // namespace PRISM