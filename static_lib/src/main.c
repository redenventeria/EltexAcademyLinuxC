#include<string.h>
#include<stdio.h>

#include"add.h"
#include"sub.h"
#include"mul.h"
#include"div.h"

const char menu[] = "1) Сложение\n2) Вычитание\n3) Умножение\n4) Деление\n5) Выход\n";

int main()
{
  char *cmd = NULL;
  size_t n = 0;

  char *nums = NULL;
  size_t num_len = 0;
  int a, b;

  do {

    printf("%s", menu);

    do {
      printf("Пожалуйста, введите номер одной из команд: ");
      ssize_t err = getline(&cmd, &n, stdin);
      cmd[strcspn(cmd, "\n")] = 0;

      if(err == -1) {
        perror("Error, unable to read a string!");
        exit(EXIT_FAILURE);
      }

    } while(!((strlen(cmd) == 1) && ('1' <= cmd[0] && cmd[0] <= '5')));

    if(cmd[0] == '5')
      break;

    do {
      printf("Введите два числа, разделённые пробелом: ");
      getline(&nums, &num_len, stdin);
    } while(sscanf(nums, "%d %d", &a, &b) != 2);

    switch (cmd[0]) {
      case '1':
        printf("%d + %d = %d\n", a, b, add(a, b));
        break;
      case '2':
        printf("%d - %d = %d\n", a, b, sub(a, b));
        break;
      case '3':
        printf("%d * %d = %d\n", a, b, mul(a, b));
        break;
      case '4':
        printf("%d / %d = %f\n", a, b, division(a, b));
        break;
      case '5':
        break;
      default:
        printf("Branch is ureachable!\n");
        break;
    }

  } while(cmd[0] != '5');
  
  free(cmd);
  free(nums);
}
