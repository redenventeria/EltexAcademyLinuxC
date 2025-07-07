
#define LEN 10

typedef struct {
	char name[LEN];
	char second_name[LEN];
	char tel[LEN];
} contact;

void new_contact(contact *buf, char *name, char *sec_name, char *tel);
void copy_contact(contact *to, contact *from);
