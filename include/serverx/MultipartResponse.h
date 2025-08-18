#pragma once

#include "./Response.h"

namespace espx::serverx {
    /**
     * Multi-part response
     */
    class MultipartResponse : public Response {
    public:

        /**
         * Constructor
         * @param request
         */
        MultipartResponse(Request *request) :
                Response(request),
                started(false),
                sof("multipartframe") {

        }

        /**
         * Start multipart response
         */
        void start() {
            ok();
            cors();
            header("Content-Type", str("multipart/x-mixed-replace;boundary=").concat(sof));
            header("Access-Control-Allow-Origin", "*");
            body();
            println(str("--").concat(sof));

            started = true;
        }

        /**
         * Send frame
         * @param data
         * @param length
         * @param contentType
         */
        void frame(uint8_t *data, size_t length, String contentType) {
            if (!started)
                start();

            header("Content-Type", contentType);
            body(data, length);
            println();
            println(str("--").concat(sof));
        }

    protected:
        String sof;
        bool started;
    };
}
