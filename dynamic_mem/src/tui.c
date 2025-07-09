#include"../include/tui.h"

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



void pretty_fgets(char *restrict buf, size_t n, FILE *restrict stream)
{
		char *err = fgets(buf, n, stream);

    if(err == NULL) {
      perror("Failed to read string!\n");
      exit(-1);
    }

		if(buf[strlen(buf) - 1] != '\n')
			while(getchar() != '\n') {}

		buf[strcspn(buf, "\n")] = '\0';
}



void get_val_input(char *restrict buf, size_t n, FILE *restrict stream, buf_validator *is_valid, const char *prompt, const char *failure)
{
  printf("%s", prompt);
	do {
		pretty_fgets(buf, n, stream);

		if(!is_valid(buf))
			printf("%s\n", failure);
	} while(!is_valid(buf));
}
