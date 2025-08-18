#pragma once
#include <WiFi.h>
#include "./Str.h"
#include "./OpStatus.h"
#include "./threadx.h"

namespace espx {
    /**
     * Base class for HTTP servers
     */
    class Serverx : public HasOpStatus {
    public:
        ::NetworkServer server;

        /**
         * Constructor
         */
        Serverx() {
            sconfig.port = 80;
        }

        /**
         *
         * @param port
         */
        void listenOn(uint16_t port) {
            sconfig.port = port;
        }

    protected:
        String ip;
        TaskHandle_t taskHandle;
        struct {
            uint16_t port;
        } sconfig;

        /**
         * Get server name
         * (used as the identifier of the thread)
         * @return
         */
        virtual String name() = 0;

        /**
         *
         * @return
         */
        virtual ThreadxTask handler() = 0;

        /**
         * Get stack size for the thread
         * @return
         */
        virtual String stack() {
            return "8k";
        }

        /**
         *
         */
        void begin() {
            ip = WiFi.localIP().toString();

            server.begin(sconfig.port);

            // accept requests in a thread
            threadx(
                handler(),
                threadx.Name(name()),
                threadx.Core(1),
                threadx.Stack(stack()),
                threadx.Important(),
                threadx.Userdata(this),
                threadx.Handler(taskHandle)
            );
        }
    };
}