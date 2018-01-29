#ifndef __LIST_H
#define __LIST_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>


struct list
{
	void *data;
	struct list *next;
};

struct list *createNode(void *data,int length);
struct list *addList(struct list *head,struct list *p);
void showList(struct list *head,void(*show)(void*));
void showNode(struct list *p,void(*show)(void*));
struct list *searchList(struct list *head,void *data,int(*compare)(void*,void*));
struct list *deleteList(struct list *head,void *data,int(*compare)(void*,void*));
struct list *reversal(struct list *head);
void freeList(struct list *head);


#endif
