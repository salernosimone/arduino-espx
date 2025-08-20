/**
 * Start a thread that takes photos in background.
 * Pictures can be access by a queue.
 */
#include <espx.h>
#include <espx/camx.h>


void setup() {
    delay(1000);
    Serial.begin(115200);
    Serial.println("Camx example: take photos in background");

    // configure camx through Serial Monitor
    camx.model.prompt();
    camx.pixformat.jpeg();
    camx.quality.high();
    camx.resolution.qvga();

    // initialize camx,
    // enter endless loop on error
    camx.begin().raise();

    // capture frames in background
    camx.runInBackground();
}


void loop() {
  // camera capture is locked behind a semaphore:
  // to read the current frame, we need to acquire it
  if (camx.lock()) {
    Serial.printf("Current frame size: %d bytes, timestamp: %u\n", camx.frame.length, camx.frame.t);
    // finally, release the lock
    camx.unlock();
    delay(30);
  }

  delay(10);
}