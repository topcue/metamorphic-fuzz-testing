#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "KISA_SHA256.h"

const u32 SHA256_K[64] =
{
	0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1,
	0x923f82a4, 0xab1c5ed5, 0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
	0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174, 0xe49b69c1, 0xefbe4786,
	0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
	0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147,
	0x06ca6351, 0x14292967, 0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
	0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85, 0xa2bfe8a1, 0xa81a664b,
	0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
	0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a,
	0x5b9cca4f, 0x682e6ff3, 0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
	0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

#define ROTL_ULONG(x, y) ((((u32)(x)<<(u32)((y)&31)) | (((u32)(x)&0xFFFFFFFFU)>>(u32)(32-((y)&31)))) & 0xFFFFFFFFU)
#define ROTR_ULONG(x, y) (((((u32)(x)&0xFFFFFFFFU)>>(u32)((y)&31)) | ((u32)(x)<<(u32)(32-((y)&31)))) & 0xFFFFFFFFU)

#define ENDIAN_REVERSE_ULONG(dwS)	( (ROTL_ULONG((dwS),  8) & 0x00ff00ff) | (ROTL_ULONG((dwS), 24) & 0xff00ff00) )
#define BIG_B2D(B, D)		D = ENDIAN_REVERSE_ULONG(*(ulong_ptr)(B))
#define BIG_D2B(D, B)		*(u32* )(B) = ENDIAN_REVERSE_ULONG(D)
#define LITTLE_B2D(B, D)	D = *(u32*)(B)
#define LITTLE_D2B(D, B)	*(u32* )(B) = (u32)(D)
#define GetData(x)	ENDIAN_REVERSE_ULONG(x)

#define RR(x, n)		ROTR_ULONG(x, n)
#define SS(x, n)		(x >> n)

#define Ch(x, y, z)		((x & y) ^ ((~x) & z))
#define Maj(x, y, z)	((x & y) ^ (x & z) ^ (y & z))
#define Sigma0(x)		(RR(x,  2) ^ RR(x, 13) ^ RR(x, 22))
#define Sigma1(x)		(RR(x,  6) ^ RR(x, 11) ^ RR(x, 25))

#define RHO0(x)			(RR(x,  7) ^ RR(x, 18) ^ SS(x,  3))
#define RHO1(x)			(RR(x, 17) ^ RR(x, 19) ^ SS(x, 10))

#define FF(a, b, c, d, e, f, g, h, j) {							\
	T1 = h + Sigma1(e) + Ch(e, f, g) + SHA256_K[j] + X[j];		\
	d += T1;													\
	h = T1 + Sigma0(a) + Maj(a, b, c);							\
}

void SHA256_Transform(u32* Message, u32* ChainVar)
{
	u32 a = 0, b = 0, c = 0, d = 0, e = 0, f = 0, g = 0, h = 0, T1 = 0, X[64] = {0x00};
	u32 j = 0;

	for (j = 0; j < 16; j++)
	{
		X[j] = GetData(Message[j]);
	}
	
	for (j = 16; j < 64; j++)
	{
		X[j] = RHO1(X[j - 2]) + X[j - 7] + RHO0(X[j - 15]) + X[j - 16];
	}

	a = ChainVar[0];
	b = ChainVar[1];
	c = ChainVar[2];
	d = ChainVar[3];
	e = ChainVar[4];
	f = ChainVar[5];
	g = ChainVar[6];
	h = ChainVar[7];

	for (j = 0; j < 64; j += 8)
	{
		FF(a, b, c, d, e, f, g, h, j + 0);
		FF(h, a, b, c, d, e, f, g, j + 1);
		FF(g, h, a, b, c, d, e, f, j + 2);
		FF(f, g, h, a, b, c, d, e, j + 3);
		FF(e, f, g, h, a, b, c, d, j + 4);
		FF(d, e, f, g, h, a, b, c, j + 5);
		FF(c, d, e, f, g, h, a, b, j + 6);
		FF(b, c, d, e, f, g, h, a, j + 7);
	}

	ChainVar[0] += a;	ChainVar[1] += b;
	ChainVar[2] += c;	ChainVar[3] += d;
	ChainVar[4] += e;	ChainVar[5] += f;
	ChainVar[6] += g;	ChainVar[7] += h;

	a = 0; b = 0; c = 0; d = 0; e = 0; f = 0; g = 0; h = 0; T1 = 0; j = 0;
	memset(X, 0x00, 64);

}

