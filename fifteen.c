/**
 * fifteen.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Implements the Game of Fifteen (generalized to d x d).
 *
 * Usage: ./fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [MIN,MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */
 
#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// board's minimal dimension
#define MIN 3

// board's maximal dimension
#define MAX 9

// board, whereby board[i][j] represents row i and column j
int board[MAX][MAX];

// board's dimension
int d;

// position of the blank tile
int blank_row;
int blank_column;

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);
void save(void);

int main(int argc, string argv[])
{
    // greet player
    greet();

    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: ./fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < MIN || d > MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            MIN, MIN, MAX, MAX);
        return 2;
    }

    // initialize the board
    init();
    blank_row = d - 1;
    blank_column = d - 1;

    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // saves the current state of the board (for testing)
        save();

        // check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = GetInt();

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        // sleep for animation's sake
        usleep(500000);
    }

    // that's all folks
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("GAME OF FIFTEEN\n");
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1,
 * (i.e., fills board with values but does not actually print them),
 * whereby board[i][j] represents row i and column j.
 */
void init(void)
{
    // calculate the number of numbers we'll need on the board
    int n = (d * d) - 1;
    
    // go through the rows
    for (int i = 0; i < d; i++)
    {
    
        // go through the columns
        for (int j = 0; j < d; j++, n--)
        {
        
            // assign the correct value to the field
            if (n > 0)
            {
                board[i][j] = n;
            }
            
            // assign "_" to the empty field
            else
            {
                board[i][j] = 95;
            }
        }
    }
    if (((d * d) - 1) % 2 != 0)
        {
            board[d-1][d-2] = 2;
            board[d-1][d-3] = 1;
        }
    return;
}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    int n = (d * d) - 1;
    
    // for each row
    for (int i = 0; i < d; i++)
    {
        // for each column
        for (int j = 0; j < d; j++, n--)
        {
            // print the number
            if (board[i][j] != 95)
            {
                printf("%2d ", board[i][j]);
            }
            
            // print the "empty" symbol
            else
            {
                printf(" %c ", board[i][j]);
            }      
        }    
        // print new line
        printf("\n");
    }
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */
bool move(int tile)
{
    // give error message if tile number is bogus
    if (tile > (d * d) - 1)
    {
        return false;
    }
    
    int tile_row = 0;
    int tile_column = 0;
    
    // search for the tile's position
    // go through the rows
    for (int i = 0; i < d; i++)
    {
        //go through the columns
        for (int j = 0; j < d; j++)
        {
            //check if this is the tile we're looking for
            if (board[i][j] == tile)
            {
                tile_row = i;
                tile_column = j;
            }
        }
    }
    
    // if tile borders empty space
    if (((tile_row == blank_row) &&
        ((tile_column == blank_column - 1) || (tile_column == blank_column +1))) ||
        ((tile_column == blank_column) &&
        ((tile_row == blank_row - 1) || (tile_row == blank_row +1))))
    {
        // move the tile
        board[blank_row][blank_column] = tile;
        board[tile_row][tile_column] = 95;
        
        //remember where the blank tile is now
        blank_row = tile_row;
        blank_column = tile_column;
        
        return true;
    }
    else
    {
        return false;
    }       
}

/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */
bool won(void)
{
    int last_tile = 0;
    
    // go through all tiles
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            // check if the current tile is larger than the tile before it
            if (board[i][j] > last_tile)
            {
                last_tile = board[i][j];
            }
            else
            {
                return false;
            }
        }
    }
    return true;
}

/**
 * Saves the current state of the board to disk (for testing).
 */
void save(void)
{
    // log
    const string log = "log.txt";

    // delete existing log, if any, before first save
    static bool saved = false;
    if (!saved)
    {
        unlink(log);
        saved = true;
    }

    // open log
    FILE* p = fopen(log, "a");
    if (p == NULL)
    {
        return;
    }

    // log board
    fprintf(p, "{");
    for (int i = 0; i < d; i++)
    {
        fprintf(p, "{");
        for (int j = 0; j < d; j++)
        {
            fprintf(p, "%i", board[i][j]);
            if (j < d - 1)
            {
                fprintf(p, ",");
            }
        }
        fprintf(p, "}");
        if (i < d - 1)
        {
            fprintf(p, ",");
        }
    }
    fprintf(p, "}\n");

    // close log
    fclose(p);
}
