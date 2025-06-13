#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>

#include "read_cf.h"
#include "gtk_interface.h"

void cf_load_buffer(cf_file *ptr) {
    while (1) {
        int n = fread(ptr->data, 1, 8, ptr->fptr);
        if (n == 0) {
            printf("Rolling over\n");
            fseek(ptr->fptr, 0, SEEK_SET);
            ptr->msb += 0x10000;
            ptr->msb_loaded = 0;
            ptr->msb_offset = 0;
            continue;
        }
        if (ptr->data[0] == 2) return;
        if (ptr->data[0] == 4) {
            uint32_t val;
            fread(&(val), 1, 4, ptr->fptr);
            if (ptr->msb_loaded == 0) {
                ptr->msb_offset = val - (ptr->msb >> 16) - 1;
                ptr->msb_loaded = 1;
            }
            ptr->msb = (val - ptr->msb_offset)<<16;
        } else fseek(ptr->fptr, ptr->data[1]&0x7f, SEEK_CUR);
    }
}

void cf_open(cf_file *ptr, const char *fname) {
    ptr->fptr = fopen(fname, "r");
    ptr->msb_loaded = 0;
    ptr->msb_offset = 0;
    ptr->msb = 0;
    ptr->usec_offset = time_usecs();
    cf_load_buffer(ptr);
}

void cf_close(cf_file *ptr) {
    fclose(ptr->fptr);
}

uint8_t cf_available(cf_file *ptr) {
    if (time_usecs() - ptr->usec_offset >= (ptr->msb | ptr->data[2] | (ptr->data[3]<<8))) return 1;
    return 0;
}

void cf_read_message(cf_file *ptr, uint8_t *buf, uint8_t bufsize) {
    uint8_t len = ptr->data[1];
    if (len + 8 > bufsize) len = bufsize-8;
    memcpy(buf, ptr->data, 8);
    fread(buf+8, 1, len, ptr->fptr);
    cf_load_buffer(ptr);
    // Modify data for testing
    if ((buf[4] | (buf[5] << 8)) == 312) {
        buf[8] = (((time_usecs() - ptr->usec_offset) / 1000000) % 6)<<4;
    }
}