void SHA256_Init(SHA256_INFO* Info)
{
	memset(Info, 0x00, sizeof(SHA256_INFO));

	Info->chainVar[0] = 0x6a09e667;
	Info->chainVar[1] = 0xbb67ae85;
	Info->chainVar[2] = 0x3c6ef372;
	Info->chainVar[3] = 0xa54ff53a;
	Info->chainVar[4] = 0x510e527f;
	Info->chainVar[5] = 0x9b05688c;
	Info->chainVar[6] = 0x1f83d9ab;
	Info->chainVar[7] = 0x5be0cd19;

	Info->HighLength = Info->LowLength = 0;
}

void SHA256_Process(SHA256_INFO* Info,
	const u8* pszMessage,
	u32 uDataLen)
{
	if ((Info->LowLength += (uDataLen << 3)) < 0)
		Info->HighLength++;

	Info->HighLength += (uDataLen >> 29);

	while (uDataLen >= SHA256_BLOCKLEN)
	{
		memcpy((u8*)Info->szBuffer, pszMessage,
			(u32)SHA256_BLOCKLEN);
		SHA256_Transform((u32*)Info->szBuffer, (u32*) Info->chainVar);
		pszMessage += SHA256_BLOCKLEN;
		uDataLen -= SHA256_BLOCKLEN;
	}

	memcpy((u8*)Info->szBuffer, pszMessage, uDataLen);
}

void SHA256_Close(SHA256_INFO* Info, u8* pszDigest)
{
	u32 i = 0, Index = 0;

	Index = ((Info->LowLength) >> 3) % SHA256_BLOCKLEN;
	Info->szBuffer[Index++] = 0x80;

	if (Index > (SHA256_BLOCKLEN - 8))
	{
		memset((u8*)Info->szBuffer + Index, 0, (u32)(SHA256_BLOCKLEN - Index));
		SHA256_Transform((u32*)Info->szBuffer, (u32*) Info->chainVar);
		memset((u8*)Info->szBuffer, 0, (u32)SHA256_BLOCKLEN - 8);
	}
	else
	{
		memset((u8*)Info->szBuffer + Index, 0, (u32)(SHA256_BLOCKLEN - Index - 8));
	}

	Info->LowLength = ENDIAN_REVERSE_ULONG(Info->LowLength);
	Info->HighLength = ENDIAN_REVERSE_ULONG(Info->HighLength);

	((u32*)Info->szBuffer)[(SHA256_BLOCKLEN / 4) - 2] = Info->HighLength;
	((u32*)Info->szBuffer)[(SHA256_BLOCKLEN / 4) - 1] = Info->LowLength;

	SHA256_Transform((u32*)Info->szBuffer, (u32*) Info->chainVar);

	for (i = 0; i < SHA256_DIGESTLEN; i += 4)
	{
		BIG_D2B((Info->chainVar)[i / 4], &(pszDigest[i]));
	}
	i = 0; Index = 0;
}

void SHA256_Clear(SHA256_INFO* Info)
{
	memset(Info, 0x00, sizeof(SHA256_INFO));
}

//! TODO: Set LONGTEST_MAXLEN 51200 after debugging 
#define LONGTEST_MAXLEN 51200

void SHA256_Encrypt(const u8* pszMessage, u32 uPlainTextLen, u8* pszDigest)
{
	//! Stack Buffer Overflow
	u8 buf[LONGTEST_MAXLEN] = {0x00,};

	//! Heap Buffer Overflow
	// u8* buf = NULL;
	// buf = (u8*)calloc(uPlainTextLen, sizeof(u8));

	memcpy(buf, pszMessage, uPlainTextLen);

	SHA256_INFO info = {0x00,};
	SHA256_Init(&info);
	SHA256_Process(&info, buf, uPlainTextLen);
	SHA256_Close(&info, pszDigest);
	SHA256_Clear(&info);

	//! Heap Buffer Overflow
	// if(buf) free(buf);
}

// EOF

