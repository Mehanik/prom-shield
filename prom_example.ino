#include <Arduino.h>
#include <SD.h>
#include <RTClib.h>
#include "prom_proto.h"

Prom_proto board;

void setup() {                
    board.enableSerialLogging();
    board.enableSDlogging();
    board.init(); // Shield initialization
    board.logging("Initialization complete."); // Writing message to system log
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

    // Using keyboard
    v = board.keyboardRead(0);

    // Using relay outputs
    board.rellayWrite(0, v);

    // Using analog inputs
    float u, i;
    u = board.analogReadVolts(0);
    i = board.analogReadAmps(1);
    board.rs232.print("Volrage at analog input 0 = ");
    board.rs232.print(u);
    board.rs232.print("\n");
    board.rs232.print("Current at analog input 1 = ");
    board.rs232.print(i);
    board.rs232.print("\n");

    // Using analog inputs as digital
    v = board.analogReadLogic(3);
    board.rellayWrite(1, v);

    // Using RTC

    delay(1000);
}
