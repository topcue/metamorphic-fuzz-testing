
#ifndef KISA_SHA256_H
#define KISA_SHA256_H


#include "Metamorphic_Fuzz_Test.h"

#define SHA256_BLOCKLEN	64
#define SHA256_DIGESTLEN 32

typedef unsigned char u8;
typedef unsigned int u32;

typedef struct
{
	u32 chainVar[8];
	u32 HighLength;
	u32 LowLength;
	u8 szBuffer[64];
} SHA256_INFO;

void SHA256_Encrypt(const u8* pszMessage, u32 uPlainTextLen, u8* pszDigest);
void SHA256_Init(SHA256_INFO* Info);
void SHA256_Process(SHA256_INFO* Info, const u8* pszMessage,u32 uDataLen);
void SHA256_Close(SHA256_INFO* Info, u8* pszDigest);

#endif

//EOF
