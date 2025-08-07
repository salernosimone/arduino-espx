#include <espx.h>
#include <espx/camx.h>

void setup() {
    delay(1000);
    Serial.begin(115200);
    Serial.println("Camx example: take photo");
    
    // configure camx through Serial Monitor
    camx.model.prompt();
    camx.pixformat.prompt();
//    camx.quality.prompt();
    camx.resolution.prompt();
    
    // initialize camx
    // enter endless loop on error
    camx.begin().raise();
}

void loop() {
    Serial.println("loop");
    delay(2000);
} 