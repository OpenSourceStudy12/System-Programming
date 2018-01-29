#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
typedef struct stu
{
	int num;
	char name[20];
	char sex;
}Stu;

int main()
{
	int fd,ret;	
	Stu stu;

	fd = open("io",O_WRONLY | O_CREAT |O_TRUNC,0640);
	if(fd < 0)
	{
		perror("open");
		return -1;
	}
	for(int i=0;i<5;i++)
	{
		scanf("%d %s %c",&stu.num,stu.name,&stu.sex);
		ret = write(fd,&stu,sizeof(Stu));
		if(ret < 0)
		{
			perror("write");
			return -1;
		}
	}

	fprintf(stdout,"write success\n");
	
	


	return 0;

}
