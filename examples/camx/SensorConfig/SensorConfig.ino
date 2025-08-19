/**
 * Configure sensor.
 * Boot the MJPEG server to visually inspect tha changes.
 */
#include <espx.h>
#include <espx/wifix.h>
#include <espx/mdnsx.h>
#include <espx/camx.h>
#include <espx/camx/mjpegx.h>


void setup() {
    delay(1000);
    Serial.begin(115200);
    Serial.println("Camx example: sensor config");

    // configure camx through Serial Monitor
    camx.model.prompt();
    camx.pixformat.jpeg();
    camx.quality.high();
    camx.resolution.qvga();
    camx.begin().raise();

    // configure sensor AFTER camera init!
    // (use IDE autocomplete suggestions to explore all options)
    camx.sensor.flipHor();
    camx.sensor.flipVert();
    camx.sensor.saturation(Intensity::high);
    camx.sensor.brightness(Intensity::base);
    // always remember to apply() the settings!
    camx.sensor.apply();

    // start mjpeg server
    wifix("SSID", "PASSWORD").raise();
    mdnsx("esp32cam");
    mjpegx.listenOn(80);
    mjpegx.begin().raise();
}


void loop() {
  // server runs in background
}