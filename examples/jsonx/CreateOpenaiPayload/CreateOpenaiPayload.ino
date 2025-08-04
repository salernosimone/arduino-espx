/**
 * Generate JSON string for OpenAI chat completion API
 */
#include <espx.h>
#include <espx/jsonx.h>


// save JSON into a String
StringIO buffer;
Jsonx jsonx(buffer);


void setup() {
  delay(1000);
  Serial.begin(115200);
  Serial.println("JSONx example: OpenAI chat completion payload");
}


void loop() {
  jsonx.rootObject({
    jsonx.scalar("model", "gpt-41-mini"),
    jsonx.scalar("temperature", 0),
    jsonx.list(
      "messages",
      jsonx.object({
        jsonx.scalar("role", "system"),
        jsonx.scalar("content", "You are an helpful assistant")
      }),
      jsonx.object({
        jsonx.scalar("role", "user"),
        jsonx.scalar("content", "The capital of Italy?")
      })
    )
  });

  Serial.print("OpenAI payload: ");
  Serial.println(buffer.value());

  // clear buffer, otherwise JSON gets appended
  buffer.clear();
  delay(2000);
}