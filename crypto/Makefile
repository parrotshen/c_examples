SUB_DIR += aes128
SUB_DIR += sha256
SUB_DIR += snow3g

all clean:
	@for i in $(SUB_DIR) ; do \
        make -sC $$i $@ ; \
    done

