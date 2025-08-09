#include<stdio.h>
#include<curses.h>

#include"panel.h"

#define PAN_N 2

void init(mc_panel **panels)
{
  initscr();
  noecho();
  cbreak();
  keypad(stdscr, 1);
  start_color();
  curs_set(0);

  int y, x; getmaxyx(stdscr, y, x);
  newmc_pan(&panels[0], "/", y, x / 2, 0, 0);
  newmc_pan(&panels[1], "/", y, x - x / 2, 0, x / 2);
  keypad(panels[0]->win, 1);
  keypad(panels[1]->win, 1);
}

int main()
{
    mc_panel *panels[PAN_N];
    int active_pan = 0;
    init(panels);
    for(size_t i = 0; i < PAN_N; i++)
      display_dir(*panels[i]);
    refresh();

    int command = ' ';
    while (command != 'q')
    {
        command = wgetch(panels[active_pan]->win);
        switch(command) {
            case 'w':
                active_pan = (active_pan + 1) % PAN_N;
                break;
            case KEY_UP:
                if (panels[active_pan]->entry_i > 0)
                    panels[active_pan]->entry_i--;
                break; 
            case KEY_DOWN:
                if(panels[active_pan]->entry_i < panels[active_pan]->content_n - 1)
                    panels[active_pan]->entry_i++;
                break;
            case 'e':
                move_to_dir(panels[active_pan]);
                break;
        }
        clear();
        for(size_t i = 0; i < PAN_N; i++)
            display_dir(*panels[i]);
        refresh();
    }
    endwin();

    freemc_pan(&panels[0]);
    freemc_pan(&panels[1]);

    return 0;
}
