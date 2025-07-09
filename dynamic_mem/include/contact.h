#ifndef _INCLUDE_CONTACT_H_
#define _INCLUDE_CONTACT_H_

#define LEN 10

typedef struct {
	char name[LEN];
	char second_name[LEN];
	char tel[LEN];
} contact;

void new_contact(contact *buf, const char *name, const char *sec_name, const char *tel);
void str_contact(char *buf, contact *c);

int is_name(const char *buf);
int is_tel(const char *buf);

#endif /* ifndef _INCLUDE_CONTACT_H_ */
