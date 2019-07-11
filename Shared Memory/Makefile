all:client server
client: testcase.c LpcProxy.c LpcProxy.h Lpc.h
	gcc -o client testcase.c LpcProxy.c
server: FileServer.c Lpc.h LpcStub.h LpcStub.c
	gcc -o server FileServer.c LpcStub.c -lpthread
clean:
	rm -rf *.o

