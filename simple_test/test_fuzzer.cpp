#include <stddef.h>
#include <stdint.h>

void vuln(uint8_t* data, size_t size) {
  if (size > 0 && data[0] == 'F')
    if (size > 1 && data[1] == 'u')
      if (size > 2 && data[2] == 'z')
        if (size > 3 && data[3] == 'z')
          __builtin_trap();
}

extern "C" int LLVMFuzzerTestOneInput(uint8_t* data, size_t size) {
  vuln(data, size);
  return 0;
}

// EOF

