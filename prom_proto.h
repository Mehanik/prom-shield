#ifndef PROM_PROTO_H
#define PROM_PROTO_H

#include <Arduino.h>
#include <SD.h>
#include <RTClib.h>
#include <Wire.h>

struct Status_struct{
    uint8_t sd_present : 1;
    uint8_t sd_logging : 1;
    uint8_t serial_logging : 1;
};

struct Byte{
    uint8_t bit0 : 1;
    uint8_t bit1 : 1;
    uint8_t bit2 : 1;
    uint8_t bit3 : 1;
    uint8_t bit4 : 1;
    uint8_t bit5 : 1;
    uint8_t bit6 : 1;
    uint8_t bit7 : 1;
};

union ByteUnion{
    Byte b;
    uint8_t i;
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
        uint8_t sd_init();

        void digitalWrite(const uint8_t, const uint8_t);
        void analogWrite(const uint8_t, const uint8_t);
        uint8_t digitalRead(const uint8_t);
        void rellayWrite(const uint8_t, const uint8_t);
        float analogReadVolts(const uint8_t);
        float analogReadAmps(const uint8_t);
        uint16_t analogReadRAW(const uint8_t num);
        uint8_t analogReadLogic(const uint8_t);
        uint8_t keyboardRead(const uint8_t);

    private:
        static const uint8_t relay_out[2];
        static const uint8_t relay_num;

        static const uint8_t digital_out[4];
        static const uint8_t digital_out_num;

        static const uint8_t digital_in[4];
        static const uint8_t digital_in_num;

        static const uint8_t analog_in[4];
        static const uint8_t analog_in_num;

        static const uint8_t keyboard[1];
        static const uint8_t keyboard_num;

        static const uint8_t sd_cs;
        static const uint8_t sd_di;
        static const uint8_t sd_do;
        static const uint8_t sd_clk;

        static const uint8_t i2c_sda;
        static const uint8_t i2c_scl;

        static const ByteUnion supports_pwm;

        static Status_struct status;
};

#endif
