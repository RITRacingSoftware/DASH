#ifndef TESTING_ARDUINO_H
#define TESTING_ARDUINO_H

class PrintSerial {
    public:
        inline PrintSerial();
        void printf(const char* fmt, ...);
        void begin(int baud);
};

void delay(int ms);

extern PrintSerial Serial;


#endif
