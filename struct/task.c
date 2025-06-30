#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>



// set of prompt strings used for getting user input
const char menu_prompt[] = "1) Добаить абонента\n2) Удалить абонента\n3) Поиск всех абонентов по имени\n4) Вывод всех записей\n5) Выход\nВведите команду: ";
const char menu_failure[] = "Такой команды нет, попробуйте ещё раз";

const char name_prompt[] = "Введите имя: ";
const char name_failure[] = "Имя должно содержать не более 9 символов!";


const char second_name_prompt[] = "Введите фамилию: ";
const char second_name_failure[] = "Фамилия должна содержать не более 9 символов!";


const char tel_prompt[] = "Введите номер: ";
const char tel_failure[] = "Номер должен содержать не более 9 символов и состоять из цифр!";

const char confirm_prompt[] = "Подтвердить изменения(y/n)? ";
const char confirm_failure[] = "Такой опции нет, попробуйте ещё раз.";



// Main struct and array.
typedef struct {
	char name[10];
	char second_name[10];
	char tel[10];
} abonent;

#define BUF_SIZE 20
#define MAX_BOOK_SIZE 100
abonent book[MAX_BOOK_SIZE];
size_t count_ab = 0;



/* 
 * Function for processing user input.
 * Safely reads input to buf, eats all excess characters until '\n' is found.
 * Repeatedly tries to get correct input valiated by validator 
 * function until getting correct string (accoring to validator function)
 * Arguments prompt an promt_failure are used to give feeback.
 */
void get_val_input(char *buf, const char *prompt, const char *prompt_failure, int (*validator)(char*))
{
	do {
		printf("%s", prompt);
		fgets(buf, BUF_SIZE, stdin);

		if(buf[strlen(buf) - 1] != '\n')
			while(getchar() != '\n') {}

		buf[strcspn(buf, "\n")] = '\0';

		if (!(*validator)(buf))
			printf("%s\n\n", prompt_failure);

	} while (!(*validator)(buf));
}



// Validation functions.
int is_command(char *buf)
{
	return strlen(buf) == 1 && '1' <= buf[0] && buf[0] <= '5';
}

int is_name(char *buf)
{
	return strlen(buf) <= 9;
}

int is_tel(char *buf)
{
	for(size_t i = 0; buf[i] != '\0'; i++)
		if (!isdigit(buf[i]))
			return 0;

	return 1;
}

int is_confirm(char *buf)
{
	return strlen(buf) == 1 && (buf[0] == 'y' || buf[0] == 'n');
}



// Function declarations.
void add_abonent();
void delete_abonent();
void search_abonent();
void print_all_abonents();
void test_fill();



// Main function.
int main()
{
	//test_fill();
	
	char command[BUF_SIZE] = {'\0'};

	while(command[0] != '5')
	{
		get_val_input(command, menu_prompt, menu_failure, &is_command);

		switch(command[0]) {
			case '1':
				add_abonent(); break;
			case '2':
				delete_abonent(); break;
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

	if(command[0] == '5')
		printf("OK, закрыаем...\n");
	else
		printf("ERROR: unreachable branch!");

}



// Provides interactive prompt to user insertion.
void add_abonent()
{
	if(count_ab == MAX_BOOK_SIZE) {
		printf("В спраочнике не осталось места, добавление новых элементов невозможно!\n\n");
		return;
	}

	char name[BUF_SIZE]; get_val_input(name, name_prompt, name_failure, &is_name);
	char second_name[BUF_SIZE]; get_val_input(second_name, second_name_prompt, second_name_failure, &is_name);
	char tel[BUF_SIZE]; get_val_input(tel, tel_prompt, tel_failure, &is_tel);
	
	char confirm[BUF_SIZE]; get_val_input(confirm, confirm_prompt, confirm_failure, &is_confirm);

	//printf("%s %s %s %s\n", name, second_name, tel, confirm);

	if (confirm[0] == 'y') {
		strcpy(book[count_ab].name, name);
		strcpy(book[count_ab].second_name, second_name);
		strcpy(book[count_ab].tel, tel);
		count_ab++;
		printf("Изменения сохранены!\n\n");	
	}
	else
		printf("OK, отмена.\n\n");

	return;
}



// Provides interactive prompt for user deletion.
void delete_abonent()
{
	char name[BUF_SIZE]; get_val_input(name, name_prompt, name_failure, &is_name);
	
	for(size_t i = 0; i < count_ab; i++) {

		if(!strcmp(name, book[i].name)) {
			printf("Найден пользоатель с данными:\nИмя: %s Фамилия: %s Номер: %s\nУдалить?\n", book[i].name, book[i].second_name, book[i].tel);

			char confirm[BUF_SIZE]; get_val_input(confirm, confirm_prompt, confirm_failure, &is_confirm);
			if(confirm[0] == 'y') {
				memset(book[i].name, 0, 10);
				memset(book[i].second_name, 0, 10);
				memset(book[i].tel, 0, 10);
			}
		}
	}

}




// Provides interactive prompt for user search.
void search_abonent()
{
	char name[BUF_SIZE]; get_val_input(name, name_prompt, name_failure, &is_name);
	
	for(size_t i = 0; i < count_ab; i++)
		if(!strcmp(name, book[i].name))
			printf("Найден пользоатель с данными:\nИмя: %s Фамилия: %s Номер: %s\n", book[i].name, book[i].second_name, book[i].tel);
}




// Prints entire user base.
void print_all_abonents()
{
	for(size_t i = 0; i < count_ab; i++)
		if(book[i].name[0] != '\0')	
			printf("Имя: %s Фамилия: %s Номер: %s\n", book[i].name, book[i].second_name, book[i].tel);


}



// Testing function, fills entire array with random entries.
void test_fill()
{
	count_ab = MAX_BOOK_SIZE;
	for(size_t i = 0; i < count_ab; i++)
	{
		book[i].name[0] = 'a' + rand() % 26;
		book[i].second_name[0] = 'a' + rand() % 26;
		book[i].tel[0] = '0' + rand() % 10;
	}
}
