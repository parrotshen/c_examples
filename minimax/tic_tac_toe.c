#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int find_max(int scores[10], int len)
{
    int index = 0;
    int max = -10;
    int j;

    for (j=0; j<len; j++)
    {
        if (scores[j] > max)
        {
            max = scores[j];
            index = j;
        }
    }

    return index;
}

int find_min(int scores[10], int len)
{
    int index = 0;
    int min = 10;
    int j;

    for (j=0; j<len; j++)
    {
        if (scores[j] < min)
        {
            min = scores[j];
            index = j;
        }
    }

    return index;
}

char winner(char board[10])
{
    int check[8][3] = {
           { 1, 2, 3 },
           { 4, 5, 6 },
           { 7, 8, 9 },
           { 1, 4, 7 },
           { 2, 5, 8 },
           { 3, 6, 9 },
           { 1, 5, 9 },
           { 3, 5, 7 }
        };
    int cell1, cell2, cell3;
    int i;

    for (i=0; i<8; i++)
    {
        cell1 = board[ check[i][0] ];
        cell2 = board[ check[i][1] ];
        cell3 = board[ check[i][2] ];

        if ((cell1 == 'o') &&
            (cell2 == 'o') &&
            (cell3 == 'o'))
        {
            return 'o';
        }
        else if ((cell1 == 'x') &&
                 (cell2 == 'x') &&
                 (cell3 == 'x'))
        {
            return 'x';
        }
    }

    return 0;
}

int check_score(char board[10], int depth)
{
    if (winner( board ) == 'x')
    {
        return (10 - depth);
    }
    else if (winner( board ) == 'o')
    {
        return (depth - 10);
    }
    else
    {
        return 0;
    }
}

int game_end(char board[10])
{
    int i;

    if (winner( board ) > 0)
    {
        /* The game is end because there has the winner */
        return 1;
    }

    for (i=1; i<=9; i++)
    {
        if (board[i] == ' ')
        {
            /* Still has at least one vacant cell */
            return 0;
        }
    }

    /* No more vacant cell */
    return 1;
}

int minimax(char board[10], int depth, char player, char *log)
{
    char string[256];
    char space[64];

    int retval = 0;
    int scores[10];
    int moves[10];
    int move;
    int index;
    int num;
    int k;
    int i;


    if ( game_end( board ) )
    {
        int score = check_score(board, depth);

        printf("%s... [1;33m%d[0m\n", log, score);
        return score;
    }

    /* List all available moves */
    num = 0;
    for (move=1; move<=9; move++)
    {
        if (board[move] == ' ')
        {
            moves[num++] = move;
        }
    }
    if (0 == num)
    {
       printf("ERR: no more move at depth %d\n", depth);
       return 0;
    }

    /* The computer's turn */
    if (player == 'x')
    {
        for (k=0; k<num; k++)
        {
            /* Make the move */
            move = moves[k];
            board[move] = 'x';

            sprintf(string, "%sX(%d) ", log, move);

            scores[k] = minimax(board, (depth + 1), 'o', string);

            /* Undo the move */
            board[move] = ' ';
        }

        index = find_max(scores, num);

        for (i=0; i<(depth*5); i++)
        {
            space[i]= 0x20;
        }
        space[i] = 0;
        if (num > 1)
        {
            printf("%smax -> [1;36m%d[0m (", space, scores[index]);
            for (i=0; i<num; i++)
            {
                if (i == (num - 1))
                {
                    printf("%d", scores[i]);
                }
                else
                {
                    printf("%d, ", scores[i]);
                }
            }
            printf(")\n");
        }
        else
        {
            printf("%smax -> %d\n", space, scores[index]);
        }
    } 
    else
    {
        for (k=0; k<num; k++)
        {
            /* Make the move */
            move = moves[k];
            board[move] = 'o';

            sprintf(string, "%sO(%d) ", log, move);

            scores[k] = minimax(board, (depth + 1), 'x', string);

            /* Undo the move */
            board[move] = ' ';
        }

        index = find_min(scores, num);

        for (i=0; i<(depth*5); i++)
        {
            space[i]= 0x20;
        }
        space[i] = 0;
        if (num > 1)
        {
            printf("%smin -> [1;36m%d[0m (", space, scores[index]);
            for (i=0; i<num; i++)
            {
                if (i == (num - 1))
                {
                    printf("%d", scores[i]);
                }
                else
                {
                    printf("%d, ", scores[i]);
                }
            }
            printf(")\n");
        }
        else
        {
            printf("%smin -> %d\n", space, scores[index]);
        }
    } 

    if (0 == depth)
    {
        /* Return the move at top level */
        retval = moves[index];
    }
    else
    {
        retval = scores[index];
    }

    return retval;
} 

int main()
{
    char board[10] = 
    {
       0,
       'o', 'o', 'x', 
       'x', ' ', 'o', 
       ' ', ' ', 'x' 
    }; 
    int move;


    move = minimax(board, 0, 'x', "");
    board[move] = 'X';


    printf("\n\n");
    printf("       COL\n");
    printf("        0   1   2\n");
    printf("\n");
    printf("ROW 0   %c | %c | %c \n", board[1], board[2], board[3]);
    printf("       ---+---+---\n");
    printf("    1   %c | %c | %c \n", board[4], board[5], board[6]);
    printf("       ---+---+---\n");
    printf("    2   %c | %c | %c \n", board[7], board[8], board[9]);
    printf("\n");


    printf("The best move of 'x' is\n");
    printf(
        "  ROW: %d, COL: %d\n\n",
        ((move - 1) / 3),
        ((move - 1) % 3)
    );

    return 0;
}

