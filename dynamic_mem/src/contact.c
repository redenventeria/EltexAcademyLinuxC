#include<stdio.h>
#include<string.h>

#include"../include/contact.h"

void  new_contact(contact *buf, const char *name, const char *second_name, const char *tel) {

	strncpy(buf->name, name, LEN);
	strncpy(buf->second_name, second_name, LEN - 1);
	strncpy(buf->tel, tel, LEN - 1);

}

void str_contact(char *buf, contact *c)
{
  
	sprintf(buf, "name:%s sec_name:%s tel:%s\n", c->name, c->second_name, c->tel);
}

int is_name(const char *buf)
{
  return 1 <= strlen(buf) && strlen(buf) <= 9;
}

int is_tel(const char *buf)
{
  if(!(1 <= strlen(buf) && strlen(buf) <= 9))
      return 0;
  
  while(*buf != '\0') {
    if(!('0' <= *buf && *buf <= '9'))
      return 0;
    buf++;
  }
  return 1;
}


