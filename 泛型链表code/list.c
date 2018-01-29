#include"list.h"

//泛型链表的创建节点，插入链表，查找链表，打印链表，释放链表.

//创建泛型链表的节点
struct list *createNode(void *data,int length)
{
	void *temp=malloc(length);
	assert(temp!=NULL);
	memcpy(temp,data,length);
	struct list *p=malloc(sizeof(struct list));
	assert(p!=NULL);
	p->data=temp;
	p->next=NULL;
	return p;
}


//插入泛型链表
struct list *addList(struct list *head,struct list *p)
{
	struct list *s=NULL;
	if(head==NULL)
	{
		head=p;
		return head;
	}

	s=head;
	while(s->next!=NULL)
		s=s->next;
	s->next=p;

	return head;
}

/*打印整个泛型链表
	1,head为泛型链表头指针
	2,show为指向数据结构的一个自定义打印函数,该函数打印数据结构的成员;
*/
void showList(struct list *head,void(*show)(void*))
{
	struct list *p=head;
	while(p!=NULL)
	{
		show(p->data);
		p=p->next;
	}
	return ;
}

/*打印泛型链表中的一个节点
	1,p为泛型链表节点
	2,show为指向数据结构的一个自定义打印函数,该函数打印数据结构的成员;
*/
void showNode(struct list *p,void(*show)(void*))
{
	show(p->data);
	return;
}

//查找泛型链表
/*按照值查找泛型链表中的一个节点，返回查找到的节点指针
	1,head为泛型链表头指针
	2,data是需要查找值的引用
	3,compare一个函数指针,指向自定义的一个比较函数
		(1)参数1为关键值类型的引用,参数2为数据结构的引用
		(2)查找的值与数据结构引用中的成员值相等，返回真,否则假
*/
struct list* searchList(struct list *head,void *data,int(*compare)(void*,void*))
{
	struct list *s=head;
	while(s!=NULL)
	{
		if(compare(data,s->data))
			break;
		s=s->next;
	}
	return s;
}


//删除泛型链表的一个节点
/*按照值删除泛型链表中的一个节点，返回头指针
	1,head为泛型链表头指针
	2,data是需要删除值的引用
	3,compare一个函数指针,指向自定义的一个比较函数
		(1)参数1为关键值类型的引用,参数2为数据结构的引用
		(2)查找的值与数据结构引用中的成员值相等，返回真,否则假
*/
struct list *deleteList(struct list *head,void *data,int(*compare)(void*,void*))
{
	struct list *s,*p;
	p=head;
	if(compare(data,head->data))
	{
		s=head;
		head=head->next;
		free(s->data);
		free(s);
		return head;
	}

	while(p->next!=NULL)
	{
		if(compare(data,p->next->data))
		{
			s=p->next;
			p->next=s->next;
			free(s->data);
			free(s);
			break;
		}
		p=p->next;
	}
	return head;
}

/*链表的倒序*/
struct list *reversal(struct list *head)
{
	struct list *newhead=NULL;
	struct list *p;
	if(head==NULL)
		return head;
	while(head->next!=NULL)
	{
		p=head->next;
		head->next=newhead;
		newhead=head;
		head=p;
	}
	head->next=newhead;
	newhead=head;
	return newhead;

}

//释放泛型链表
void freeList(struct list *head)
{
	struct list *p=NULL;
	while(head!=NULL)
	{
		p=head->next;
		//先释放数据成员
		free(head->data);
		//再释放链表的节点
		free(head);
		head=p;
	}
	return;
}
