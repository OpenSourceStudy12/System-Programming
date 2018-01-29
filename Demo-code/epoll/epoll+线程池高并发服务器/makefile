OBJ:=epoll.o pool.o read_client.o
OPT:=-c -I ../inc -g 
all:$(OBJ)
	gcc $(OBJ) -o all -pthread
epoll.o:epoll.c
	gcc $(OPT) epoll.c 
pool.o:pool.c
	gcc $(OPT) pool.c 
read_client.o:read_client.c
	gcc $(OPT) read_client.c


clean:
	rm -rf *.o all
