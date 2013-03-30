#ifndef PROM_PROTO_H
#define PROM_PROTO_H

#include <Arduino.h>
#include <SD.h>
#include <RTClib.h>
#include <Wire.h>

struct Status_struct{
    int sd_present : 1;
    int sd_logging : 1;
    int serial_logging : 1;
};

class Prom_proto{
    public:
        static HardwareSerial &rs232;
        static HardwareSerial &log_serial;
        RTC_DS1307 rtc;

        void init();
        void logging(String message);

        void enableSDlogging() {status.sd_logging = 1;}
        void disableSDlogging() {status.sd_logging = 0;}
        void enableSerialLogging() {status.serial_logging = 1;}
        void disableSerialLogging() {status.serial_logging = 0;}

    private:
        static const uint8_t relay[2];
        static const uint8_t relay_num;

        static const uint8_t digital_out[4];
        static const uint8_t digital_out_num;

        static const uint8_t digital_in[4];
        static const uint8_t digital_in_num;

        static const uint8_t analog_in[4];
        static const uint8_t analog_in_num;

        static const uint8_t sd_cs;
        static const uint8_t sd_di;
        static const uint8_t sd_do;
        static const uint8_t sd_clk;

        static const uint8_t i2c_sda;
        static const uint8_t i2c_scl;

        static Status_struct status;
};

#endif
