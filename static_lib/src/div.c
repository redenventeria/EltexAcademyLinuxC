#include"div.h"

float division(int a, int b)
{
  if(b == 0) {
    perror("Division by zero!\n");
    errno = EDOM;
    exit(EXIT_FAILURE);
  }

  return (float)a / b;
}
