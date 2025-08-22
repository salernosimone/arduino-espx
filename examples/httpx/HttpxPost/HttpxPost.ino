/**
* Do POST requests with httpx
*/
#include <espx.h>
#include <espx/wifix.h>
#include <espx/httpx.h>


void setup() {
    delay(1000);
    Serial.begin(115200);
    Serial.println("Httpx example: POST request");

    // connect to WiFi
    wifix("SSID", "PASSWORD").raise();
}

void loop() {
    // POST request
    // skips SSL cert verification
    // timeout after 5 seconds
    // Accept json header
    // Content Type json header
    // send dummy JSON body
    auto response = httpx.run(
        "https://httpbin.org/post",
        // configurations go first
        httpx.Insecure(),
        httpx.RequestTimeout("5s"),
        // then headers
        httpx.Header("Accept", "application/json"),
        httpx.Header("Content-Type", "application/json"),
        // Body() MUST be the last
        httpx.Body("POST", "[1, 2, 3]")
    );

    if (!response) {
        // request failed
        Serial.print("Request failed with reason: ");
        Serial.println(response.failure());
    }
    else {
        // request was ok
        Serial.printf("Response [%d]\n", response.code);
        Serial.println(response.text());
    }

    // end connection
    httpx.end();

    delay(5000);
}