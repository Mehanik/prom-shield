#include <Arduino.h>
#include <SD.h>
#include <RTClib.h>
#include "prom_proto.h"

Prom_proto board;

void setup() {                
    board.enableSerialLogging();
    board.enableSDlogging();
    board.init();
}

// the loop routine runs over and over again forever:
void loop() {
    board.logging("All right");
    delay(5000);
}
