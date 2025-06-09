#ifndef __APP_MODE
#define __APP_MODE 1
#endif
#ifndef __BUILD_MODE
#define __BUILD_MODE 1
#endif

#include <iostream>

#include "extras.h"
#include "mosquitto.h"
#include "mqtt_client.h"
#include "options_parser.h"
#include "unistd.h"

int main(int argc, char** argv)
{
        PRISM::options opts;
        if (!validate_opts(argc, argv, opts))
        {
                std::cout << "invalid arguments, use -h for help\n";
                exit(EXIT_FAILURE);
        }

        struct mosquitto* mosquitto_client;
        PRISM::queue_context message_q;
        PRISM::init_queue(&message_q);
        if (!init_mqtt(opts, &mosquitto_client, &message_q))
        {
                std::cout
                    << "failed to initialize mosquitto client, aborting\n";
                exit(EXIT_FAILURE);
        }

        // start_loop(mosquitto_client, message_q);
        mosquitto_loop_start(mosquitto_client);

        while (1)
        {
                std::cout << message_q.queue->size() << '\n';
                sleep(1);
        }

        mosquitto_loop_stop(mosquitto_client, true);

        PRISM::cleanup_queue(&message_q);
        PRISM::cleanup_mqtt(mosquitto_client);

        return 0;
}