#include "argparse_internal.h"

#include <stddef.h>

size_t argparse_utf8_strlen(const char *str)
{
    if (!str) {
        return 0;
    }

    size_t count = 0;
    const unsigned char *bytes = (const unsigned char *)str;

    while (*bytes) {
        if ((*bytes & 0x80) == 0x00) {
            bytes++;
        } else if ((*bytes & 0xE0) == 0xC0) {
            if ((bytes[1] & 0xC0) != 0x80) {
                bytes++;
                continue;
            }
            bytes += 2;
        } else if ((*bytes & 0xF0) == 0xE0) {
            if ((bytes[1] & 0xC0) != 0x80 || (bytes[2] & 0xC0) != 0x80) {
                bytes++;
                continue;
            }
            bytes += 3;
        } else if ((*bytes & 0xF8) == 0xF0) {
            if ((bytes[1] & 0xC0) != 0x80 || (bytes[2] & 0xC0) != 0x80 ||
                (bytes[3] & 0xC0) != 0x80) {
                bytes++;
                continue;
            }
            bytes += 4;
        } else {
            bytes++;
            continue;
        }
        count++;
    }

    return count;
}
