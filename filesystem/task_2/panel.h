#include<curses.h>
#include<stdio.h>
#include<dirent.h>
#include<malloc.h>
#include<string.h>
#include<errno.h>

extern const int INACTIVE_COLOR;
extern const int ACTIVE_COLOR;

typedef struct {
  WINDOW *win;
  char *loc;
  struct dirent **content;
  int content_n;
  size_t entry_i;
} mc_panel;

void newmc_pan(
    mc_panel **pan,
    const char* loc,
    int nlines,
    int ncols,
    int begin_y,
    int begin_x);

void freemc_pan(mc_panel **pan);

void display_dir(mc_panel *pan);

