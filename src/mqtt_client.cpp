#include "mqtt_client.h"

#include <string.h>

#include <iostream>

#include "coredefs.h"
#include "mosquitto.h"

namespace
{

void queue_inserter_callback(mosquitto* mosq, void* userdata,
                             const mosquitto_message* msg)
{
        (void)mosq;  // quit bitching compiler
        auto qc = static_cast<PRISM::queue_context*>(userdata);
        if (!qc || !qc->mutex || !qc->queue)
        {
                std::cout << "bad queue passed\n";
                exit(EXIT_FAILURE);
        }

        size_t message_count = msg->payloadlen / sizeof(PRISM::message);
        if (msg->payloadlen % sizeof(PRISM::message) != 0)
        {
                std::cout << "bad message size, skipping\n";
                return;
        }
        PRISM::message msg_array[message_count];
        memcpy(msg_array, msg->payload, msg->payloadlen);

        std::lock_guard<std::mutex> lock(*(qc->mutex));

        for (size_t i = 0; i < message_count; i++)
        {
                qc->queue->push(msg_array[i]);
        }
        // destroying the lock frees the mutex :p
        // I believe this should be everything
}

}  // namespace

namespace PRISM
{
void init_queue(queue_context* qc)
{
        cleanup_queue(qc);
        qc->queue = new std::queue<message>;
        qc->mutex = new std::mutex;
}

void cleanup_queue(queue_context* qc)
{
        delete qc->queue;
        delete qc->mutex;
        qc->queue = nullptr;
        qc->mutex = nullptr;
}

bool init_mqtt(const options& opts, mosquitto** mosq, queue_context* qc)
{
        mosquitto_lib_init();

        *mosq = mosquitto_new(NULL, true, qc);
        if (!*mosq)
        {
                std::cout << "Failed to create mosquitto client\n";
                return false;
        }

        mosquitto_username_pw_set(*mosq, opts.username.c_str(),
                                  opts.password.c_str());

        // register the callback
        mosquitto_message_callback_set(*mosq, queue_inserter_callback);

        mosquitto_tls_set(*mosq, opts.cert_path.c_str(), NULL, NULL, NULL,
                          NULL);  // sys certs
        if (mosquitto_connect(*mosq, opts.host.c_str(), opts.port, 60) !=
            MOSQ_ERR_SUCCESS)
        {
                std::cout << "Failed to connect to host\n";
                return false;
        }

        mosquitto_subscribe(*mosq, NULL, opts.topic.c_str(), 0);

        return true;
}

void cleanup_mqtt(mosquitto* mosq)
{
        mosquitto_destroy(mosq);
        mosquitto_lib_cleanup();
}

}  // namespace PRISM