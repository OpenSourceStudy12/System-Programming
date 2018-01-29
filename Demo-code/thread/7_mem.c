#include <stdio.h>
#include <string.h>

struct student
{
	int id;
	char name[20];
};

int main()
{
/*
	char str[20];
	memset(str, 97, sizeof(str));

	for (int i = 0; i < 20; i++)
	{
		putchar(str[i]);
	}
	putchar('\n');
	return 0;
*/
/*
	char str[20];

	while (1)
	{
//		memset(str, 0, sizeof(str));
		bzero(str, sizeof(str));
		scanf("%s", str);

		for (int i = 0; i < 20; i++)
		{
			putchar(str[i]);
		}
		putchar('\0');
	}
	*/
/*
	char str1[20] = "hello world";
	char str2[20];
	memcpy(str2, str1, sizeof(str1));

	for (int i = 0; i < 20; i++)
	{
		putchar(str2[i]);
	}
	putchar('\n');

	int num1 = 10;
	int num2;
	memcpy(&num2, &num1, sizeof(num1));
	printf("%d\n", num2);
	*/

	struct student stu = {101, "zhangsan"};
	char buf[200];
	int num = 10;
	struct student s;
	int num2;

	memcpy(buf, &stu, sizeof(stu));
	memcpy(buf+sizeof(stu), &num, sizeof(num));
	
	memcpy(&s, buf, sizeof(s));
	memcpy(&num2, buf+sizeof(s), sizeof(num2));

	printf("\t%d\t%s\t %d\n", s.id, s.name, num2);
	return 0;
}
