#include "Metamorphic_Fuzz_Test.h"
#include "KISA_SHA256.h"
#include "def.h"

int SHA256_Metamorphic_Test(const uint8_t *data, size_t size)
{
	//* 하나의 테스트 벡터에 대해 3가지 Metamorphic Testing을 진행
	//* 하나의 테스트 벡터에 대해 하나의 Metamorphic Testing도 진행해 봐야할듯

	SHA256_Bit_Contribution_Test(data, size);
	SHA256_Exclusion_Test(data, size);
	SHA256_Update_Test(data, size);

	return 0;
}

//! Bit Contribution Test
void SHA256_Bit_Contribution_Test(const uint8_t *data, size_t size)
{
	uint8_t *msg_buffer = NULL;
	uint8_t digest_test_buffer[2][SHA256_DIGEST_LEN] = {0x00,};

	//* set parameters
	msg_buffer = (uint8_t *)calloc(size, sizeof(uint8_t));
	assert(msg_buffer != NULL);

	//* Origin Case
	SHA256_Encrypt(data, size, digest_test_buffer[0]);

	//* Test Case
	for (uint32_t cnt_i = 0; cnt_i < size * 8; cnt_i++)
	{
		memset(msg_buffer, 0x00, size);
		memset(digest_test_buffer[1], 0x00, SHA256_DIGEST_LEN);

		memcpy(msg_buffer, data, size);

		msg_buffer[cnt_i / 8] ^= (0x01 << (cnt_i % 8));

		SHA256_Encrypt(msg_buffer, size, digest_test_buffer[1]);

		if (!memcmp(digest_test_buffer[0], digest_test_buffer[1], SHA256_DIGEST_LEN))
		{
			//* Metamorphic Error
#ifdef ADDRESS_SANITIZER
			printf("[-] Bit Contribution Test Failed!\n");
#endif
#ifdef METAMORPHIC_TESTING
			printf("[-] Bit Contribution Test Failed!\n");
#endif
			__builtin_trap();
		}
	}
	if (msg_buffer != NULL) free(msg_buffer);
}

//! Bit Exclusion Test
void SHA256_Exclusion_Test(const uint8_t *data, size_t size)
{
	uint8_t *msg_buffer = NULL;
	uint8_t digest_test_buffer[2][SHA256_DIGEST_LEN] = {0x00,};

	//* set parameters
	msg_buffer = NULL;
	msg_buffer = (uint8_t *)calloc(size + HASH_EXCLUSION_BYTELEN, sizeof(uint8_t));
	assert(msg_buffer != NULL);

	//* Origin Case 
	SHA256_Encrypt(data, size, digest_test_buffer[0]);

	//* Test Case 
	for (uint32_t cnt_i = 0; cnt_i < HASH_EXCLUSION_BYTELEN * 8; cnt_i++)
	{
		memset(msg_buffer, 0x00, size + HASH_EXCLUSION_BYTELEN);
		memset(digest_test_buffer[1], 0x00, SHA256_DIGEST_LEN);

		memcpy(msg_buffer, data, size);

		msg_buffer[size + cnt_i / 8] ^= (0x01 << (cnt_i % 8));

		SHA256_Encrypt(msg_buffer, size, digest_test_buffer[1]);

		if (memcmp(digest_test_buffer[0], digest_test_buffer[1], SHA256_DIGEST_LEN))
		{
			//* Metamorphic Error
#ifdef ADDRESS_SANITIZER
			printf("[-] Bit Exclusion Test Failed!\n");
#endif
#ifdef METAMORPHIC_TESTING
			printf("[-] Bit Exclusion Test Failed!\n");
#endif
			__builtin_trap();
		}
	}
	if (msg_buffer != NULL) free(msg_buffer);
}

//! Update Test
void SHA256_Update_Test(const uint8_t *data, size_t size)
{
	SHA256_INFO ctx = {0x00, };					 
	uint8_t *msg_fragment1 = NULL; 
	uint8_t *msg_fragment2 = NULL; 
	uint8_t digest_test_buffer[2][SHA256_DIGEST_LEN] = {	0x00,};
	uint32_t msg_frag1Len;
	uint32_t msg_frag2Len;

	//* Origin Case
	SHA256_Encrypt(data, size, digest_test_buffer[0]);

	for (uint32_t cnt_i = 1; cnt_i <= size; (cnt_i ++))
	{
		msg_frag1Len = cnt_i ;
		msg_frag2Len = size - msg_frag1Len;

		msg_fragment1 = NULL;
		msg_fragment2 = NULL;
		
		msg_fragment1 = (uint8_t *)calloc(msg_frag1Len, sizeof(uint8_t));
		assert(msg_fragment1 != NULL);
		msg_fragment2 = (uint8_t *)calloc(msg_frag2Len, sizeof(uint8_t));
		assert(msg_fragment2 != NULL);

		memcpy(msg_fragment1, data, msg_frag1Len);
		memcpy(msg_fragment2, data + msg_frag1Len, msg_frag2Len);

		//* Test Case
		SHA256_Init(&ctx);
		SHA256_Process(&ctx, msg_fragment1, msg_frag1Len);
		SHA256_Process(&ctx, msg_fragment2, msg_frag2Len);
		SHA256_Close(&ctx, digest_test_buffer[1]);

		if (memcmp(digest_test_buffer[0], digest_test_buffer[1], SHA256_DIGEST_LEN))
		{
#ifdef ADDRESS_SANITIZER
			printf("[-] Update Test Failed!\n");
#endif
#ifdef METAMORPHIC_TESTING
			printf("[-] Update Test Failed!\n");
#endif
			__builtin_trap();
		}
		if (msg_fragment1 != NULL) free(msg_fragment1);
		if (msg_fragment2 != NULL) free(msg_fragment2);
	}
}
