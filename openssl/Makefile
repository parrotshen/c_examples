CC = gcc

APPS = aes128_ecb aes128_cbc aes128_cfb128 aes128_ofb128
APPS += sha1 sha256
APPS += https ssl_socket

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

sha1: sha1.c
	$(CC) $< -Wall -lcrypto -o $@

sha256: sha256.c
	$(CC) $< -Wall -lcrypto -o $@

https: https.c
	$(CC) $< -Wall -lssl -o $@

ssl_socket: ssl_socket.c
	$(CC) $< -Wall -lssl -o $@

clean:
	rm -f $(APPS)

.PHONY: all clean
