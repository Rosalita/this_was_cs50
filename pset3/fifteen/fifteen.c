/**
 * fifteen.c
 *
 * Implements Game of Fifteen (generalized to d x d).
 *
 * Usage: fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [DIM_MIN,DIM_MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */

#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 9

// board
int board[DIM_MAX][DIM_MAX];

// dimensions
int d;

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);

int main(int argc, string argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    // open log
    FILE *file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }

    // greet user with instructions
    greet();

    // initialize the board
    init();

    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear(); // comment out this line to to debug init function

        // draw the current state of the board
        draw();

        // log the current state of the board (for testing)
        for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                fprintf(file, "%i", board[i][j]);
                if (j < d - 1)
                {
                    fprintf(file, "|");
                }
            }
            fprintf(file, "\n");
        }
        fflush(file);

        // check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = get_int();

        // quit if user inputs 0 (for testing)
        if (tile == 0)
        {
            break;
        }

        // log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        // sleep thread for animation's sake
        usleep(500000);
    }

    // close log
    fclose(file);

    // success
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
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).
 */
void init(void)
{
    // TODO
    // find the max value to put into the game board
    int maxvalue = (d * d) -1; // for a 4 * 4 board this would be 15. For a 5 * 5 board it would be 24

    // loop through the 2d array and start putting values in
    // the largest number must be at top left corner then working down to bottom right.
    // the last value will be a "0", but we will not display the 0
    // index for row comes first and column comes second
    // if height and width of board are even (modulo) then tiles at [n][n-2] and [n][n-3] must be swapped
    for (int i = 0; i < d; i++){
        for (int j = 0; j < d; j++){
            board[i][j] = maxvalue;
            maxvalue --;
        }
    }

    // check to see if need to switch tiles due to height and width of board even
    if (d % 2 == 0){ // if dimensions divides perfectly by 2 then its even
     // so swap the penultimate value and one before around
     int swap = board[d-1][d-2];
     board[d-1][d-2] = board[d-1][d-3];
     board[d-1][d-3] = swap;
    }

}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    // TODO
    //for each row
       //for each column
          //print the tiles value
    // print a new line

    // to ensure formatting, print a blank space before any single digit numbers
    // printf %2i will ensure that there is a space
    // represent blank tile with a _ so add a condition where if the value is 0 to print an underscore


        for (int i = 0; i < d; i++) // for each row
        {
            for (int j = 0; j < d; j++) // for each column
            {
                if (board[i][j] != 0){ // if the value is not 0
                printf("%2i ",board[i][j]); // print the tiles value making sure single digit have a blank space before, followed by a space after the value
                } else{ // if it's the 0
                printf(" _ "); // print underscore with blank space either side
                }

            }
            printf("\n"); // then print a new line
        }

}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false.
 */
bool move(int tile)
{
    // TODO

    // user provides tile number to move, need to iterate over the board to find that tile location
    // edit board array to move tile if move is legal
    // can only move if the tile is above below left or right of the blank tile
    // move is done by swapping selected tile and blank tile
    // suggests keeping track of where the blank tile is using two variables for blank x and blank y

    //check that the tile to move is a value on the board?


    //initialise some vars to hold x,y coords of tile to move and blank
    int movex;
    int movey;
    int blankx;
    int blanky;

    // check all values to find x and y of tile to move
    // to find x and y of blank
    for (int i = 0; i < d; i++) // for each row
    {
        for (int j = 0; j < d; j++) // for each column
        {

           if (board[i][j] == tile){// if value is the tile to be moved
             movex = i;
             movey = j;
            }
           // if value is 0 then its the blank, story its x and y in blankx & blanky
           if (board[i][j] == 0){
             blankx = i;
             blanky = j;
            }
        }

    }

    // if space is not in the top row
    if(blankx != 0){
       // check if tile above is the tile, e.g. tile 2,3 space 3,3
        if (board[blankx-1][blanky] == tile){ //if it is above the space
           // switch the values
           board[blankx][blanky] = board[movex][movey]; // put tile into the blank's space
           board[movex][movey] = 0; // set the tile's space to 0
           return true;
       }
    }

    // if space is not in the far right column
    if (blanky != 3){
       // check if tile to right is the tile, e.g. tile 2,2 space 2,1
       if(board[blankx][blanky+1] == tile){ // if its to the right of space
           // switch the values
           board[blankx][blanky] = board[movex][movey]; // put tile into the blank's space
           board[movex][movey] = 0; // set the tile's space to 0
           return true;
       }
    }

    // if space is not in the bottom row
    if(blankx != 3){
       // check if the tile below it is the tile, e.g. tile 2,2 space 1,2
       if(board[blankx+1][blanky] == tile){ // if its below
           // switch the values
           board[blankx][blanky] = board[movex][movey]; // put tile into the blank's space
           board[movex][movey] = 0; // set the tile's space to 0
           return true;
       }
    }

    // if space is not in the far left column
    if (blanky != 0){
       // check if tile to the left is the tile, e.g tile 3,2 space 3,3
       if(board[blankx][blanky-1] == tile){ // if its to the left
           // switch the values
           board[blankx][blanky] = board[movex][movey]; // put tile into the blank's space
           board[movex][movey] = 0; // set the tile's space to 0
           return true;
       }
    }


   // if havent been able to swap tile and space, return false
    return false;
}

/**
 * Returns true if game is won (i.e., board is in winning configuration),
 * else false.
 */
bool won(void)
{
    // TODO
    // iterate over the board and if any value is incorrect, return false
    // return true once all values checked


    // calculate the largest tile value
    int maxvalue = (d * d) -1; //8
    int checkvalue = 1;


      for (int i = 0; i < d; i++){
        for (int j = 0; j < d; j++){

            // if board 0,0 doesn't equal the check value
            if (board[i][j] != checkvalue){ //3 2  = 8
                return false; // then return false
            } else {

            if (checkvalue == maxvalue){ // check to see if reached maxvalue
              //if we have then the game is won so return true
              return true;
            }

            // if not at the max value yet increase checkvalue by 1
                checkvalue ++;
            }


        }
    }

    return false;
}
