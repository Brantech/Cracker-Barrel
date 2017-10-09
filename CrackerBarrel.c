/*
* This is a game commonly found at Cracker Barrel. It comes in the shape of a triangle and has 14 pegs with 15 possible
* holes to put them in. The goal is to hop each neighboring peg until there is only one peg left. You can only hop a peg
* over pegs directly next to it and into an open space.
*
* Positions :
*
*   11  12  13  14  15
*     7   8   9   10
*       4   5   6
*         2   3
*           1
*
* This program will take the position of the empty spot and return the set of moves needed to win the game.
* */

//-----------------------------------------------------------------------------------------//

#include <stdio.h>
#include <stdlib.h>

//-----------------------------------------------------------------------------------------//

// Struct that holds information about a point.
typedef struct node
{
    int x;
    int y;
    struct node *next;
} Point;

// Up Left, Up Right, Down Right, Down Left, Left, Right
int DX[] = { -2, -2, 2, 2, 0, 0 };
int DY[] = { 0, 2, 0, -2, -2, 2 };

//-----------------------------------------------------------------------------------------//

int **init(int);
void setOpen( int **, int);
void erase(int **, Point**);
void add(Point **head, Point **tail, int x, int y);
void trim(Point **head, Point **tail);
int solve(int **board, Point **head, Point **tail);
void printList(Point *head);
int pointToPos(Point *p);

//-----------------------------------------------------------------------------------------//

int main(int argc, char **argv)
{
    puts("11  12  13  14  15");
    puts("  7   8   9   10");
    puts("    4   5   6");
    puts("      2   3");
    puts("        1");
    puts("\nPlease enter the number where the peg is missing.");

    // Get input and abort if it is invalid.
    int n;
    scanf("%d", &n);
    if(n > 15 || n < 1)
    {
        puts("Invalid Input!\n");
        return 1;
    }

    Point *head = NULL, *tail = NULL;

    int **board = init(n);
    solve(board, &head, &tail);
    printList(head);

    erase(board, &head);
    while (getchar() != '\n');
    getchar();
    return 0;
}

//-----------------------------------------------------------------------------------------//

// Creates a 2D array of 5 arrays with lengths descending from 5.
int **init(int n)
{
    int i, j;

    int **board = (int **)malloc(5 * sizeof(int *));
    for(i = 0; i < 5; i++)
    {
        board[i] = (int *)malloc((5 - i) * sizeof(int));
        for(j = 0; j < 5 - i; j++)
            board[i][j] = 1;
    }

    setOpen(board, n);
    return board;
}

//-----------------------------------------------------------------------------------------//

// Sets the given position on the board to 0.
void setOpen( int **board, int n )
{
    if( n > 10 )
        board[0][n - 11] = 0;
    else if( n > 6 )
        board[1][n - 7] = 0;
    else if( n > 3 )
        board[2][n - 4] = 0;
    else if( n > 1 )
        board[3][n - 2] = 0;
    else
        board[4][0] = 0;
}

//-----------------------------------------------------------------------------------------//

// Frees memory used by board.
void erase(int **board, Point **head)
{
    int i = 0;
    for(; i < 5; i++)
        free(board[i]);
    free(board);

    while(*head != NULL)
    {
        Point *temp = (*head)->next;
        free(*head);
        *head = temp;
    }
}

//-----------------------------------------------------------------------------------------//

// Adds nodes to the tail of the linked list.
void add(Point **head, Point **tail, int x, int y)
{
    if(*head == NULL)
        *head = *tail = (Point *)malloc(sizeof(Point));
    else
    {
        (*tail)->next = (Point *)malloc(sizeof(Point));
        *tail = (*tail)->next;
    }

    (*tail)->x = x;
    (*tail)->y = y;
    (*tail)->next = NULL;
}

//-----------------------------------------------------------------------------------------//

// Deletes one node from the end of the linked list.
void trim(Point **head, Point **tail)
{
    if(*head == NULL) return;
    if(*head == *tail)
    {
        free(*head);
        return;
    }

    Point *temp = *head;
    while(temp->next != *tail)
        temp = temp->next;

    free(*tail);
    *tail = temp;
    (*tail)->next = NULL;
}

//-----------------------------------------------------------------------------------------//

// This function uses recursion and back-tracking to find the order the pegs should be hopped.
int solve(int **board, Point **head, Point **tail)
{
    // A simple queue in the form of an array.
    Point queue[14];
    int start = 0, end = -1;

    int i, j;
    // Adds available pegs to the end of the queue.
    for(i = 0; i < 5; i++)
    {
        for(j = 0; j < 5 - i; j++)
        {
            if(board[i][j] == 1)
            {
               end++;
               queue[end].x = i;
               queue[end].y = j;
            }
        }
    }

    // If the size of the queue is 1, the puzzle has been solved.
    if(start == end) return 1;

    while(start <= end)
    {
        Point p = queue[start++];
        // Tries to make the peg hop in every direction.
        for( i = 0; i < 6; i++ )
        {
            // Check if it can move a peg in that direction without going out of bounds.
            if( p.x + DX[i] > 4 || p.x + DX[i] < 0 ) continue;
            if( p.y + DY[i] > 4 - (p.x + DX[i]) || p.y + DY[i] < 0 ) continue;

            // Check if there is a peg to hop over, and a space to hop to.
            if( board[p.x + DX[i] / 2][p.y + DY[i] / 2] == 0) continue;
            if( board[p.x + DX[i]][p.y + DY[i]] == 1) continue;

            // Invert the pegs effected by the hop.
            board[p.x + DX[i]][p.y + DY[i]] = 1;
            board[p.x + DX[i] / 2][p.y + DY[i] / 2] = 0;
            board[p.x][p.y] = 0;

            // Add the start and end position of the moving peg.
            add(head, tail, p.x, p.y);
            add(head, tail, p.x + DX[i], p.y + DY[i]);

            // Recursively call the function with the new board.
            if(solve(board, head, tail) == 1) return 1;

            // Revert to the state before the changes were made if the sequence failed.
            board[p.x + DX[i]][p.y + DY[i]] = 0;
            board[p.x + DX[i] / 2][p.y + DY[i] / 2] = 1;
            board[p.x][p.y] = 1;

            trim(head, tail);
            trim(head, tail);
        }
    }

    return 0;
}

//-----------------------------------------------------------------------------------------//

// Formatted output of the linked list.
void printList(Point *head)
{
    Point *temp;

    puts("------------");
    for(temp = head; temp != NULL; temp = temp->next->next)
        printf("| %-2d -> %2d |\n------------\n", pointToPos(temp), pointToPos(temp->next));
}

//-----------------------------------------------------------------------------------------//

// Turns a point into a position on the board.
int pointToPos(Point *p)
{
    switch( p->x )
    {
    case 0:
        return p->y + 11;
    case 1:
        return p->y + 7;
    case 2:
        return p->y + 4;
    case 3:
        return p->y + 2;
    default:
        return 1;
    }
}

//-----------------------------------------------------------------------------------------//
