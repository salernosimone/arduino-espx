/**
 * Stream JSON to Serial (no memory allocation)
 */
#include <espx.h>
#include <espx/jsonx.h>


Jsonx jsonx(Serial);


void setup() {
  delay(1000);
  Serial.begin(115200);
  Serial.println("JSONx example: print to Serial");
}


void loop() {
  /**
   * Create a JSON object with scalar and
   * composite values
   */
  jsonx.rootObject({
    jsonx.scalar("name", "John"),
    jsonx.scalar("age", 20),
    jsonx.object("address", {
      jsonx.scalar("city", "Rome"),
      jsonx.scalar("country", "Italy")
    }),
    jsonx.list("scores", 10, 20, 30),
    jsonx.list(
      "children",
      jsonx.object(),
      jsonx.null(),
      jsonx.object({
        jsonx.scalar("name", "Jane"),
        jsonx.list("hobbies", "Ping pong", "Boxing")
      })
    )
  });

  delay(2000);
}