#include<stdio.h>
#include<dirent.h>
#include<malloc.h>
#include<ncurses.h>

typedef struct {
  WINDOW *win;
  char *loc;
  char **contents;
} mc_panel;

int get_dir(struct dirent **list)
{
  int n = scandir(".", &list, NULL, alphasort);

  for(size_t i = 0; i < n; i++)
    printf("%s\n", list[i]->d_name);

}


