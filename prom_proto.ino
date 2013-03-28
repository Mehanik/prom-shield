#include <SD.h>
#include "RTClib.h"
#include "prom_proto.h"

void Prom_proto::Prom_proto(){
    for (int i = 0; i < relay_num; i++)
        pinMode(relay[i], OUTPUT);

    for (int i = 0; i < digital_out_num; i++)
        pinMode(digital_out[i], OUTPUT);

    for (int i = 0; i < digital_in_num; i++)
        pinMode(digital_in[i], INPUT);

    rs232.begin(9600);
    log_serial.begin(9600);
    status.serial_logging_enable = 1;

    Wire.begin;
    rtc.begin();

    if (! RTC.isrunning()){
        logging("RTC is NOT running!");
        // following line sets the RTC to the date & time this sketch was compiled
        RTC.adjust(DateTime(__DATE__, __TIME__));
    }

    sdLoggingStart();
}

void Prom_proto::logging(String message)
{
    DateTime now = rtc.now();

    // sending by serial
    if (status.serial_logging_enable){
        log_serial.print('[');
        log_serial.print(now.year, DEC);
        log_serial.print('-');
        log_serial.print(now.month, DEC);
        log_serial.print('-');
        log_serial.print(now.day, DEC);
        log_serial.print(' ');
        log_serial.print(now.hour, DEC);
        log_serial.print(':');
        log_serial.print(now.minute, DEC);
        log_serial.print(':');
        log_serial.print(now.second, DEC);
        log_serial.print("]  ");
        log_serial.print(message);
    }

    // writing to file
    if (status.sd_logging_enable && status.sd_present){
        ;
    }
}
