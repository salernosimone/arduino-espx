#pragma once
#include <esp_camera.h>

typedef struct {
    const char name[16];
    framesize_t framesize;
    uint16_t width;
    uint16_t height;
} CamxResolution;

const uint8_t CAMX_RESOLUTIONS_COUNT = 23;
const CamxResolution CAMX_RESOLUTIONS[] = {
        {"YOLO", FRAMESIZE_96X96, 96, 96},
        {"QQVGA", FRAMESIZE_QQVGA, 160, 120},
        {"QCIF", FRAMESIZE_QCIF, 176, 144},
        {"HQVGA", FRAMESIZE_HQVGA, 240, 176},
        {"SQUARE", FRAMESIZE_240X240, 240, 240},
        {"QVGA", FRAMESIZE_QVGA, 320, 240},
        {"CIF", FRAMESIZE_CIF, 400, 296},
        {"HVGA", FRAMESIZE_HVGA, 480, 320},
        {"VGA", FRAMESIZE_VGA, 640, 480},
        {"SVGA", FRAMESIZE_SVGA, 800, 600},
        {"XGA", FRAMESIZE_XGA, 1024, 768},
        {"HD", FRAMESIZE_HD, 1280, 720},
        {"SXGA", FRAMESIZE_SXGA, 1280, 1024},
        {"UXGA", FRAMESIZE_UXGA, 1600, 1200},
        {"FHD", FRAMESIZE_FHD, 1920, 1080},
        {"P_HD", FRAMESIZE_P_HD, 720, 1280},
        {"P_3MP", FRAMESIZE_P_3MP, 864, 1536},
        {"QXGA", FRAMESIZE_QXGA, 2048, 1536},
        {"QHD", FRAMESIZE_QHD, 2560, 1440},
        {"WQXGA", FRAMESIZE_WQXGA, 2560, 1600},
        {"P_FHD", FRAMESIZE_P_FHD, 1080, 1920},
        {"QSXGA", FRAMESIZE_QSXGA, 2560, 1920}
};

namespace espx::camx {
    class Resolution {
    public:
        framesize_t framesize;
        uint16_t width;
        uint16_t height;

        /**
         * Constructor
         */
        Resolution() {
            // default to VGA
            vga();
        }

        /**
         * Set custom resolution
         * @param width
         * @param height
         */
        void custom(uint16_t width, uint8_t height) {
            set(FRAMESIZE_INVALID, width, height);
        }

        /**
         *
         * @param framesize
         * @param width
         * @param height
         */
        void set(framesize_t framesize, uint16_t width, uint16_t height) {
            this->framesize = framesize;
            this->width = width;
            this->height = height;
        }

        /**
         * Set resolution from const char*
         * @param resolution
         */
        void set(const char *resolution) {
            String res(resolution);

            set(res);
        }

        /**
         * Set resolution from string
         * @param resolution
         */
        void set(String& resolution) {
            for (uint8_t i = 0; i < CAMX_RESOLUTIONS_COUNT; i++) {
                const CamxResolution *res = &(CAMX_RESOLUTIONS[i]);

                if (resolution == res->name) {
                    framesize = res->framesize;
                    width = res->width;
                    height = res->height;

                    return;
                }
            }

            Serial.printf("Unknown resolution: %s", resolution.c_str());
        }

        /**
         *
         */
        void prompt() {
            String choices[CAMX_RESOLUTIONS_COUNT];

            for (uint8_t i = 0; i < CAMX_RESOLUTIONS_COUNT; i++)
                choices[i] = (&(CAMX_RESOLUTIONS[i]))->name;

            const int choice = promptChoice("Select a resolution", choices, CAMX_RESOLUTIONS_COUNT);
            const String resolution = choices[choice];

            remember(resolution.c_str());
        }

        inline void yolo() { set("YOLO"); }

        inline void qqvga() { set("QQVGA"); }

        inline void qcif() { set("QCIF"); }

        inline void hqvga() { set("HQVGA"); }

        inline void square() { set("SQUARE"); }

        inline void qvga() { set("QVGA"); }

        inline void cif() { set("CIF"); }

        inline void hvga() { set("HVGA"); }

        inline void vga() { set("VGA"); }

        inline void svga() { set("SVGA"); }

        inline void xga() { set("XGA"); }

        inline void hd() { set("HD"); }

        inline void sxga() { set("SXGA"); }

        inline void uxga() { set("UXGA"); }

        inline void fhd() { set("FHD"); }

        inline void p_hd() { set("P_HD"); }

        inline void p_3mp() { set("P_3MP"); }

        inline void qxga() { set("QXGA"); }

        inline void qhd() { set("QHD"); }

        inline void wqxga() { set("QWXGA"); }

        inline void p_fhd() { set("P_FHD"); }

        inline void qsxga() { set("QSXGA"); }

    protected:
        /**
         * Show the user a message to skip prompting in the future
         * @param resolution
         */
        void remember(const String& resolution) {
            Serial.printf("You can skip prompting next time with `camx.resolution.%s()`\n", str(resolution).lower().c_str());
            set(resolution.c_str());
        }
    };
}