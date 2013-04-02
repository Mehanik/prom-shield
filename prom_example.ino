#include <Arduino.h>
#include <SD.h>
#include <RTClib.h>
#include <string.h>
#include "prom_proto.h"

Prom_proto board;
uint8_t key_state;
uint8_t duty_cycle = 0;

void setup() {                
    board.enableSerialLogging();
    board.enableSDlogging();
    board.init(); // Shield initialization
    board.logging("Initialization complete."); // Writing message to system log

    // Using keyboard
    key_state = board.keyboardRead(0);
}

void loop() {
    uint8_t val;

    // Using digital inputs and outputs
    for(int n = 1; n < 4; n++){
        val = board.digitalInRead(n);
        board.digitalOutWrite(n, val);
    }

    // Using pwm output
    board.analogOutWrite(0, duty_cycle);
    duty_cycle++;

    // Using relay outputs
    val = board.digitalInRead(0);
    board.rellayWrite(0, val);

    // Using analog inputs as digital
    val = board.analogReadLogic(3);
    board.rellayWrite(1, val);

    val = board.keyboardRead(0);
    if ((val != key_state) && val){ // If key pressed
        // Save to log file voltage on input 0
        float u, i;
        char tmpstr[40];
        u = board.analogReadVolts(0);
        sprintf(tmpstr, "Voltage at analog input #0 = %i.%i V", \
                (uint16_t)u, (uint16_t)(u * 1000));
        board.logging(tmpstr);

        // Save to log file current on input 1
        i = board.analogReadAmps(1);
        sprintf(tmpstr, "Current at analog input #1 = %i.%i mA", \
                (uint16_t)(i*1000), (uint16_t)(i * 1000000) % 1000);
        board.logging(tmpstr);

        // And RAW value at input 2
        sprintf(tmpstr, "RAW value at analog input #2 = %i", board.analogReadRAW(2));
        board.logging(tmpstr);
    }
    key_state = val;

    delay(100);
}
