#include "testing_arduino.h"
#include <cstdarg>
#include <stdio.h>

PrintSerial::PrintSerial() {

}

void PrintSerial::printf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
}

void PrintSerial::begin(int baud) {

}

PrintSerial Serial;
