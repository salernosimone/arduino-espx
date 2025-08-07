#pragma once

typedef struct {
    String name;
    const char alias[12];
    int8_t d0;
    int8_t d1;
    int8_t d2;
    int8_t d3;
    int8_t d4;
    int8_t d5;
    int8_t d6;
    int8_t d7;
    int8_t xclk;
    int8_t pclk;
    int8_t vsync;
    int8_t href;
    int8_t sccb_sda;
    int8_t sccb_scl;
    int8_t pwdn;
    int8_t reset;
} CamxModel;

const uint8_t CAMX_MODELS_COUNT = 11;

const CamxModel CAMX_MODELS[CAMX_MODELS_COUNT] = {
        {
                .name = "AiThinker",
                .alias = "aithinker",
                .d0 = 5,
                .d1 = 18,
                .d2 = 19,
                .d3 = 21,
                .d4 = 36,
                .d5 = 39,
                .d6 = 34,
                .d7 = 35,
                .xclk = 0,
                .pclk = 22,
                .vsync = 25,
                .href = 23,
                .sccb_sda = 26,
                .sccb_scl = 27,
                .pwdn = 32,
                .reset = -1,
        },
        {
                .name = "XIAO",
                .alias = "xiao",
                .d0 = 15,
                .d1 = 17,
                .d2 = 18,
                .d3 = 16,
                .d4 = 14,
                .d5 = 12,
                .d6 = 11,
                .d7 = 48,
                .xclk = 10,
                .pclk = 13,
                .vsync = 38,
                .href = 47,
                .sccb_sda = 40,
                .sccb_scl = 39,
                .pwdn = -1,
                .reset = -1,
        },
        {
                .name = "M5",
                .alias = "m5",
                .d0 = 32,
                .d1 = 35,
                .d2 = 34,
                .d3 = 5,
                .d4 = 39,
                .d5 = 18,
                .d6 = 36,
                .d7 = 19,
                .xclk = 27,
                .pclk = 21,
                .vsync = 22,
                .href = 26,
                .sccb_sda = 25,
                .sccb_scl = 23,
                .pwdn = -1,
                .reset = 15,
        },
        {
                .name = "M5 Fish Eye",
                .alias = "m5fisheye",
                .d0 = 32,
                .d1 = 35,
                .d2 = 34,
                .d3 = 5,
                .d4 = 39,
                .d5 = 18,
                .d6 = 36,
                .d7 = 19,
                .xclk = 27,
                .pclk = 21,
                .vsync = 25,
                .href = 26,
                .sccb_sda = 22,
                .sccb_scl = 23,
                .pwdn = -1,
                .reset = 15,
        },
        {
                .name = "M5 Timer X",
                .alias = "m5timerx",
                .d0 = 32,
                .d1 = 35,
                .d2 = 34,
                .d3 = 5,
                .d4 = 39,
                .d5 = 18,
                .d6 = 36,
                .d7 = 19,
                .xclk = 27,
                .pclk = 21,
                .vsync = 22,
                .href = 26,
                .sccb_sda = 25,
                .sccb_scl = 23,
                .pwdn = -1,
                .reset = 15,
        },
        {
                .name = "ESP - EYE",
                .alias = "espeye",
                .d0 = 34,
                .d1 = 13,
                .d2 = 14,
                .d3 = 35,
                .d4 = 39,
                .d5 = 38,
                .d6 = 37,
                .d7 = 36,
                .xclk = 4,
                .pclk = 25,
                .vsync = 5,
                .href = 27,
                .sccb_sda = 18,
                .sccb_scl = 23,
                .pwdn = -1,
                .reset = -1,
        },
        {
                .name = "ESP - EYE S3",
                .alias = "espeyes3",
                .d0 = 11,
                .d1 = 9,
                .d2 = 8,
                .d3 = 10,
                .d4 = 12,
                .d5 = 18,
                .d6 = 17,
                .d7 = 16,
                .xclk = 15,
                .pclk = 13,
                .vsync = 6,
                .href = 7,
                .sccb_sda = 4,
                .sccb_scl = 5,
                .pwdn = -1,
                .reset = -1,
        },
        {
                .name = "WROVER",
                .alias = "wrover",
                .d0 = 4,
                .d1 = 5,
                .d2 = 18,
                .d3 = 19,
                .d4 = 36,
                .d5 = 39,
                .d6 = 34,
                .d7 = 35,
                .xclk = 21,
                .pclk = 22,
                .vsync = 25,
                .href = 23,
                .sccb_sda = 26,
                .sccb_scl = 27,
                .pwdn = -1,
                .reset = -1,
        },
        {
                .name = "WROOM S3",
                .alias = "wrooms3",
                .d0 = 11,
                .d1 = 9,
                .d2 = 8,
                .d3 = 10,
                .d4 = 12,
                .d5 = 18,
                .d6 = 17,
                .d7 = 16,
                .xclk = 15,
                .pclk = 13,
                .vsync = 6,
                .href = 7,
                .sccb_sda = 4,
                .sccb_scl = 5,
                .pwdn = -1,
                .reset = -1,
        },
        {
                .name = "TTGO Plus",
                .alias = "ttgoplus",
                .d0 = 34,
                .d1 = 13,
                .d2 = 26,
                .d3 = 35,
                .d4 = 39,
                .d5 = 38,
                .d6 = 37,
                .d7 = 36,
                .xclk = 4,
                .pclk = 25,
                .vsync = 5,
                .href = 27,
                .sccb_sda = 18,
                .sccb_scl = 23,
                .pwdn = -1,
                .reset = -1,
        },
        {
                .name = "TTGO PIR",
                .alias = "ttgopir",
                .d0 = 5,
                .d1 = 14,
                .d2 = 4,
                .d3 = 15,
                .d4 = 18,
                .d5 = 23,
                .d6 = 36,
                .d7 = 39,
                .xclk = 32,
                .pclk = 19,
                .vsync = 27,
                .href = 25,
                .sccb_sda = 13,
                .sccb_scl = 12,
                .pwdn = 26,
                .reset = -1,
        }
};

