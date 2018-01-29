#include <stdio.h>
#include <string.h>

int main()
{
	/*请注意内存不要越界
	 *memset:设置一片内存空间的值，一般用来清零
	 *bzero: 用来清零
	 *memcpy: 复制内存(内存拷贝)
	 */

	//memset(str, 0, sizeof(str)) ---> bzero(str, sizeof(str));
	char str[20];

	memset(str, 'a', sizeof(str));

	for (int i = 0; i < 20; i++)
	{
		printf("%c", str[i]);
	}
	putchar('\n');

	int num = 100;
	int number;
	memcpy(&number, &num, sizeof(num));
	printf("number = %d\n", number);

	char pstr[21];
	memcpy(pstr, str, sizeof(str));
	pstr[20] = 0;
	printf("pstr = %s\n", pstr);
	return 0;
}
