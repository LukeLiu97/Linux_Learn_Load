#ifndef _LIST_H_
#define _LIST_H_

//#include <stdio.h>
#include <stdlib.h>
//#include <string.h>

typedef struct Node_
{
	void *data;
	struct Node_ *next;
}Node;

typedef struct List_
{
	int size;
	Node *head;
	Node *tail;
}List;


void initList(List *list);
int insertList(List *list,Node *prevNode,void *data);
int removeList(List *list,Node *prevNode,void **data);

#endif
