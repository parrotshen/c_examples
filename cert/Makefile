CC = gcc
CFLAGS += -Wall
CFLAGS += -DOPENSSL_NO_KRB5
CFLAGS += -lssl
CFLAGS += -lcrypto

all: convert_cert create_cert verify_pem verify_der get_pubkey get_prikey

convert_cert: convert_cert.c
	$(CC) $(CFLAGS) $< -o $@

create_cert: create_cert.c
	$(CC) $(CFLAGS) $< -o $@

verify_pem: verify_pem.c
	$(CC) $(CFLAGS) $< -o $@

verify_der: verify_der.c
	$(CC) $(CFLAGS) $< -o $@

get_pubkey: get_pubkey.c
	$(CC) $(CFLAGS) $< -o $@

get_prikey: get_prikey.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f convert_cert create_cert verify_pem verify_der get_pubkey get_prikey
