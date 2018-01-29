#include <stdio.h>
#include <string.h>

int main()
{
/*	int num;
	char *str = "12345www.baidu.com";

	sscanf(str, "%d", &num);

	printf("num = %d\n", num);
	*/

	
	char *str = "zhangsan, 20, F, 1234567@163.com";
	int age;
	char name[20];
	sscanf(str, "%[^,]", name);
	sscanf(str, "%*s%d", &age);
	printf("name = %s, age = %d\n", name, age);
	

	/*
	char *str = "zhangsan, 20, F, 1234567@163.com";
	char name[20];

//	sscanf(str, "%*[^,]%*c%*[^,]%*c%*[^,]%*c%s", name);
	sscanf(str, "%*s%*s%*s%s", name);
	printf("tail = %s\n", name);
	*/
/*
	char *str = "123abc456";
	char arr[20];
	sscanf(str, "%[0-9a-c]", arr);
	printf("arr = %s\n", arr);
*/
	return 0;
}
