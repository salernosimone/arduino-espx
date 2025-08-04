/**
 * Utilities to connect to a WiFi network
 */
#include <espx.h>
#include <espx/wifix.h>


void setup() {
    delay(1000);
    Serial.begin(115200);
    Serial.println("Wifix example: connect to WiFi");

    // try to connect
    // default timeout is 8 seconds
    // default retries is 0
    wifix("SSID", "PASSWORD");

    if (wifix) {
        // we are connected to WiFi, print IP address
        Serial.println("Connected!");
        Serial.print("IP: ");
        Serial.println(wifix.ip);
    } else {
        // connection didn't succeed
        Serial.println("Can't connect to WiFi");
    }

    // you can configure the connection timeout and retries
    // since these arguments are optional, you use
    // named arguments (you can re-order them as you like).
    // I vastly prefer this style instead of wifix("SSID", "PASS", "8s", 2)
    // "What's 8s? What does 2 stand for?"
    // if only C++ had named arguments like Python...
    wifix("SSID", "PASSWORD", wifix.Timeout("8s"), wifix.Retry(2));

    // while in dev mode,
    // you can abort if connection was not successful:
    // `raise()` will enter an endless loop
    wifix("BAD SSID", "BAD PASSWORD").raise();
}

void loop() {
}