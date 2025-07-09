#include"../include/phone_book.h"

void run_app()
{
phone_book b; new_book(&b);
  
	//test_fill();
	char command[BUF_SIZE] = {'\0'};
    
	while(command[0] != '5')
	{
		get_val_input(command, BUF_SIZE, stdin, is_command, menu_prompt, menu_failure);

		switch(command[0]) {
			case '1':
				add_abonent(&b); break;
			case '2':
				delete_abonent(&b); break;
			case '3':
				search_abonent(&b); break;
			case '4':
				print_all_abonents(&b); break;
			case '5':
        break;
			default:
				printf("ERROR: unreachable branch!");
		}
		printf("\n");
	}

  free_book(&b);

	if(command[0] == '5')
		printf("OK, закрыаем...\n");
	else
		printf("ERROR: unreachable branch!");

}

int is_confirm(const char *buf)
{
  return strlen(buf) == 1 && (buf[0] == 'y' || buf[0] == 'n');
}

int is_command(const char *buf)
{
  return strlen(buf) == 1 && ('1' <= buf[0] && buf[0] <= '5');
}
 


void new_book(phone_book *b)
{
  new_list(b);
}



void add_abonent(phone_book *b)
{
	contact c;
  get_val_input(c.name, LEN, stdin, is_name, name_prompt, name_failure);
  get_val_input(c.second_name, LEN, stdin, is_name, second_name_prompt, second_name_failure);
  get_val_input(c.tel, LEN, stdin, is_tel, tel_prompt, tel_failure);

  char confirm[BUF_SIZE]; get_val_input(confirm, BUF_SIZE, stdin, is_confirm, confirm_prompt, confirm_failure);

  if (confirm[0] == 'y')  
    add_tail(b, &c, sizeof(contact));

}



int contact_eq(contact *a, contact *b) {
	return !strncmp(a->name, b->name, LEN);
}



void search_abonent(phone_book *b)
{

  contact c; get_val_input(c.name, LEN, stdin, is_name, name_prompt, name_failure);
  dl_list_node *buf[MAX_MATCHES];
  char str_buf[LONG_BUF];

  find_all_nodes(&buf, MAX_MATCHES, b, &c, (comp)contact_eq);
  for(size_t i = 0; buf[i] != NULL; i++) {
    str_contact(str_buf, buf[i]->elem);
    if(contact_eq(&c, buf[i]->elem)) {
      printf("%s", str_buf);
    }
  }
}



void delete_abonent(phone_book *b)
{
  contact c; get_val_input(c.name, LEN, stdin, is_name, name_prompt, name_failure);
  dl_list_node *buf[MAX_MATCHES];
  char str_buf[BUF_SIZE];
  char confirm[BUF_SIZE];

  find_all_nodes(&buf, MAX_MATCHES, b, &c, (comp)contact_eq);
  
  for(size_t i = 0; buf[i] != NULL; i++) {
    if(contact_eq(&c, buf[i]->elem)) {

      str_contact(str_buf, buf[i]->elem);
      printf("Найден контакт с данными %s\n Удалить?", str_buf);
      get_val_input(confirm, BUF_SIZE, stdin, is_confirm, confirm_prompt, confirm_failure);
      
      if(confirm[0] == 'y')
        delete_node(b, buf[i]);
    }
  }
}



void print_all_abonents(phone_book *b)
{
  dl_list_node *node = b->head;
  char buf[LONG_BUF];

  while(node != NULL) {
    str_contact(buf, node->elem);
    printf("%s\n", buf);
    node = node->next;
  }
}

void free_book(phone_book *b)
{
  free_list(b);
}



