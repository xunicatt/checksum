/*
** Author: Aniket Biswas
** Checksum Generator
*/

#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "checksum/checksum.h"

#define BUFF_SIZE 512

int main(void) {
    char buffer[BUFF_SIZE] = {0};
    size_t segment = 0, len = 0;
    unsigned long checksum = 0;

    //INPUTS
    fprintf(stdout, "bit_string> ");
    fgets(buffer, sizeof(buffer) - 1, stdin);

    fprintf(stdout, "segment_size> ");
    fscanf(stdin, "%zu", &segment);

    //SEGMENT CHECK
    if(checksum_check_segment(segment)) [[clang::unlikely]] {
        fprintf(stderr, "error: segment length is not in the power of 2");
        return EXIT_FAILURE;
    }

    //PADDED BITS
    len = strnlen(buffer, sizeof(buffer));
    buffer[--len] = 0;
    if(len%segment) [[clang::likely]] {
        int retval = 0;
        if((retval = checksum_add_padded_bits(buffer, segment, len, sizeof(buffer))) == -1) [[clang::unlikely]] {
            fprintf(stderr, "error: buffer overflow");
            return EXIT_FAILURE;
        }

        len = retval;
    }

    fprintf(stdout, "padded_string=%s\n", buffer);
    //CHECKSUM GENERATION
    checksum = checksum_generate(buffer, segment, len);
    int new_size = 0;
    if((new_size = checksum_add_checksum(buffer, segment, len, sizeof(buffer), checksum)) == -1) [[clang::unlikely]] {
        fprintf(stderr, "error: buffer overflow\n");
        return  EXIT_FAILURE;
    }

    fprintf(stdout, "checksum=%s\n", buffer+len);
    fprintf(stdout, "code_word=%s\n", buffer);
    len = new_size;
}
