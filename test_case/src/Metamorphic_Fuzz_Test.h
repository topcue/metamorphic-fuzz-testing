
#ifndef METAMORPHIC_FUZZ_TEST_H
#define METAMORPHIC_FUZZ_TEST_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <memory.h>
#include <assert.h>
#include <time.h>
#include <malloc.h>
#include "KISA_SHA256.h"

#define SHA256_DIGEST_LEN 32
#define HASH_EXCLUSION_BYTELEN 4

//! Metamorphic Testing
void SHA256_Bit_Contribution_Test(const uint8_t* data, size_t size);
void SHA256_Exclusion_Test(const uint8_t* data, size_t size);
void SHA256_Update_Test(const uint8_t *data, size_t size);

#endif

//EOF
