#include <Arduino.h>
#include <SD.h>
#include <RTClib.h>
#include <string.h>
#include "prom_proto.h"

Prom_proto board;
uint8_t key_state;

void setup() {                
    board.enableSerialLogging();
    board.enableSDlogging();
    board.init(); // Shield initialization
    board.logging("Initialization complete."); // Writing message to system log

    // Using keyboard
    key_state = board.keyboardRead(0);
}

void loop() {
    // Using digital outs
    board.digitalWrite(3, HIGH);
    board.digitalWrite(2, LOW);
    board.analogWrite(0, 128);

    // Using digital inputs
    uint8_t v;
    v = board.digitalRead(0);
    board.digitalWrite(1, v);

    // Using relay outputs
    board.rellayWrite(0, HIGH);

    v = board.keyboardRead(0);
    if ((v != key_state) && v){
        // Using analog inputs
        float u, i;
        char tmpstr[40];
        u = board.analogReadVolts(0);
        sprintf(tmpstr, "Voltage at analog input #0 = %i.%i V", \
                (uint16_t)u, (uint16_t)(u * 1000));
        board.logging(tmpstr);

        i = board.analogReadAmps(1);
        sprintf(tmpstr, "Current at analog input #1 = %i.%i mA", \
                (uint16_t)(i*1000), (uint16_t)(i * 1000000) % 1000);
        board.logging(tmpstr);

        sprintf(tmpstr, "RAW value at analog input #2 = %i", board.analogReadRAW(2));
        board.logging(tmpstr);
    }
    key_state = v;

    // Using analog inputs as digital
    v = board.analogReadLogic(3);
    board.rellayWrite(1, v);

    delay(100);
}
