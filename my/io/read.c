#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>

typedef struct student
{
	int num;
	char name[20];
	char sex;
}Stu;

int main()
{
	int fd,ret;
	Stu stu;
	fd = open("io",O_RDONLY);
	if(fd < 0)
	{
		perror("open");
		return -1;
	}
	for(;;)
	{
		ret = read(fd,&stu,sizeof(Stu));
		if(ret < 0)
		{
			perror("read");
			return -1;
		}
		else if(ret == 0)
			break;
		printf("%d %s %c\n",stu.num,stu.name,stu.sex);
	}
	printf("read succes\n");
	return 0;
}
