#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>


#define MAZE_COL  35  /* number of X */
#define MAZE_ROW  27  /* number of Y */
#define MAZE_SIZE 945


               /*  up  down  left  right */
int  NeighX[4] = {  0,    0,   -1,     1 };
int  NeighY[4] = { -1,    1,    0,     0 };

int  Wall[MAZE_SIZE];
int  WallNum = 0;

char Cells[MAZE_SIZE];
int  X[MAZE_SIZE];
int  Y[MAZE_SIZE];


int Random(int min, int max)
{
    struct timeval tv;
    int val;

    gettimeofday(&tv, NULL);

    srand( (unsigned int)tv.tv_usec );
    val = ((rand() % (max - min + 1)) + min);

    return val;
}

void AddWallList(int indexC)
{
    int indexW;
    int new;
    int x, y;
    int n;
    int w;

    for (n=0; n<4; n++)
    {
        x = X[indexC] + NeighX[n];
        y = Y[indexC] + NeighY[n];
        indexW = (x + (y * MAZE_COL));

        if (Cells[indexW] == 'W')
        {
            new = 1;
            for (w=0; w<WallNum; w++)
            {
               /* Already in the wall list */
               if (Wall[w] == indexW)
               {
                  new = 0;
                  break;
               }
            }
            if ( new )
            {
                Wall[WallNum++] = indexW;
            }
        }
    }
}

void RemoveWallList(int k)
{
    int i;
    int j;

    Wall[k] = 0;
    if (WallNum > 0)
    {
        i = 0;
        for (j=0; j<WallNum; j++)
        {
           if (Wall[j] > 0)
           {
              Wall[i++] = Wall[j];
           }
        }
        WallNum -= 1;
    }
}

void Reset(void)
{
    int i, x, y;

    /* Set all cells to the blocked type */
    for (y=0; y<MAZE_ROW; y++)
    {
       for (x=0; x<MAZE_COL; x++)
       {
           i = (x + (y * MAZE_COL));
           Cells[i] = 'B';
           X[i] = x;
           Y[i] = y;
       }
    }
    /* Mark the cells should be the cell type */
    for (y=1; y<(MAZE_ROW-1); y+=2)
    {
       for (x=1; x<(MAZE_COL-1); x+=2)
       {
           i = (x + (y * MAZE_COL));
           Cells[i] = 'C';
       }
    }
    /* Mark the cells should be the wall type */
    for (y=2; y<(MAZE_ROW-2); y+=2)
    {
       for (x=1; x<(MAZE_COL-1); x+=2)
       {
          i = (x + (y * MAZE_COL));
          Cells[i] = 'W';
       }
    }
    for (y=1; y<(MAZE_ROW-1); y+=2)
    {
       for(x=2; x<(MAZE_COL-2); x+=2)
       {
          i = (x + (y * MAZE_COL));
          Cells[i] = 'W';
       }
    }

    /* Initial the wall list to empty */
    for (i=0; i<MAZE_SIZE; i++)
    {
        Wall[i] = 0;
    }
    WallNum = 0;
}

void Draw(void)
{
    int found;
    int indexC;
    int x, y;
    int w;

    for (y=0; y<MAZE_ROW; y++)
    {
        for (x=0; x<MAZE_COL; x++)
        {
            indexC = (x + (y * MAZE_COL));
            if (Cells[indexC] == 'W')
            {
                found = 0;
                for (w=0; w<WallNum; w++)
                {
                    if (Wall[w] == indexC)
                    {
                        found = 1;
                        break;
                    }
                }
                if ( found )
                {
                    printf(" [1;32m%c[0m", Cells[indexC]);
                }
                else
                {
                    printf(" %c", Cells[indexC]);
                }
            }
            else if (Cells[indexC] == 'S')
            {
                printf(" [1;33m%c[0m", Cells[indexC]);
            }
            else
            {
                printf(" %c", Cells[indexC]);
            }
        }
        printf("\n");
    }
    printf("\n");
}

