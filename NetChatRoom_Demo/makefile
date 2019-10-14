NetChatRoom:main.o client.o server.o global.o
	gcc main.o global.o client.o server.o -o NetChatRoom -pthread
main.o:main.c
	gcc -c main.c -o main.o
client.o:client.c
	gcc -c client.c -o client.o
server.o:server.c
	gcc -c server.c -o server.o
global.o:global.c
	gcc -c global.c -o global.o

.PHONY:clean
clean:
	rm NetChatRoom main.o client.o server.o global.o
