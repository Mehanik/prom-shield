

class Prom_proto{
    private:
        const int relay[2] = {7, 8};
        const int relay_num = 2;

        const int digital_out[4] = {5, 6, A6, A7};
        const int digital_out_num = 4;

        const int digital_in[4] = {2, 3, 4, 9};
        const int digital_in_num = 4;

        const int analog_in[4] = {A0, A1, A2, A3};
        const int analog_in_num = 4;

        const int sd_cs  = 10; 
        const int sd_di  = 11;
        const int sd_do  = 12; 
        const int sd_clk = 13; 

        const int i2c_sda = A4;
        const int i2c_scl = A5;

        struct {
            int sd_present : 1;
            int sd_logging_enable : 1;
            int serial_logging_enable : 1;
        } status = { 
            .sd_present = 0, 
            .sd_logging_enable = 0,
            .serial_logging_enable = 0            
        };

    public:
        const HardwareSerial &rs232 = Serial;
        const HardwareSerial &log_serial = Serial;
        RTC_DS1307 rtc;

        void Prom_proto();
        void logging(String message);

        void enableSDlogging() {status.sd_logging_enable = 1};
        void disableSDlogging() {status.sd_logging_enable = 0};
}
