#include<stdio.h>
#include<stdlib.h>
#include<string.h>



const char menu[] = "1) Добаить абонента\n2) Удалить абонента\n3) Поиск всех абонентов по имени\n4) Вывод всех записей\n5) Выход\nВведите команду: ";
const char menu_failure[] = "Такой команды нет, попробуйте ещё раз";

const char name[] = "Введите имя: ";
const char name_failure = "Имя должно содержать не более 9 символов!";


const char second_name[] = "Введите фамилию: ";
const char second_name_failure[] = "Фамилия должна содержать не более 9 символов!";


const char tel[] = "Введите Номер: ";
const char tel_failure = "Номер должен содержать не более 9 символов и состоять из цифр!";



#define BUF_SIZE 20

typedef struct {
	char name[10];
	char second_name[10];
	char tel[10];
} abonent;



char get_val_input(char *buf, char *prompt, char *prompt_failure, int (*validator)(char*))
{
	do {
		printf("%s", prompt);
		fgets(buf, BUF_SIZE, stdin);

		if(buf[strlen(buf) - 1] != '\n')
			while(getchar() != '\n') {}

		if (!valiator(buf))
			printf("%s\n\n", prompt_failure);

	} while (!(validator(buf));
}


 
#define MAX_BOOK_SIZE
abonent m[MAX_BOOK_SIZE];
size_t count_ab = 0;



int is_command(char *buf)
{
	return strlen(buf) == 2 && '1' <= buf[0] && buf[0] <= '5';
}

int is_name(char *buf)
{
	return strlen(buf) <= 9;
}

int is_tel(char *buf)
{
	return 1;
}



void add_abonent();
void del_abonent();
void search_abonent();
void print_all_abonents();



int main()
{	
	
	char command[BUF_SIZE];

	while(command != '5')
	{
		command = get_val_input(command, menu, "Такой команды нет, попробуйте ещё раз", &is_command);

		switch(command) {
			case '1':
				add_abonent(); break;
			case '2':
				del_abonent(); break;
			case '3':
				search_abonent(); break;
			case '4':
				print_all_abonents(); break;
			case '5':
				break;
			default:
				printf("ERROR: unreachable branch!");
		}
		printf("\n");
	}

	if(command == '5')
		printf("OK, closing...\n");
	else
		printf("ERROR: branch is unreachable!");

}



void add_abonent()
{
	if(count_ab == MAX_BOOK_SIZE) {
		printf("В спраочнике не осталось места, добавление новых элементов невозможно!\n\n");
		return;
	}

	printf();
}
