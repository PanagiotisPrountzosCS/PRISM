#ifndef __APP_MODE
#define __APP_MODE 1
#endif
#ifndef __BUILD_MODE
#define __BUILD_MODE 1
#endif

#if (__APP_MODE == 2)
// gui
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl2.h"
#include "implot.h"
#endif

#include <cxxopts.hpp>
#include <iostream>

int main(int argc, char** argv)
{
        (void)argc;
        (void)argv;
        std::cout << __APP_MODE << '\n' << __BUILD_MODE << '\n';
        return 0;
}