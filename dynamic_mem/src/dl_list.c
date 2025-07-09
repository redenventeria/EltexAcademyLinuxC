#include<stdlib.h>
#include<errno.h>
#include<malloc.h>
#include<string.h>



#include"../include/dl_list.h"



void new_list(dl_list *list)
{
  list->head = NULL;
  list->tail = NULL;
  list->len = 0;
}



void add_tail(dl_list *list, void *elem, size_t elem_size)
{

  // Allocating memory for new elem and node

	dl_list_node *new = (dl_list_node*)malloc(sizeof(dl_list_node));
	
	if (new == NULL) {
		perror("Out of memory!\n");
		exit(errno);
	}

	void *elem_copy = malloc(elem_size);

	if (elem_copy == NULL) {
		perror("Out of memory!\n");
		exit(errno);
	}

  // Creating new node
  
  dl_list_node *tail_copy = list->tail;

	memcpy(elem_copy, elem, elem_size);
	new->elem = elem_copy;
	new->elem_size = elem_size;
  new->prev = list->tail;
  new->next = NULL;

  // Adding information about new node to list

	if(list->head == NULL)
    list->head = new;
  if(list->tail != NULL)
    list->tail->next = new;
  list->tail = new;
  list->len++;

	return;
}



void delete_node(dl_list *list, dl_list_node *node)
{
  // Updating list info

  if(list->head == node)
    list->head = node->next;
  if(list->tail == node)
    list->tail = node->prev;
  list--;

  // Modifying node neighbours so structure is preserved

	if(node->prev != NULL)
		node->prev->next = node->next;
	if(node->next != NULL)
		node->next->prev = node->prev;


	free(node->elem);
	node->elem = NULL;
	node->prev = NULL;
	node->next = NULL;
  free(node);
}



void find_all_nodes(dl_list_node **buf, size_t buf_len, dl_list *list, void *value, comp eq)
{
	dl_list_node *node = list->head;
  size_t i = 0;

  while(i < buf_len - 1 && node != NULL)
  {
    if(eq(value, node->elem)) {
      buf[i] = node;
      i++;
    }
    node = node->next;
  
  }

  buf[i] = NULL;
  

}



void free_list(dl_list *list)
{
	dl_list_node *node = list->head;
	
	while(node != NULL) {
		dl_list_node *t = node->next;
		free(node->elem);
		free(node);
		node = t;
  }
  
  list->head = NULL;
  list->tail = NULL;
  list->len = 0;
}



int int_eq(int *a, int *b)
{
	return *a == *b;
}
