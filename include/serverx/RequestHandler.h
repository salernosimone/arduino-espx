#pragma once
#include <WiFi.h>
#include "./Request.h"

namespace espx::serverx {
    template<class Handler>
    class RequestHandler {
    public:

        /**
         * Act as a function
         * @param userdata
         */
        void operator()(void *userdata) {
            Handler *handler = (Handler*) userdata;

            while (true) {
                NetworkClient client = handler->server.accept();
                Request request(&client);

                if (!client || !client.available()) {
                    delay(10);
                    continue;
                }

                request.parseHeadline();

                if (request.method.length() < 3) {
                    ESP_LOGE("Serverx", "Malformed request");
                    continue;
                }

                ESP_LOGD("Serverx", "Handling %s request to %s%s", request.method, request.path, request.queryString);
                handler->handleRequest(&request);
                request.flush();
                ESP_LOGD("Serverx", "Handled %s request to %s%s", request.method, request.path, request.queryString);
            }
        }

    protected:
    };
}