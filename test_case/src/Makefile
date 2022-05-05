CC     = ${HOME}/metamorphic-fuzz-testing/clang-llvm/bin/clang
# CFLAGS = -fsanitize=address # -g -Wall -O3  #! Consider -O3 option
# TARGET = ${Target}    #! Consider Target Name
OBJS   = main.o Metamorphic_Fuzz_Test.o KISA_SHA256.o

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

all: $(TARGET)

main.o: main.c
	$(CC) $(CFLAGS) -c -o main.o main.c

Metamorphic_Fuzz_Test.o : Metamorphic_Fuzz_Test.c
	$(CC) $(CFLAGS) -c -o Metamorphic_Fuzz_Test.o Metamorphic_Fuzz_Test.c

KISA_SHA256.o : KISA_SHA256.c
	$(CC) $(CFLAGS) -c -o KISA_SHA256.o KISA_SHA256.c

clean:
	rm $(OBJS) $(TARGET)

# EOF

