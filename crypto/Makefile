CC = gcc

all: aead_aes_gcm skcipher_aes_ecb skcipher_aes_cbc skcipher_aes_ctr

aead_aes_gcm: aead_aes_gcm.c
	$(CC) $< -Wall -o $@

skcipher_aes_ecb: skcipher_aes_ecb.c
	$(CC) $< -Wall -o $@

skcipher_aes_cbc: skcipher_aes_cbc.c
	$(CC) $< -Wall -o $@

skcipher_aes_ctr: skcipher_aes_ctr.c
	$(CC) $< -Wall -o $@

clean:
	rm -f aead_aes_gcm skcipher_aes_ecb skcipher_aes_cbc skcipher_aes_ctr
