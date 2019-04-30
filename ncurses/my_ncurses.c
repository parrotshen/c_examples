#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <curses.h>

static WINDOW *_win = NULL;

int init_conio(void)
{
    _win = initscr();

    if (NULL == _win)
    {
        fprintf(stderr, "ERR: fail to initial ncurses\n\n");
        return -1;
    }

    keypad(stdscr, TRUE);

    noecho();

    return 0;
}

void uninit_conio(void)
{
    delwin( _win );

    endwin();

    refresh();
}

void gotoxy(int x, int y)
{
    wmove(_win, y, x);

    wrefresh( _win );
}

int wherex(void)
{
    int x = 0;
    int y = 0;

    getyx(_win, y, x);

    return x;
}

int wherey(void)
{
    int x = 0;
    int y = 0;

    getyx(_win, y, x);

    return y;
}


int main(int argc, char *argv[])
{
    int x = 10;
    int y = 20;

    if (0 == init_conio())
    {
        printw("goto (%d, %d)\n", x, y);
        refresh();

        sleep(1);

        gotoxy(x, y);

        attron( A_STANDOUT );
        printw("Hello, world!\n");
        attroff( A_STANDOUT );
        refresh();

        sleep(1);

        printw("now at (%d, %d)\n", wherex(), wherey());
        refresh();

        uninit_conio();
    }

    return 0;
}

