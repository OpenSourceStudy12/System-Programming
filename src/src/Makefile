OCJ = chef_client.o chef_func.o package.o commen.o
OPJ = waiter_client.o waiter_func.o package.o commen.o
OFJ = server.o server_func.o mysql.o pool.o package.o commen.o
OAJ = admin_client.o admin_menu.o admin_func.o package.o commen.o
OBJ = -c -I ../inc/ -g

all: server admin_client  waiter_client chef_client

server:$(OFJ)
	gcc $(OFJ) -o server -lmysqlclient -pthread

admin_client:$(OAJ)
	gcc $(OAJ) -o admin_client

waiter_client:$(OPJ)
	gcc $(OPJ) -o waiter_client

chef_client:$(OCJ)
	gcc $(OCJ) -o chef_client

server.o:server.c
	gcc $(OBJ) server.c

server_func.o:server_func.c 
	gcc $(OBJ) server_func.c  

mysql.o:mysql.c
	gcc $(OBJ) mysql.c 

pool.o:pool.c
	gcc $(OBJ) pool.c 

admin_client.o:admin_client.c
	gcc $(OBJ) admin_client.c

admin_menu.o:admin_menu.c
	gcc $(OBJ) admin_menu.c

admin_func.o:admin_func.c
	gcc $(OBJ) admin_func.c

waiter_client.o:waiter_client.c
	gcc $(OBJ) waiter_client.c  

waiter_func.o:waiter_func.c
	gcc $(OBJ) waiter_func.c

chef_client.o:chef_client.c
	gcc $(OBJ) chef_client.c

chef_func.o:chef_func.c
	gcc $(OBJ) chef_func.c

package.o:package.c
	gcc $(OBJ) package.c

commen.o:commen.c
	gcc $(OBJ) commen.c

clean: 
	rm *.o server waiter_client