namespace espx::camx {
    /**
     * Camera models pin definitions
     */
    class Model {
    public:
        CamxModel *pinout;

        /**
         * Constructor
         */
        Model() : pinout(NULL) {

        }

        /**
         * Set model from const char*
         * @param model
         */
        void set(const char *model) {
            String mod(model);

            set(mod);
        }

        /**
         * Set model from string
         * @param alias
         */
        void set(String& alias) {
            for (uint8_t i = 0; i < CAMX_MODELS_COUNT; i++) {
                const CamxModel *model = &(CAMX_MODELS[i]);

                if (alias == model->alias) {
                    pinout = (CamxModel *) model;
                    return;
                }
            }

            Serial.printf("Unknown model: %s", alias.c_str());
        }

        /**
         *
         */
        void prompt() {
            String choices[CAMX_MODELS_COUNT];

            for (uint8_t i = 0; i < CAMX_MODELS_COUNT; i++)
                choices[i] = (&(CAMX_MODELS[i]))->name;

            const int choice = promptChoice("Select a resolution", choices, CAMX_MODELS_COUNT);
            const String alias = (&(CAMX_MODELS[choice]))->alias;

            remember(alias.c_str());
        }

        inline void aithinker() { set("aithinker"); }

        inline void xiao() { set("xiao"); }

        inline void wrooms3() { set("wrooms3"); }

        inline void ttgoplus() { set("ttgoplus"); }

        inline void ttgopir() { set("ttgopir"); }

        inline void m5() { set("m5"); }

        inline void m5fisheye() { set("m5fisheye"); }

        inline void m5timerx() { set("m5timerx"); }

        inline void espeye() { set("espeye"); }

        inline void espeyes3() { set("espeyes3"); }

        inline void wrover() { set("wrover"); }

    protected:
        /**
         * Show the user a message to skip prompting in the future
         * @param alias
         */
        void remember(const char *alias) {
            Serial.printf("You can skip prompting next time with `camx.model.%s()`\n", alias);
            set(alias);
        }
    };
}