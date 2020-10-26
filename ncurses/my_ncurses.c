#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <curses.h>


static WINDOW *_win = NULL;

int init_conio(void)
{
    /* start curses mode */
    _win = initscr();

    if (NULL == _win)
    {
        fprintf(stderr, "ERR: fail to initial ncurses\n\n");
        return -1;
    }

    /* enable function keys */
    keypad(stdscr, TRUE);

    /* don't echo */
    noecho();

    return 0;
}

void uninit_conio(void)
{
    delwin( _win );

    /* end curses mode */
    endwin();
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

void show_message(int x, int y, char *pStr)
{
    char position[32];

    sprintf(position, "Cursor at (%2d, %2d)", x, y);
    mvprintw(0, 0, position);

    attron( A_STANDOUT );
    mvprintw(y, x, pStr);
    attroff( A_STANDOUT );

    wmove(_win, y, x);
    wrefresh( _win );
}

void clean_message(int x, int y, int len)
{
    int i;

    for (i=0; i<len; i++)
    {
        mvdelch(y, x);
    }
}

int main(int argc, char *argv[])
{
    char message[] = "Hello, world!";
    int height = 0; /* max y */
    int width = 0;  /* max x */
    int x = 0;
    int y = 1;
    int ch;

    if (0 == init_conio())
    {
        getmaxyx(_win, height, width);
        mvprintw((height - 1), 0, "Press 'q' to exit");
        height -= 2;
        width -= strlen(message);


        show_message(x, y, message);

        while ((ch = getch()) != 'q')
        {
            switch ( ch )
            {
                case KEY_LEFT:
                    clean_message(x, y, strlen(message));
                    if (x > 0) x--;
                    show_message(x, y, message);
                    break;
                case KEY_RIGHT:
                    clean_message(x, y, strlen(message));
                    if (x < width) x++;
                    show_message(x, y, message);
                    break;
                case KEY_UP:
                    clean_message(x, y, strlen(message));
                    if (y > 1) y--;
                    show_message(x, y, message);
                    break;
                case KEY_DOWN:
                    clean_message(x, y, strlen(message));
                    if (y < height) y++;
                    show_message(x, y, message);
                    break;
            }
        }

        uninit_conio();
    }

    return 0;
}

