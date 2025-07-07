#include<stdlib.h>
#include<errno.h>
#include<malloc.h>
#include<string.h>



#include"dl_list.h"



dl_list* new_list(void *elem, size_t elem_size)
{
	dl_list *list = (dl_list*)malloc(sizeof(dl_list));	
		
	if (list == NULL) {
		perror("Out of memory!\n");
		exit(errno);
	}
	
	void *elem_copy = malloc(elem_size);
	memcpy(elem_copy, elem, elem_size);
	list->elem = elem_copy;

	list->elem_size = elem_size;
	list->prev = NULL;
	list->next = NULL;
	
	return list;
}



void add_tail(dl_list *list, void *elem, size_t elem_size)
{
	
	if(list->next != NULL) {
		add_tail(list->next, elem, elem_size);
		return;
	}

	dl_list *new = (dl_list*)malloc(sizeof(dl_list));
	
	if (list == NULL) {
		perror("Out of memory!\n");
		exit(errno);
	}
	
	void *elem_copy = malloc(elem_size);
	memcpy(elem_copy, elem, elem_size);
	new->elem = elem_copy;

	new->elem_size = elem_size;
	new->prev = list;
	new->next = NULL;
	
	list->next = new;
	
	return;
}



void delete_node(dl_list *list)
{

	if(list->prev != NULL)
		list->prev->next = list->next;
	if(list->next != NULL)
		list->next->prev = list->prev;

	free(list->elem);

	list->elem = NULL;
	list->prev = NULL;
	list->next = NULL;

	free(list);
	list = NULL;
}



void find_all_nodes(dl_list **buf, size_t n, dl_list *list, void *value, comp eq)
{
	size_t i = 0;

	dl_list *cur = list;
	while(i < n - 1 && cur != NULL) {
		if(eq(value, cur->elem)) {	
			buf[i] = cur;
			i++;
		}
		cur = cur->next;
	}
	
	if(list != NULL)
		cur = list->prev;
	while(i < n - 1 && cur != NULL) {
		if(eq(value, cur->elem)) {	
			buf[i] = cur;
			i++;
		}
		cur = cur->prev;
	}
	
	buf[i] = NULL;
}



void free_list(dl_list *list)
{
	dl_list *cur_n = list;
	dl_list *cur_p = NULL;
	if(list != NULL)  
		cur_p = list->prev;
	
	while(cur_n != NULL) {
		dl_list *t = cur_n->next;
		free(cur_n->elem);
		free(cur_n);
		cur_n = t;
	}

	while(cur_p != NULL) {
		dl_list *t = cur_p->prev;
		free(cur_p->elem);
		free(cur_p);
		cur_p = t;
	}

}



int int_eq(int *a, int *b)
{
	return *a == *b;
}



#include<stdio.h>
int main()
{
	int a = 12;
	dl_list *l = new_list(&a, sizeof(a));
	
	int b = 15;
	int c = 111;
	int d = 98;
	
	add_tail(l, &b, sizeof(int));
	add_tail(l, &c, sizeof(int));
	add_tail(l, &d, sizeof(int));	
	dl_list **buf = malloc(sizeof(dl_list*) * 10);
	find_all_nodes(buf, 10, l, &c, (comp)int_eq);
	
	size_t i = 0;
	while(buf[i] != NULL)
		i++;
	printf("%d\n", *(int*)(buf[0]->elem));
	delete_node(buf[0]);

	find_all_nodes(buf, 10, l, &b, (comp)int_eq);
	
	printf("%d\n", *(int*)(buf[0]->elem));
	
	free(buf);
	free_list(l);	
}
