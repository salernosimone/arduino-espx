/**
 * Create background threads
 */
#include <espx.h>
#include <espx/threadx.h>


String name = "John Doe";
TaskHandle_t handle;


void setup() {
  delay(2000);
  Serial.begin(115200);
  Serial.setTimeout(4000);
  Serial.println("Threadx example: create threads");
  delay(2000);

  // create an "anonymous" thread
  // with default config
  threadx([](void *) {
      while (true) {
          Serial.println("Inside anonymous thread");
          delay(1000);
      }
  });

  // you can also create a "detached" function that runs once
  // in background and exits (no while loop needed!)
  threadx([](void *) {
      Serial.println("Run and exit!");
  });

  // create fully configured thread
  threadx(
    [](void *userdata) {
      // convert userdata back to String
      String name = *((String*) userdata);

      while (true) {
          Serial.print("Inside fully configured thread. Name: ");
          Serial.println(name);
          delay(2000);
      }
    },
    threadx.Name("HelloWorldThread"),
    threadx.Core(1),
    threadx.Stack("5 kb"),
    // you can choose between
    // - NotImportant (priority = 0)
    // - Important (priority = MAX / 2)
    // - Critical (priority = MAX)
    threadx.Important(),
    // Userdata is passed as first argument to the task function
    threadx.Userdata(&name),
    // bind thread to handler (see loop)
    threadx.Handler(handle)
  );
}


void loop() {
  // cancel thread on user input
  Serial.println("Do you want to abort HelloWorldThread? (yes|no)");
  String answer = Serial.readString();

  if (answer.startsWith("yes")) {
    Serial.println("Aborting thread...");
    Serial.setTimeout(60000);
    vTaskDelete(handle);
  }
}