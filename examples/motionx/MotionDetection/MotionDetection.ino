/**
 * Perform motion detection on camera frames
 */
#include <JPEGDEC.h>
#include <espx.h>
#include <espx/camx.h>
#include <espx/motionx.h>


void setup() {
    delay(1000);
    Serial.begin(115200);
    Serial.println("Motionx example: motion detection");

    // configure camx through Serial Monitor
    camx.model.prompt();
    camx.pixformat.jpeg();
    camx.quality.high();
    camx.resolution.vga();

    // initialize camx,
    // enter endless loop on error
    camx.begin().raise();

    // configure motion detection
    // 1. configure smoothing factor
    //    (the higher, the more past frames are taken into account)
    motionx.smooth(0.8);
    // 2. for the first N frames, just update the background
    //    without running the detection
    motionx.trainFor(10);
    // 3. count in "motion" only pixels that changed value
    //    by more than X
    motionx.diffBy(10);
    // 4. configure moving threshold to trigger a positive detection:
    //    trigger when Y percent of pixels changed
    motionx.threshold("30%");
    // 5. drop motion events in rapid succession:
    //    limit to at most one every N seconds
    motionx.throttle("3s");
}


void loop() {
    auto frame = camx.grab();

    if (!motionx.process(frame)) {
        Serial.println(motionx.failure());
        return;
    }

    Serial.printf(
            "moving pixels %%=%.2f, detection time=%dms\n",
            motionx.ratio(), motionx.stopwatch.millis()
    );

    // motionx is true if ratio >= threshold
    if (motionx) {
      Serial.println("Motion detected");
    }
}