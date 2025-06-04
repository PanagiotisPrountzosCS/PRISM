#ifndef __APP_MODE
#define __APP_MODE 1
#endif
#ifndef __BUILD_MODE
#define __BUILD_MODE 1
#endif

#include <iostream>

#include "extras.h"
#include "options_parser.h"

int main(int argc, char** argv)
{
        options opts;
        if (!validate_opts(argc, argv, opts))
        {
                std::cout << "invalid arguments, use -h for help\n";
                exit(EXIT_FAILURE);
        }

        return 0;
}