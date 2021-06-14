#ifndef DLPLAN_UTILS_SHA_256_H
#define DLPLAN_UTILS_SHA_256_H

#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C"
{
#endif

void calc_sha_256(uint8_t hash[32], const void *input, size_t len);

#ifdef __cplusplus
}
#endif

#endif