int main(int argc, char *argv[])
{
    int verbose = 0;
    int indexU, indexD;
    int indexL, indexR;
    int indexC;
    int indexW;
    int x, y;
    int n;
    int w;


    if (argc > 1)
    {
        verbose = 1;
    }

    Reset();

    /*
    * [1] Pick a cell, mark it as part of the maze.
    */
    do
    {
       x = Random(1, (MAZE_COL-2));
       y = Random(1, (MAZE_ROW-2));
       indexC = (x + (y * MAZE_COL));
    } while (Cells[indexC] != 'C');
    Cells[indexC] = 0x20;

    /*
    * [2] Add the walls of the cell to the wall list.
    */
    for (n=0; n<4; n++)
    {
        x = X[indexC] + NeighX[n];
        y = Y[indexC] + NeighY[n];
        indexW = (x + (y * MAZE_COL));

        if (Cells[indexW] == 'W')
        {
            Wall[WallNum++] = indexW;
        }
    }

    /*
    * [3] While there are walls in the list:
    *       A. Pick a random wall from the list.
    *          If only one of the two cells that the wall divides is visited, then:
    *            a. Make the wall a passage and mark the unvisited cell as part of the maze.
    *            b. Add the neighboring walls of the cell to the wall list.
    *       B. Remove the wall from the list.
    */
    while (WallNum > 0)
    {
        w = Random(0, (WallNum - 1));
        if (Wall[w] > 0)
        {
           indexW = Wall[w];

           /* up-down */
           x = X[indexW] + NeighX[0];
           y = Y[indexW] + NeighY[0];
           indexU = (x + (y * MAZE_COL));
           x = X[indexW] + NeighX[1];
           y = Y[indexW] + NeighY[1];
           indexD = (x + (y * MAZE_COL));

           /* left-right */
           x = X[indexW] + NeighX[2];
           y = Y[indexW] + NeighY[2];
           indexL = (x + (y * MAZE_COL));
           x = X[indexW] + NeighX[3];
           y = Y[indexW] + NeighY[3];
           indexR = (x + (y * MAZE_COL));

           if ((Cells[indexU] != 'B') && (Cells[indexD] != 'B'))
           {
              if ((Cells[indexU] == 0x20) && (Cells[indexD] == 0x20))
              {
                  RemoveWallList( w );
                  Cells[indexW] = 'B';
              }
              else
              {
                  RemoveWallList( w );
                  Cells[indexW] = 0x20;

                  if (Cells[indexU] == 'C')
                  {
                     Cells[indexU] = 0x20;
                     AddWallList( indexU );
                  }
                  else if (Cells[indexD] == 'C')
                  {
                     Cells[indexD] = 0x20;
                     AddWallList( indexD );
                  }
              }
           }
           else if ((Cells[indexL] != 'B') && (Cells[indexR] != 'B'))
           {
              if ((Cells[indexL] == 0x20) && (Cells[indexR] == 0x20))
              {
                  RemoveWallList( w );
                  Cells[indexW] = 'B';
              }
              else
              {
                  RemoveWallList( w );
                  Cells[indexW] = 0x20;

                  if (Cells[indexL] == 'C')
                  {
                     Cells[indexL] = 0x20;
                     AddWallList( indexL );
                  }
                  else if (Cells[indexR] == 'C')
                  {
                     Cells[indexR] = 0x20;
                     AddWallList( indexR );
                  }
              }
           }
        }
        else
        {
           printf("ERR: Wall[%d] = 0\n", w);
        }

        if ( verbose )
        {
            Draw();
        }
    }

    /* Set the start position */
    x = 1;
    y = 1;
    Cells[x + y * MAZE_COL] = 'S';
    /* Set the exit */
    x = (MAZE_COL - 1);
    y = (MAZE_ROW - 2);
    Cells[x + y * MAZE_COL] = 0x20;

    Draw();

    return 0;
}

