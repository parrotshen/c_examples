CC = gcc

APPS = aes128_ecb aes128_cbc aes128_cfb128 aes128_ofb128

all: $(APPS)
	@strip $^

aes128_ecb: aes128_ecb.c
	$(CC) $< -Wall -lcrypto -o $@

aes128_cbc: aes128_cbc.c
	$(CC) $< -Wall -lcrypto -o $@

aes128_cfb128: aes128_cfb128.c
	$(CC) $< -Wall -lcrypto -o $@

aes128_ofb128: aes128_ofb128.c
	$(CC) $< -Wall -lcrypto -o $@

clean:
	rm -f $(APPS)

.PHONY: all clean
