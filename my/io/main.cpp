#include"file.h"
#include<string.h>

int main()
{
	string filename = "put";
	char buff[200]="www.baidu.com";
	int ret;
	File file(filename,O_RDONLY | O_TRUNC);

	ret = file.open();
	if(ret == 0)
	{
		perror("open");
		return -1;
	}

	ret = file.write(buff,strlen(buff));
	if(ret < 0)
	{
		perror("write");
		return -1;
	}

	return 0;
}
