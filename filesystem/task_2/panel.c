#include<dirent.h>
#include<stdlib.h>
#include<limits.h>

#include"panel.h"


const int INACTIVE_COLOR = 1;
const int ACTIVE_COLOR = 2;

#define MAX_PATH_SIZE 1024

void newmc_pan(
    mc_panel **pan,
    const char* loc,
    int nlines,
    int ncols,
    int begin_y,
    int begin_x)
{
  *pan = malloc(sizeof(mc_panel));

  (*pan)->win = subwin(stdscr, nlines, ncols, begin_y, begin_x);

  (*pan)->loc = malloc(MAX_PATH_SIZE);
  strncpy((*pan)->loc, loc, MAX_PATH_SIZE);

  (*pan)->entry_i = 0;

  (*pan)->content = NULL;
  (*pan)->content_n = scandir((*pan)->loc, &(*pan)->content, NULL, alphasort);

  init_pair(INACTIVE_COLOR, COLOR_WHITE, COLOR_BLACK);
  init_pair(ACTIVE_COLOR, COLOR_BLACK, COLOR_WHITE);
}

void freemc_pan(mc_panel **pan)
{

  free((*pan)->loc);
  delwin((*pan)->win);
  free(*pan);
  *pan = NULL;
}

void display_dir(mc_panel pan) {

  wmove(pan.win, 0, 0);  
  for(size_t i = 0; i < pan.content_n; i++)
  {
    if(i == pan.entry_i) {
        wattron(pan.win, COLOR_PAIR(ACTIVE_COLOR));
        wprintw(pan.win, "%s\n", pan.content[i]->d_name);
        wattroff(pan.win, COLOR_PAIR(ACTIVE_COLOR));
    }
    else {
        wattron(pan.win, COLOR_PAIR(INACTIVE_COLOR));
        wprintw(pan.win, "%s\n", pan.content[i]->d_name);
        wattroff(pan.win, COLOR_PAIR(INACTIVE_COLOR));
    }
  }

  refresh();
}

void move_to_dir(mc_panel *pan)
{
    struct dirent **content = pan->content;
    if(pan->content[pan->entry_i]->d_type != DT_DIR)
      return;
    
    strncat(pan->loc, "/", MAX_PATH_SIZE);
    strncat(pan->loc, content[pan->entry_i]->d_name, MAX_PATH_SIZE);

    char *newloc = malloc(MAX_PATH_SIZE);
    realpath(pan->loc, newloc);
    
    free(pan->loc);
    pan->loc = NULL;
    pan->loc = newloc;
    newloc = NULL;

    for(size_t i = 0; i < pan->content_n; i++)
    {
        free(pan->content[i]);
        pan->content[i] = NULL;
    }
    free(pan->content);
    pan->content = NULL;

    pan->content_n = scandir(pan->loc, &pan->content, NULL, alphasort);
    pan->entry_i = 0;
}

//int main()
//{
//  initscr();
//  start_color()
//  mc_panel *pan;
//  newmc_pan(&pan, ".", 49, 50, 0, 0);
//  wmove(pan->win, -1, 0);
// // for(int i = -1; i < 20*20; i++)
// //   wprintw(pan->win, "A");
//  display_dir(pan);
//  wgetch(pan->win);
//  endwin();
//  printf("%d", errno);
//}
//
//int get_dir(struct dirent **list)
//{
//  int n = scandir(".", &list, NULL, alphasort);
//
//  for(size_t i = 0; i < n; i++)
//    printf("%s\n", list[i]->d_name);
//
//}
