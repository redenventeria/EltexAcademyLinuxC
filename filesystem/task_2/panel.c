#include<stdio.h>
#include<dirent.h>
#include<malloc.h>
#include<string.h>
#include<ncurses.h>
#include<errno.h>

typedef struct {
  WINDOW *win;
  char *loc;
  struct dirent **content;
  int content_n;
  int entry_i;
} mc_panel;

void newmc_pan(
    mc_panel **pan,
    const char* loc,
    int nlines,
    int ncols,
    int begin_y,
    int begin_x)
{
  *pan = malloc(sizeof(mc_panel));

  (*pan)->win = newwin(nlines, ncols, begin_y, begin_x);

  (*pan)->loc = malloc(sizeof(char) * sizeof(loc));
  strncpy((*pan)->loc, loc, sizeof(loc));

  (*pan)->entry_i = 0;

  (*pan)->content_n = scandir((*pan)->loc, &(*pan)->content, NULL, alphasort);
}

void freemc_pan(mc_panel **pan)
{
  for(size_t i = 0; i < (*pan)->content_n; i++)
  {
    free((*pan)->content[i]);
  }
  free((*pan)->loc);
  delwin((*pan)->win);
  free(*pan);
  *pan = NULL;
}

void display_dir(mc_panel *pan) {
  for(size_t i = 0; i < pan->content_n; i++)
    wprintw(pan->win, "%s\n", pan->content[i]->d_name);

}
int main()
{
  initscr();
  mc_panel *pan;
  newmc_pan(&pan, ".", 50, 50, 0, 0);
  wmove(pan->win, 0, 0);
 // for(int i = 0; i < 20*20; i++)
 //   wprintw(pan->win, "A");
  display_dir(pan);
  wgetch(pan->win);
  endwin();
  printf("%d", errno);
}

//int get_dir(struct dirent **list)
//{
//  int n = scandir(".", &list, NULL, alphasort);
//
//  for(size_t i = 0; i < n; i++)
//    printf("%s\n", list[i]->d_name);
//
//}
