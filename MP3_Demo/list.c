#include "list.h"

void initList(List *list)
{
	list->size = 0;
	list->head = NULL;
	list->tail = NULL;	

	return ;
}

int insertList(List *list,Node *prevNode,void *data)
{
	if(list == NULL)
	{
//		printf("传入链表为空\n");
		return -1;
	}

	Node *tempNode = (Node *)malloc(sizeof(Node));

	if(tempNode == NULL)
	{
//		printf("无法申请堆空间\n");
		return -1;
	}
	else
	{
		tempNode->data = data;	
	}

	/* 传入的前向节点为空,取代原有头节点*/
	if(prevNode == NULL)
	{
		tempNode->next = list->head;
		list->head = tempNode;
	}
	else
	{
		tempNode->next = prevNode->next;
		prevNode->next = tempNode;
	}
	
	if(tempNode->next == NULL)
	{
		list->tail = tempNode;
	}

	list->size++;

	return 0;
}

int removeList(List *list,Node *prevNode,void **data)
{
	if(list == NULL)
	{
//		printf("链表不存在\n");
		return -1;
	}

	Node *tempNode;
	
	if(list->size <= 0 || list->head == NULL || list->tail == NULL)
	{
//		printf("链表数据异常\n");
		return -1;
	}

	if(prevNode == NULL)
	{
//		printf("移除头节点\n");
		tempNode = list->head;	
		list->head = tempNode->next;
	}
	else
	{
		tempNode = prevNode->next;
		prevNode->next = tempNode->next;
	}
	
	list->tail = tempNode->next;

	*data = tempNode->data;

	list->size--;

	free(tempNode);

	return 0;
}


///* 链表测试 */
//#define LENGTH 10
//
//int main()
//{
//	List list;
//
//	initList(&list);
//
//	char *strArray[LENGTH] = {
//		"line1",
//		"line2",	
//		"line3",	
//		"line4",	
//		"line5",	
//		"line6",	
//		"line7",	
//		"line8",	
//		"line9",	
//		"line10"};
//
//	int i;
//
//	for(i = 0;i<LENGTH;i++)
//	{
//		insertList(&list,list.tail,(void *)strArray[i]);
//	}
//
//	Node *curNode = list.head;	
//
//	while(curNode != NULL)
//	{
//		printf("%s\n",(char *)(curNode->data));
//		curNode = curNode->next;
//	}
//	
//	char *string;	
//
//	do
//	{
//		printf("List length:%d\n",list.size);
//		removeList(&list,NULL,(void **)&string);
//		printf("String:%s\n",string);	
//	}while(list.head != NULL);
//	
//	return 0;
//}
