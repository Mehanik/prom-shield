#include <SD.h>
#include "RTClib.h"
#include "prom_proto.h"

Prom_proto *board;

void setup() {                
    board = new Prom_proto;
}

// the loop routine runs over and over again forever:
void loop() {
}
