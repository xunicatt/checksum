#ifndef __CHECKSUM_H
#define __CHECKSUM_H

#include <stddef.h>

bool checksum_check_segment(const size_t);
unsigned long checksum_generate(const char*,const size_t,const size_t);
int checksum_add_padded_bits(char*,const size_t,const size_t,const size_t);
int checksum_add_checksum(char*,const size_t,const size_t,const size_t,const unsigned long);
bool checksum_check(const char*,const size_t,const size_t);
int checksum_truncate(char*,const size_t,const size_t);

#endif
