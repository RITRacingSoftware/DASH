#include <stdio.h>
#include <stdint.h>

#ifndef READ_CF_H
#define READ_CF_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct cf_file_s {
    FILE *fptr;
    uint64_t usec_offset;
    uint64_t msb;
    uint32_t msb_offset;
    uint8_t data[8];
    uint8_t msb_loaded;
} cf_file;

void cf_open(cf_file *ptr, const char *fname);
void cf_close(cf_file *ptr);
uint8_t cf_available(cf_file *ptr);
void cf_read_message(cf_file *ptr, uint8_t *buf, uint8_t bufsize);

#ifdef __cplusplus
}
#endif

#endif
