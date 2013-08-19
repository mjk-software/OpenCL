//------------------------------------------------------------------------------
//
// Name:       gameoflife.c
// 
// Purpose:    Run a naive Conway's game of life
//
// HISTORY:    Written by Tom Deakin and Simon McIntosh-Smith, August 2013
//
//------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>

// Define the board size
#define NX (17)
#define NY (17)

// Define the state of the cell
#define DEAD  0
#define ALIVE 1

// Function to load in a file which lists the alive cells
// Each line of the file is expected to be: x y 1
void load_board(char* board, char* file)
{
    FILE *fp = fopen(file, "r");
    if (!fp)
    {
        printf("Error! Could not open input file.");
        exit(EXIT_FAILURE);
    }

    int retval;
    unsigned int x, y, s;
    while ((retval = fscanf(fp, "%d %d %d\n", &x, &y, &s)) != EOF)
    {
        if (retval != 3)
        {
            printf("Error! Expected 3 values per line in input file\n");
            exit(EXIT_FAILURE);
        }
        if (x < 0 || x > NX - 1)
        {
            printf("Error! Input x-coord out of range\n");
            exit(EXIT_FAILURE);
        }
        if (y < 0 || y > NY - 1)
        {
            printf("Error! Input y-coord out of range\n");
            exit(EXIT_FAILURE);
        }
        if (s != ALIVE)
        {
            printf("Error! Alive value should be 1\n");
            exit(EXIT_FAILURE);
        }

        board[x + y * NX] = ALIVE;
    }

    fclose(fp);
}

// Function to print out the board to stdout
// Alive cells are displayed as O
// Dead cells are displayed as .
void print_board(char* board)
{
    for (unsigned int i = 0; i < NY; i++)
    {
        for (unsigned int j = 0; j < NX; j++)
        {
            if (board[i * NX + j] == DEAD)
                printf(".");
            else
                printf("O");
        }
        printf("\n");
    }
}

// Apply the rules of life to tick and save in tock
void accelerate_life(char* tick, char* tock)
{
    // The cell we work on in the loop
    unsigned int idx;

    // Indexes of rows/columns next to idx
    // wrapping around if required
    unsigned int x_l, x_r, y_u, y_d;

    for (unsigned int i = 0; i < NY; i++)
    {
        for (unsigned int j = 0; j < NX; j++)
        {
            // Calculate indexes
            idx = i * NX + j;
            x_r = (j + 1) % NX;
            x_l = (j == 0) ? NY - 1 : j - 1;
            y_u = (i + 1) % NX;
            y_d = (i == 0) ? NX - 1: i - 1;

            // Count alive neighbours (out of eight)
            int neighbours = 0;
            if (tick[i * NX + x_l] == ALIVE) neighbours++;
            if (tick[y_u * NX + x_l] == ALIVE) neighbours++;
            if (tick[y_d * NX + x_l] == ALIVE) neighbours++;
            
            if (tick[i * NX + x_r] == ALIVE) neighbours++;
            if (tick[y_u * NX + x_r] == ALIVE) neighbours++;
            if (tick[y_d * NX + x_r] == ALIVE) neighbours++;
            
            if (tick[y_u * NX + j] == ALIVE) neighbours++;
            if (tick[y_d * NX + j] == ALIVE) neighbours++;

            // Apply game of life rules
            if (tick[idx] == ALIVE)
            {
                if (neighbours == 2 || neighbours == 3)
                    // Cell lives on
                    tock[idx] = ALIVE;
                else
                    // Cell dies by over/under population
                    tock[idx] = DEAD;
            }
            else
            {
                if (neighbours == 3)
                    // Cell becomes alive through reproduction
                    tock[idx] = ALIVE;
                else
                    // Remains dead
                    tock[idx] = DEAD;
            }

        }
    }
}

// Main function
int main(int argc, void **argv)
{

    // Check we have a starting state file
    if (argc != 2)
    {
        printf("Usage:\n./gameoflife input.dat\n");
        return EXIT_FAILURE;
    }

    // Allocate memory for boards
    char* board_tick = (char *)calloc(NX * NY, sizeof(char));
    char* board_tock = (char *)calloc(NX * NY, sizeof(char));

    if (!board_tick || !board_tock)
    {
        printf("Error! Could not allocate memory for board\n");
        return EXIT_FAILURE;
    }

    // Load in the starting state to board_tick
    load_board(board_tick, argv[1]);

    // Display the starting state
    printf("Starting state\n");
    print_board(board_tick);

    // Loop
    // TODO

    accelerate_life(board_tick, board_tock);
    printf("Then:\n");
    print_board(board_tock);
    accelerate_life(board_tock, board_tick);
    printf("Then\n");
    print_board(board_tick);
    accelerate_life(board_tick, board_tock);

    // Display the final state
    printf("Finishing state\n");
    print_board(board_tock);

    return EXIT_SUCCESS;
}
