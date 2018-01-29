#include"list.h"

int compare(int *key,int *n)
{
	return (*key==*n);
}

void show(int *n)
{
	printf("%d\n",*n);
	return;
}

int main()
{
	int num;
	struct list *head=NULL;
	struct list *p=NULL;
	printf("请输入一个数字:\n");
	while(1)
	{
		scanf("%d",&num);
		if(num==-1)
			break;

		p=createNode(&num,sizeof(num));
		head=addList(head,p);
	}

	showList(head,(void*)show);
	printf("\n");

	printf("请输入查找的数字:\n");
	scanf("%d",&num);
	p=searchList(head,&num,(void*)compare);
	if(p!=NULL)
	{
		printf("查找到的数字:\n");
		showNode(p,(void*)show);
	}
	printf("\n");

	printf("请输入删除的数字:\n");
	scanf("%d",&num);
	head=deleteList(head,&num,(void*)compare);
	printf("删除后的数字:\n");
	showList(head,(void*)show);

	printf("数字的倒序:\n");
	head=reversal(head);
	showList(head,(void*)show);
	freeList(head);
	return 0;
}
