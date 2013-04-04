#include <Arduino.h>
#include <SD.h>
#include <RTClib.h>
#include <string.h>
#include "prom_proto.h"

Prom_proto board;
uint8_t key_state;
uint8_t duty_cycle = 0;

class Scan_cycle{
    public:
        unsigned long getAverage(){ // average loop duration in microseconds
            status.reset_average = 1;
            return avg_duration;
        }
        unsigned long getMax(){ // max loop duration in microseconds
            status.reset_max = 1;
            return max_duration;
        } 
        void refresh(){
            unsigned long now = micros();
            cycle_counter ++;

            if (status.reset_average){
                status.reset_average = 0;
                avg_time = now;
                cycle_counter = 0;
            } else {
                avg_duration = (now - avg_time) / cycle_counter;
            }

            unsigned long new_duration = now - last_refresh_time;
            last_refresh_time = now;
            if (status.reset_max){
                max_duration = new_duration;
            } else {
                if (max_duration < new_duration)
                    max_duration = new_duration;
            }
        }
    private:
        unsigned long avg_time;
        unsigned long avg_duration;
        unsigned long last_refresh_time;
        unsigned long cycle_counter;
        unsigned long max_duration;
        struct {
            uint8_t reset_average: 1;
            uint8_t reset_max: 1;
        } status = {1, 1};
} scan_cycle;


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
        char tmpstr[80];
        u = board.analogReadVolts(0);
        sprintf(tmpstr, "Voltage at analog input #0 = %hu.%hu V", \
                (uint16_t)u, (uint16_t)(u * 1000 - (uint16_t)(u) * 1000));
        board.logging(tmpstr);

        // Save to log file current on input 1
        i = board.analogReadAmps(1) * 1000;
        sprintf(tmpstr, "Current at analog input #1 = %hu.%hu mA", \
                (uint16_t)u, (uint16_t)(u * 1000 - (uint16_t)(u) * 1000));
        board.logging(tmpstr);

        // And RAW value at input 2
        sprintf(tmpstr, "RAW value at analog input #2 = %hu", board.analogReadRAW(2));
        board.logging(tmpstr);

        sprintf(tmpstr, "loop() duration: average = %lu us, max = %lu us\n", \
                scan_cycle.getAverage(), scan_cycle.getMax());
        board.logging(tmpstr);
    }
    key_state = val;

    scan_cycle.refresh();
}
