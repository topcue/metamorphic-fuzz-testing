#include "def.h"
#include "KISA_SHA256.h"
#include "Metamorphic_Fuzz_Test.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

int SHA256_Metamorphic_Test(const uint8_t* data, size_t size);

//! TEST_LEN should bigger than 51200
#define TEST_LEN 51200

/*
 * Address Sanitizer
 */
#ifdef ADDRESS_SANITIZER
void print_testcase(uint8_t* data, size_t size) {
	printf("[*] test case:\n");
	for (size_t i = 0; i < size; i++) {
		printf("0x%02X ", data[i]);
		if (i % 16 == 15)
			printf("\n");
	}
	printf("\n");
}

size_t read_testcase(char* path, uint8_t** buffer) {
	FILE* fp = fopen(path, "rb");
	fseek(fp, 0L, SEEK_END);
	long file_len = ftell(fp);
	*buffer = NULL;
	*buffer = (uint8_t*)malloc(sizeof(uint8_t) * file_len);

	fseek(fp, 0L, SEEK_SET);
	fgets((char*)*buffer, file_len, fp);
	fclose(fp);

	return file_len;
}

int main(int argc, char* argv[]) {
	uint8_t* data = NULL;
	size_t size = -1;
	
	if (argc < 2) {
		printf("[-] argc < 2\n");
		return -1;
	}
	
	size = read_testcase(argv[1], &data);
	printf("[*] size: %zu\n", size);
	// print_testcase(data, size);
	
	uint8_t digest[TEST_LEN] = {0x00,};

	printf("[*] Call SHA256_Encrypt()\n");
	SHA256_Encrypt(data, size, digest);

	printf("[*] Call SHA256_Metamorphic_Test()\n");
	SHA256_Metamorphic_Test(data, size);
	
    free(data);
}
#endif


/*
 * CAVP Test Only
 */
#ifdef CAVP
//! TODO: CAVP here
#endif


/*
 * Metamorphic Test Only
 */
#ifdef METAMORPHIC_TESTING
int main()
{
	//! 논문 eval 파트에서 MT는 실제 MT 테스트 벡터를 파일에서 가져와서 돌려야 하는거 아닌가?
	uint8_t data[TEST_LEN] = { 0x00, };
	SHA256_Metamorphic_Test(data, TEST_LEN);

	return 0;
}
#endif


/*
 * Fuzz Test Only
 */
#ifdef FUZZ_TESTING
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
	uint8_t digest[TEST_LEN] = {0x00,};
	SHA256_Encrypt(data, size, digest);

    return 0;
}
#endif


#ifdef METAMORPHIC_FUZZ_TESTING
/*
 * Metamorphic Fuzz Test
 */
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
	if (size < 8) {
		return 0;
	}
	// uint8_t data[TEST_LEN] = {0x00,};
	SHA256_Metamorphic_Test(data, size);

    return 0;
}
#endif

// EOF
