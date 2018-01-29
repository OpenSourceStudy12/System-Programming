#include<stdio.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/sem.h>

union semun
{
	int val;/*value for SETVAL*/
	struct semid_ds *buf;/*buffer for IPC_STAT,IPC_SET*/
	unsigned short *array;/*array for GETALL,SETALL*/
	struct seminfo *_buf;/*buffer for IPC_INFO*/	
};

/***对信号量数组semnum编号的信号量做P操作***/
int P(int semid,int semnum)
{
	struct sembuf sops={semnum,-1,SEM_UNDO};
	return (semop(semid,&sops,1));
}


/***对信号量数组semnum编号的信号量做V操作***/

int V(int semid,int semnum)
{
	struct sembuf sops={semnum,+1,SEM_UNDO};
	return(semop(semid,&sops,1));
}


int main(int argc,char **argv)
{
	int key;
	int semid,ret;
	union semun arg;
	struct sembuf semop;
	int flag;


	key=ftok("/tmp",0x66);
	if(key<0)
	{
		perror("ftok key error\n");
		return -1;
	}

/***本程序创建了三个信号量，实际使用时只用了一个0号信号量***/
	semid=semget(key,3,IPC_CREAT|0600);
	if(semid==-1)
	{
		perror("create semget error\n");
		return;
	}
	if(argc==1)
	{
		arg.val=1;
		/***对0号信号量设置初始值***/
		ret=semctl(semid,0,SETVAL,arg);
		if(ret<0)
		{
			perror("ctl sem error\n");
			semctl(semid,0,IPC_RMID,arg);
			return -1;
		}
	}
/***取0号信号量的值***/

}
