##动态库
#main:main.c /usr/lib/liblist.so
#	gcc main.c -o main -llist -L.
#/usr/lib/liblist.so:list.o
#	gcc -shared list.o -o /usr/lib/liblist.so
#list.o:list.c
#	gcc -c list.c -o list.o -fPIC

#静态库
main:main.c liblist.a
	gcc main.c -o main -llist -L.
liblist.a:list.o
	ar -rc liblist.a list.o
list.o:list.c
	gcc -c list.c -o list.o
.PHONY:clean
clean:
	rm ./list.o ./liblist.a ./main

.PHONY:precompile
precompile:main.c
	gcc -E main.c -o main.i
	