#include "prom_proto.h"

const uint8_t Prom_proto::relay_out[2] = {7, 8};
const uint8_t Prom_proto::relay_num = 2;

const uint8_t Prom_proto::digital_out[4] = {5, 6, A6, A7};
const uint8_t Prom_proto::digital_out_num = 4;
const ByteUnion Prom_proto::supports_pwm = {1, 1, 0, 0, 0, 0, 0, 0};

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

const uint8_t Prom_proto::keyboard[1] = {9};
const uint8_t Prom_proto::keyboard_num = {1};

HardwareSerial & Prom_proto::rs232 = Serial;
HardwareSerial & Prom_proto::log_serial = Serial;

Status_struct Prom_proto::status = { 
    .sd_present = 0, 
    .sd_logging = 0,
    .serial_logging = 0            
};

void Prom_proto::init(){

    for (uint8_t i = 0; i < relay_num; i++)
        pinMode(relay_out[i], OUTPUT);

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

void Prom_proto::digitalWrite(const uint8_t num, const uint8_t value){
    if (num < digital_out_num)
        ::digitalWrite(digital_out[num], value);
}

void Prom_proto::analogWrite(const uint8_t num, const uint8_t value){
    if (num < digital_out_num && (supports_pwm.i & (1 << num)))
        ::analogWrite(digital_out[num], value);
}

uint8_t Prom_proto::digitalRead(const uint8_t num){
    if (num < digital_out_num)
        return ::digitalRead(digital_in[num]);
    else
        return 0;
}

void Prom_proto::rellayWrite(const uint8_t num, const uint8_t value){
    if (num < relay_num)
        ::digitalWrite(relay_out[num], value);
}

uint8_t Prom_proto::rellayRead(const uint8_t num){
    if (num < relay_num)
        return ::digitalRead(relay_out[num]);
    else
        return 0;
}

#define ADC_COEF_V (11.0 / 1024)
#define ADC_COEF_A (0.02 / 1024)
#define ADC_THRESHOLD (1024 / 11 * 9) // 9V threshold voltage
float Prom_proto::analogReadVolts(const uint8_t num){
    if (num < analog_in_num)
        return ::analogRead(num) * ADC_COEF_V;
    else
        return 0;
}

float Prom_proto::analogReadAmps(const uint8_t num){
    if (num < analog_in_num)
        return ::analogRead(num) * ADC_COEF_A;
    else
        return 0;
}

uint8_t Prom_proto::analogReadLogic(const uint8_t num){
    if (num < analog_in_num)
        return (::analogRead(num) > ADC_THRESHOLD) ? 1 : 0;
    else
        return 0;
}

uint8_t Prom_proto::keyboardRead(const uint8_t num){
    if (num < keyboard_num)
        return ::digitalRead(keyboard[num]);
    else
        return 0;
}
