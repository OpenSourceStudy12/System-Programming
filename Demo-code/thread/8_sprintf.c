#include <stdio.h>

int main()
{
	/*
	char buf[200];
	int num = 10;

	sprintf(buf, "hello%d", num);
	printf("%s\n", buf);
	
	char *str = "zhangsan 100";
	sscanf(str, "%s%d", buf, &num);
	printf("%s, %d\n", buf, num);
	*/
/*
	char *name = "zhangsan, 20, M, 1234567@163.com";
	char str[20];

	sscanf(name, "%[^,]", str);
	printf("%s\n", str);
*/
	/*
	char *name = "zhangsan, 20, M, 1234567@163.com";
	char str[20];
	int num;

	sscanf(name, "%[^,]%*c%d", str, &num);
	printf("%s num = %d\n", str, num);
	*/
	/*
	char *name = "zhangsan, 20, M, 1234567@163.com";
	char str[20];

//	sscanf(name, "%*[^,]%*c%*[^,]%*c%*[^,]%*c%s", str);
	sscanf(name, "%*s%*s%*s%s", str);
	printf("%s\n", str);
	*/


	char *str = "abc123xyz";
	char name[20];
//	sscanf(str, "%[a-c1-3x-z]", name);
	sscanf(str, "%[a-z1-3]", name);
	printf("%s\n", name);
}
