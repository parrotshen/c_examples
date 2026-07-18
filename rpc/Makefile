all:
	make -f Makefile.hello

clean:
	rm -f hello_client hello_server *.o

distclean:
	make -f Makefile.hello clean
	rm -f Makefile.hello

rpcgen:
	rpcgen -C -a hello.x
	cp -f patch/Makefile.hello .
	cp -f patch/hello_client.c .
	cp -f patch/hello_server.c .
