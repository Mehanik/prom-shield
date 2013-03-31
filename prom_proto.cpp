#include "prom_proto.h"

const uint8_t Prom_proto::relay_out[2] = {7, 8};
const uint8_t Prom_proto::relay_num = 2;

const uint8_t Prom_proto::digital_out[4] = {5, 6, A1, A0};
const uint8_t Prom_proto::digital_out_num = 4;
const ByteUnion Prom_proto::supports_pwm = {1, 1, 0, 0, 0, 0, 0, 0};

const uint8_t Prom_proto::digital_in[4] = {9, 4, 3, 2};
const uint8_t Prom_proto::digital_in_num = 4;

const uint8_t Prom_proto::analog_in[4] = {A2, A3, A6, A7};
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
    // IO initialization
    for (uint8_t i = 0; i < relay_num; i++)
        pinMode(relay_out[i], OUTPUT);

    for (uint8_t i = 0; i < digital_out_num; i++)
        pinMode(digital_out[i], OUTPUT);

    for (uint8_t i = 0; i < digital_in_num; i++)
        pinMode(digital_in[i], INPUT);

    analogReference(INTERNAL); // 1.1V analog reference voltage

    rs232.begin(9600);
    log_serial.begin(9600);

    // I2C and RTC initialization
    Wire.begin();
    rtc.begin();
    if (! rtc.isrunning()){
        //following line sets the RTC to the date & time this sketch was compiled
        rtc.adjust(DateTime(__DATE__, __TIME__));
        logging("RTC is NOT running!");
    }

    // SD card initialization
    pinMode(sd_cs, OUTPUT);
    sd_init();
}

uint8_t Prom_proto::sd_init(){
    if (!SD.begin(sd_cs)) {
        uint8_t tmp = status.sd_logging;
        status.sd_logging = 0;
        logging("SD card initialization failed.");
        status.sd_logging = tmp;
        return 1;
    } else {
        status.sd_present = 1;
        return 0;
    }
}

void Prom_proto::logging(String message)
{
    DateTime now = rtc.now();

    // sending by serial
    if (status.serial_logging){
        log_serial.print('[');
        log_serial.print(now.year(), DEC);
        log_serial.print('-');
        log_serial.print(now.month(), DEC);
        log_serial.print('-');
        log_serial.print(now.day(), DEC);
        log_serial.print(' ');
        log_serial.print(now.hour(), DEC);
        log_serial.print(':');
        log_serial.print(now.minute(), DEC);
        log_serial.print(':');
        log_serial.print(now.second(), DEC);
        log_serial.print("]  ");
        log_serial.print(message);
        log_serial.print('\n');
    }

    // writing to file
    if (status.sd_logging ){
        if (!status.sd_present)
            sd_init(); // try to reinitialize card

        if (status.sd_present){
            File log_file = SD.open("system.log", FILE_WRITE);

            //if the file is available, write to it:
            if (log_file) {
                log_file.print('[');
                log_file.print(now.year(), DEC);
                log_file.print('-');
                log_file.print(now.month(), DEC);
                log_file.print('-');
                log_file.print(now.day(), DEC);
                log_file.print(' ');
                log_file.print(now.hour(), DEC);
                log_file.print(':');
                log_file.print(now.minute(), DEC);
                log_file.print(':');
                log_file.print(now.second(), DEC);
                log_file.print("]  ");
                log_file.print(message);
                log_file.print('\n');
                log_file.close();
            }  
            // if the file isn't open, pop up an error:
            else {
                status.sd_present = 0;
                logging("Error opening system.log");
            } 
        }
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

#define ADC_COEF_V (1.1 / 1024.0 * 11.0)
#define ADC_COEF_A (1.1 * 11.0 / 500.0 / 1024.0)
#define ADC_THRESHOLD (1024 / 11.0 / 1.1 * 9) // 9V threshold voltage
float Prom_proto::analogReadVolts(const uint8_t num){
    if (num < analog_in_num)
        return (float) ::analogRead(analog_in[num]) * ADC_COEF_V;
    else
        return 0;
}

float Prom_proto::analogReadAmps(const uint8_t num){
    if (num < analog_in_num)
        return (float) ::analogRead(analog_in[num]) * ADC_COEF_A;
    else
        return 0;
}

uint16_t Prom_proto::analogReadRAW(const uint8_t num){
    if (num < analog_in_num)
        return ::analogRead(analog_in[num]);
    else
        return 0;
}

uint8_t Prom_proto::analogReadLogic(const uint8_t num){
    if (num < analog_in_num)
        return (::analogRead(analog_in[num]) > ADC_THRESHOLD) ? 1 : 0;
    else
        return 0;
}

uint8_t Prom_proto::keyboardRead(const uint8_t num){
    if (num < keyboard_num)
        return ::digitalRead(keyboard[num]);
    else
        return 0;
}
