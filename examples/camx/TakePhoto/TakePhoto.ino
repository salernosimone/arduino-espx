/**
 * Take a picture
 */
#include <espx.h>
#include <espx/camx.h>


void setup() {
    delay(1000);
    Serial.begin(115200);
    Serial.println("Camx example: take photo");
    
    // configure camx through Serial Monitor
    camx.model.prompt();
    camx.pixformat.prompt();
    camx.quality.prompt();
    camx.resolution.prompt();
    
    // initialize camx,
    // enter endless loop on error
    camx.begin().raise();
}


void loop() {
    auto frame = camx.grab();

    if (!frame) {
      Serial.println(frame.failure());
      return;
    }

    Serial.printf("Frame size: %d bytes\n", frame.length);
} 