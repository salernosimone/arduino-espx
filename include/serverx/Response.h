#pragma once

#include <functional>
#include <WiFi.h>
#include "./Request.h"

namespace espx::serverx {
    /**
     * Reponse generator
     */
    class Response {
    public:
        Request *request;

        /**
         * Constructor
         * @param request_
         */
        Response(Request *request_) :
                request(request_),
                beforeBodyCallback(NULL) {

        }

        /**
         * Send ok status
         */
        void ok() {
            println("HTTP/1.1 200 OK");
        }

        /**
         * Send ok status
         */
        void ok(const String &message) {
            ok();
            plain(message);
        }

        /**
         * Send server error status
         */
        void serverError(const String &message) {
            println("HTTP/1.1 500 SERVER ERROR");
            plain(message);
        }

        /**
         * Send 404 status
         */
        void notFound() {
            println("HTTP/1.1 404 Not Found");
        }

        /**
         * Print header
         * @param name
         * @param value
         */
        void header(String name, Str value) {
            header(name, value.value());
        }

        /**
         * Print header
         * @tparam T
         * @param name
         * @param value
         */
        template<typename T>
        void header(String name, T value) {
            println(str(name).concat(": ", value));
        }

        /**
         * Print header
         * @tparam T
         * @param name
         * @param value
         */
        void header(String name, const char *value) {
            header(name, String(value));
        }

        /**
         * Enable CORS requests
         */
        void cors() {
            header("Access-Control-Allow-Origin", "*");
        }

        /**
         * Return plain text
         * @param text
         */
        void plain(const String &text) {
            header("Content-Type", "text/plain");
            body(text);
        }

        /**
         * Send JPEG image
         * @param data
         * @param length
         */
        void jpeg(uint8_t *data, const size_t length) {
            header("Content-Type", "image/jpeg");
            body(data, length);
        }

        /**
         *
         * @param contentType
         * @param contents
         * @param length
         */
        void gzip(String contentType, const uint8_t *contents, size_t length) {
            header("Content-Type", contentType);
            header("Connection", "close");
            header("Content-Encoding", "gzip");
            body((uint8_t *) contents, length);
        }

        /**
         * Send string body
         * @param body
         */
        void body(const String &body) {
            header("Content-Length", body.length());
            runBeforeBodyCallback();
            print(body);
        }

        /**
         * Send binary body
         * @param body
         */
        void body(uint8_t *body, const size_t length) {
            header("Content-Length", length);
            runBeforeBodyCallback();
            request->client->write(body, length);
        }

        /**
         * Start on-demand body
         */
        void body() {
            runBeforeBodyCallback();
        }

        /**
         * Call function before sending body
         * @param callback 
         */
        void beforeBody(std::function<void(Response *)> callback) {
            beforeBodyCallback = callback;
        }

        /**
         * Print Str
         * @param value
         */
        void print(Str s) {
            print(s.value());
        }

        /**
         * Print
         * @tparam T
         * @param value
         */
        template<typename T>
        void print(T value) {
            // flush pending request, if any
            request->flush();
            request->client->print(value);
        }

        /**
         * Println
         * @tparam T
         * @param value
         */
        template<typename T>
        void println(T value) {
            print(value);
            println();
        }

        /**
         * Println
         */
        void println() {
            print("\r\n");
        }

    protected:
        std::function<void(Response *)> beforeBodyCallback;

        /**
         * Call before body callback
         */
        void runBeforeBodyCallback() {
            if (beforeBodyCallback != NULL)
                beforeBodyCallback(this);

            println();
        }
    };
}
