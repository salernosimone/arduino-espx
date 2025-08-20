/**
* Do GET requests with httpx
*/
#include <espx.h>
#include <espx/wifix.h>
#include <espx/httpx.h>


void setup() {
    delay(1000);
    Serial.begin(115200);
    Serial.println("Httpx example: GET request");

    // connect to WiFi
    wifix("SSID", "PASSWORD").raise();
}

void loop() {
    // GET request
    // skips SSL cert verification
    // timeout after 5 seconds
    // "Accept: text/plain" header
    auto response = httpx.run(
        "https://icanhazdadjoke.com/",
        // configurations go first
        httpx.Insecure(),
        httpx.RequestTimeout("5s"),
        // then headers
        httpx.Header("Accept", "text/plain"),
        // Body() MUST be the last
        httpx.Body("GET")
    );

    if (!response) {
        // request failed
        Serial.print("Request failed with reason: ");
        Serial.println(response.failure());
    }
    else {
        // request was ok
        Serial.printf("Response[%d]\n%s\n", response.code, response.text().c_str());
    }

    delay(5000);
}