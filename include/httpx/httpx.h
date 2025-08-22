#pragma once

#include <functional>
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <NetworkClientSecure.h>
#include "./Response.h"

using espx::httpx::HttpxResponse;
using HttpxHandler = std::function<int(NetworkClientSecure *networkClient, HTTPClient *httpClient)>;


/**
 * Make HTTP requests
 */
class Httpx {
public:
    NetworkClientSecure networkClient;
    HTTPClient httpClient;
    HttpxResponse response;

    /**
     * Constructor
     */
    Httpx() : responseCode(0) {

    }

    /**
     * End connection
     */
    inline void end() {
        httpClient.end();
    }

    /**
     *
     * @param url
     * @return
     */
    template<typename... Args>
    HttpxResponse& run(String url, HttpxHandler handler, Args... handlers) {
        responseCode = 0;
        end();

        if (!httpClient.begin(networkClient, url))
            return response.connectionFailed(url);

        handleConfig(handler);
        handleConfig(handlers...);

        return response.pipe(httpClient, responseCode);
    }

    /**
     *
     * @return
     */
    HttpxHandler Insecure() {
        return [](NetworkClientSecure *networkClient, HTTPClient *httpClient) {
            networkClient->setInsecure();
            return 0;
        };
    }

    /**
     *
     * @return
     */
    HttpxHandler Cert(const char *cert) {
        return [cert](NetworkClientSecure *networkClient, HTTPClient *httpClient) {
            networkClient->setCACert(cert);
            return 0;
        };
    }

    /**
     *
     * @return
     */
     template<typename T>
    HttpxHandler Header(String name, T value) {
        return [name, value](NetworkClientSecure *networkClient, HTTPClient *httpClient) {
            httpClient->addHeader(name, String(value));
            return 0;
        };
    }

    /**
     *
     * @return
     */
    HttpxHandler ConnectionTimeout(const Str timeout) {
        return [timeout](NetworkClientSecure *networkClient, HTTPClient *httpClient) {
            httpClient->setConnectTimeout(timeout.millis());
            return 0;
        };
    }

    /**
     *
     * @return
     */
    HttpxHandler RequestTimeout(const Str timeout) {
        return [timeout](NetworkClientSecure *networkClient, HTTPClient *httpClient) {
            httpClient->setTimeout(timeout.millis());
            return 0;
        };
    }

    /**
     *
     * @return
     */
    HttpxHandler RequestTimeout(String timeout) {
        return RequestTimeout(str(timeout));
    }

    /**
     *
     * @return
     */
    HttpxHandler Body(String method) {
        return Body(method, (uint8_t*) NULL, 0);
    }

    /**
     *
     * @return
     */
    HttpxHandler Body(String method, const char *body) {
        return [method, body](NetworkClientSecure *networkClient, HTTPClient *httpClient) {
            return httpClient->sendRequest(method.c_str(), body);
        };
    }

    /**
     *
     * @return
     */
    HttpxHandler Body(String method, String body) {
        return Body(method, body.c_str());
    }

    /**
     *
     * @return
     */
    HttpxHandler Body(String method, uint8_t *buffer, size_t length) {
        return [method, buffer, length](NetworkClientSecure *networkClient, HTTPClient *httpClient) {
            return httpClient->sendRequest(method.c_str(), buffer, length);
        };
    }

protected:
    int responseCode;

    void handleConfig() {

    }

    /**
     *
     * @param handler
     */
    void handleConfig(HttpxHandler handler) {
        responseCode = handler(&networkClient, &httpClient);
    }

    /**
     *
     * @param handler
     * @param other
     */
    template<typename... Args>
    void handleConfig(HttpxHandler handler, Args... other) {
        handleConfig(handler);
        handleConfig(other...);
    }
};

static Httpx httpx;