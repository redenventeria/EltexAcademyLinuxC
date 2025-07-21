#include"panel.h"

const int INACTIVE_COLOR = 1;
const int ACTIVE_COLOR = 2;

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

  init_pair(INACTIVE_COLOR, COLOR_WHITE, COLOR_BLACK);
  init_pair(ACTIVE_COLOR, COLOR_BLACK, COLOR_WHITE);
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

  wmove(pan->win, 0, 0);
  wattron(pan->win, COLOR_PAIR(INACTIVE_COLOR));  
  for(size_t i = 0; i < pan->content_n; i++)
    wprintw(pan->win, "%s\n", pan->content[i]->d_name);
  wattroff(pan->win, COLOR_PAIR(INACTIVE_COLOR));  

  wmove(pan->win, 4, 0);
  wattron(pan->win, COLOR_PAIR(ACTIVE_COLOR));
  wprintw(pan->win, "%s\n", pan->content[4]->d_name);
  wattroff(pan->win, COLOR_PAIR(ACTIVE_COLOR));
}

int main()
{
  initscr();
  start_color();
  mc_panel *pan;
  newmc_pan(&pan, ".", 49, 50, 0, 0);
  wmove(pan->win, -1, 0);
 // for(int i = -1; i < 20*20; i++)
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
