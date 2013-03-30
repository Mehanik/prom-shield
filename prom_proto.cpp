#include "prom_proto.h"

const uint8_t Prom_proto::relay[2] = {7, 8};
const uint8_t Prom_proto::relay_num = 2;

const uint8_t Prom_proto::digital_out[4] = {5, 6, A6, A7};
const uint8_t Prom_proto::digital_out_num = 4;

const uint8_t Prom_proto::digital_in[4] = {2, 3, 4, 9};
const uint8_t Prom_proto::digital_in_num = 4;

const uint8_t Prom_proto::analog_in[4] = {A0, A1, A2, A3};
const uint8_t Prom_proto::analog_in_num = 4;

const uint8_t Prom_proto::sd_cs  = 10; 
const uint8_t Prom_proto::sd_di  = 11;
const uint8_t Prom_proto::sd_do  = 12; 
const uint8_t Prom_proto::sd_clk = 13; 

const uint8_t Prom_proto::i2c_sda = A4;
const uint8_t Prom_proto::i2c_scl = A5;

HardwareSerial & Prom_proto::rs232 = Serial;
HardwareSerial & Prom_proto::log_serial = Serial;

Status_struct Prom_proto::status = { 
    .sd_present = 0, 
    .sd_logging = 0,
    .serial_logging = 0            
};

void Prom_proto::init(){

    for (uint8_t i = 0; i < relay_num; i++)
        pinMode(relay[i], OUTPUT);

    for (uint8_t i = 0; i < digital_out_num; i++)
        pinMode(digital_out[i], OUTPUT);

    for (uint8_t i = 0; i < digital_in_num; i++)
        pinMode(digital_in[i], INPUT);

    rs232.begin(9600);
    log_serial.begin(9600);

    Wire.begin();
    rtc.begin();
    if (! rtc.isrunning()){
        //following line sets the RTC to the date & time this sketch was compiled
        rtc.adjust(DateTime(__DATE__, __TIME__));
        logging("RTC is NOT running!");
    }

    logging("Initialization complete");
}

void Prom_proto::logging(String message)
{
    DateTime now = rtc.now();

    // sending by serial
    if (status.serial_logging){
        Serial.print('[');
        Serial.print(now.year(), DEC);
        Serial.print('-');
        Serial.print(now.month(), DEC);
        Serial.print('-');
        Serial.print(now.day(), DEC);
        Serial.print(' ');
        Serial.print(now.hour(), DEC);
        Serial.print(':');
        Serial.print(now.minute(), DEC);
        Serial.print(':');
        Serial.print(now.second(), DEC);
        Serial.print("]  ");
        Serial.print(message);
        Serial.print('\n');
    }

    // writing to file
    if (status.sd_logging && status.sd_present){
        ;
    }
}
