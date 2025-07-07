#include<string.h>



#include"include/dl_list.h"
#include"includei/contact.h"

#define MAX_MATCHES 100

phone_book* new_book(contact *c)
{
	dl_list *list = new_list(c, sizeof(contact));
	
	return list;
}

void add_abonent(phone_book *b, char *n, char *s_n, char *t)
{
	contact c = new_contact()
	add_tail(b, c, sizeof(contact));
}

int contact_eq(contact a, contact b)
	return strncmp(a.name, b.name, LEN);

void search_abonent(phone_book *b, contact *c)
{
    dl_list **buf = malloc(MAX_MATCHES*sizeof(dl_list*));
    find_all_nodes(buf, MAX_MATCHES, b, c, contact_eq);
    

}

void delete_abonent(phone_book *b)
{

}

