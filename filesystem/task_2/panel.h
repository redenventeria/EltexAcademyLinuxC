typedef struct {
  WINDOW *win;
  char *loc;
  struct dirent **content;
  int content_n;
} mc_panel;

void newmc_pan(
    mc_panel **pan,
    const char* loc,
    int nlines,
    int ncols,
    int begin_y,
    int begin_x);


void freemc_pan(mc_panel **pan);



