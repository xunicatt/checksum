#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "checksum.h"

bool
checksum_check_segment(
    const size_t segment
) {
    return (segment & (segment - 1));
}

int
checksum_add_padded_bits(
    char* str,
    const size_t segment,
    const size_t len,
    const size_t capacity
) {
    size_t padded_bits = segment - (len%segment);
    if(len+padded_bits >= capacity) [[clang::unlikely]] return -1;

    for(size_t i = len+padded_bits; (long)i >= 0; i--)
        str[i] =  ((i >= padded_bits)*str[i-padded_bits]) + ((i < padded_bits)*'0');

    return len + padded_bits;
}

unsigned long
checksum_generate (
    const char* src,
    const size_t segment,
    const size_t len
) {
    if(
        src == NULL ||
        segment == 0 ||
        len == 0 ||
        segment >= len
    )[[clang::unlikely]]
        return -1;

    unsigned long checksum = 0;
    unsigned long data = 0;

    for(size_t i = 0; i < len; i++) {
        data <<= 1;
        data |= src[i] == '1';

        if(!((i + 1) % segment)) [[clang::unlikely]] {
            checksum += data;
            checksum += (checksum >> segment);
            checksum &= (~(1 << segment));
            data ^= data;
        }
    }

    return ~checksum;
}

int
checksum_add_checksum(
    char* str,
    const size_t segment,
    const size_t len,
    const size_t capacity,
    const unsigned long checksum
) {
    const size_t new_size = len + segment;
    if(new_size >= capacity) [[clang::unlikely]] return -1;

    for(size_t i = len, j = 0; i < new_size; i++, j++)
            str[i] = ((checksum & (1 << (segment - j - 1))) >> (segment - j - 1)) + '0';

    str[new_size] = 0;
    return new_size;
}

bool
checksum_check(
    const char *str,
    const size_t segment,
    const size_t len
) {
    if(checksum_check_segment(segment)) [[clang::unlikely]] return false;
    if(len%segment) [[clang::unlikely]] return false;

    char checksum_str[segment + 1];
    char data[len - segment];

    strncpy(checksum_str, str + len - segment, segment);
    strncpy(data, str, len - segment);

    unsigned long checksum = checksum_generate(data, segment, len - segment);
    unsigned long recv_checksum = 0;

    for(size_t i = 0; i < segment; i++) {
        recv_checksum <<= 1;
        recv_checksum |= (checksum_str[i] == '0');
    }

    return ~recv_checksum == checksum;
}

int
checksum_truncate(
    char* str,
    const size_t segment,
    const size_t len
) {
    if(segment >= len) return -1;
    str[len - segment] = 0;
    return len - segment;
}
