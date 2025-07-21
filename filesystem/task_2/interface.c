#include<stdio.h>
#include<ncurses.h>
#include<panel.h>

#DEFINE PAN_N 2

void init(ma_panel **panels)
{
  initscr();
  panels = malloc(sizeof(mc_panel) * PAN_N);

  int y, x; getmaxxy(stdscr, y, x);
  nmc_pan(panels[0], ".", y, x / 2, 0, 0);
  nmc_pan(panels[1], ".", y, x - x / 2, 0, x / 2);
}
