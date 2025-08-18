#pragma once

#include <WiFi.h>
#define MAX_HTTP_HEADERS 20

namespace espx::serverx {
    /**
     * HTTP request parser
     */
    class Request {
    public:
        NetworkClient *client;
        String method;
        String path;
        String queryString;
        struct {
            String key;
            String value;
        } headers[MAX_HTTP_HEADERS];

        /**
         * Constructor
         * @param client_
         */
        Request(NetworkClient *client_) :
                client(client_),
                method(""),
                path(""),
                queryString("") {

        }

        /**
         * Parse HTTP headline
         */
        void parseHeadline() {
            String line = client->readStringUntil('\n');

            // method
            int16_t split = line.indexOf(' ');

            if (split < 0) {
                ESP_LOGE("Request", "Can't detect request method");
                return;
            }

            method = line.substring(0, split);
            line = line.substring(split + 1);

            // path + query string
            split = line.indexOf(' ');

            if (split < 0) {
                ESP_LOGE("Request", "Can't detect request path");
                return;
            }

            path = line.substring(0, split);
            split = path.indexOf('?');

            if (split > -1) {
                queryString = path.substring(split + 1);
                path = path.substring(0, split);
            }

            if (path.endsWith("/"))
                path = path.substring(0, -1);
        }

        /**
         * Consume client data
         */
        void flush() {
            while (client->available())
                client->read();

            client->clear();
        }

        /**
         * Get query param
         * @param name
         * @return
         */
        String getQueryParam(String name) {
            int16_t startIndex = queryString.indexOf(name + "=");

            if (startIndex < 0)
                return "";

            int16_t endIndex = queryString.indexOf("&", startIndex + name.length());

            return queryString.substring(startIndex + name.length() + 1,
                                         endIndex > -1 ? endIndex : queryString.length());
        }
    };
}