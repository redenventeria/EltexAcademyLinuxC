#include<stdio.h>
#include<string.h>


#include"contact.h"



void  new_contact(contact *buf, char *name, char *second_name, char *tel) {

	strncpy(buf->name, name, LEN - 1);
	buf->name[LEN - 1] = '\0';

	strncpy(buf->second_name, second_name, LEN - 1);
	buf->second_name[LEN - 1] = '\0';

	strncpy(buf->tel, tel, LEN - 1);
	buf->tel[LEN - 1] = '\0';

}

void copy_contact(contact *to, contact *from)
{
	memcpy(to, from, sizeof(contact));	
}

void print_contact(contact *c)
{
	printf("name:%s sec_name:%s tel:%s\n", c->name, c->second_name, c->tel);
}

int main()
{
	contact c;
	new_contact(&c, "abc", "def", "0");
	contact d;
	copy_contact(&d, &c);
	print_contact(&c);
	print_contact(&d);
	
	return 0;
}
