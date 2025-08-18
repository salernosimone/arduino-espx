/**
 * Create a MJPEG HTTP server
 */
#include <espx.h>
#include <espx/wifix.h>
#include <espx/mdnsx.h>
#include <espx/camx.h>
#include <espx/camx/mjpegx.h>


void setup() {
    delay(1000);
    Serial.begin(115200);
    Serial.println("Camx example: MJPEG server");

    // configure camx through Serial Monitor
    camx.model.prompt();
    camx.pixformat.jpeg();
    camx.quality.high();
    camx.resolution.qvga();

    // initialize camx,
    // enter endless loop on error
    camx.begin().raise();

    // connect to WiFi and set hostname
    // to avoid using the IP address of the board
    wifix("SSID", "PASSWORD").raise();
    mdnsx("esp32cam");

    // start MJPEG server
    mjpegx.listenOn(80);
    mjpegx.begin().raise();
}


void loop() {
}