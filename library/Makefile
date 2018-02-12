SUBDIR  = archive
SUBDIR += dynamic
SUBDIR += export
SUBDIR += initial
SUBDIR += preload
SUBDIR += shared
SUBDIR += static
SUBDIR += weak

all:
	@for i in $(SUBDIR) ; do \
        make -C $$i ; \
    done

clean:
	@for i in $(SUBDIR) ; do \
        make -C $$i clean ; \
    done

.PHONE: $(SUBDIR)
