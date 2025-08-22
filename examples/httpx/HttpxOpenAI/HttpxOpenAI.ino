/**
* Send text to OpenAI with httpx
*/
#include <espx.h>
#include <espx/wifix.h>
#include <espx/httpx.h>
#include <espx/jsonx.h>


String apiKey;


void setup() {
    delay(1000);
    Serial.begin(115200);
    Serial.println("Httpx example: send request to OpenAI");

    // connect to WiFi
    wifix("SSID", "PASSWORD").raise();

    // get api key once
    apiKey = promptString("Enter API key: ");
}

void loop() {
    const String model = promptString("Enter model id");
    const String prompt = promptString("Enter prompt");

    // use jsonx to construct payload
    StringIO payload;
    Jsonx jsonx(payload);

    jsonx.rootObject({
        jsonx.scalar("model", model.c_str()),
        jsonx.scalar("temperature", 0.5f),
        jsonx.list(
            "messages",
            jsonx.object({
                jsonx.scalar("role", "user"),
                jsonx.scalar("content", prompt.c_str())
            })
        )
    });

    Serial.print("Payload: ");
    Serial.println(payload.value());

    // POST request
    // skip SSL cert verification
    // timeout after 10 seconds
    // Accept json header
    // Content Type json header
    auto response = httpx.run(
        "https://api.openai.com/v1/chat/completions",
        // configurations go first
        httpx.Insecure(),
        httpx.RequestTimeout("10s"),
        // then headers
        httpx.Header("Accept", "application/json"),
        httpx.Header("Content-Type", "application/json"),
        httpx.Header("Authorization", String("Bearer ") + apiKey),
        // Body() MUST be the last
        httpx.Body("POST", payload.c_str())
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