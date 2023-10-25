#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>

const int rows = 10;
const int columns = 8;
const int mines = 10;

void placeMines(int grid[][columns], int rowGuess, int columnGuess);
void fillGrid(int grid[][columns]);
void printGrid(int grid[][columns]);
void playSweep(int gridVis[][columns],int grid[][columns], char guessType, int rowGuess, int columnGuess);
bool boom(int grid[][columns], int gridVis[][columns]);
void updateGrid(int grid[][columns], int gridVis[][columns], int rowGuess, int columnGuess);
void revealGrid(int grid[][columns], int gridVis[][columns]);
bool inBounds(int i, int j);
bool gridFilled(int gridVis[][columns]);


int main()
{   
    char buf[4];
    int urandom = open("/dev/urandom", O_RDONLY);
    read(urandom, buf, 4);
    close(urandom);
    srand(*(unsigned int *)buf);

    int firstRowGuess;
    int firstColumnGuess;

    int grid[rows][columns];
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            grid[i][j] = 0;
        }
    }
    int gridVis[rows][columns];
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            gridVis[i][j] = 0;
        }
    }
    
    revealGrid(grid, gridVis);
    
    printf("Guess (row column): ");
    scanf("%d %d", &firstRowGuess, &firstColumnGuess);
    int c;
    while((c = getchar()) != '\n' && c != EOF) continue;

    placeMines(grid, firstRowGuess - 1, firstColumnGuess - 1);

    fillGrid(grid);
    updateGrid(grid, gridVis, firstRowGuess - 1, firstColumnGuess - 1);
    revealGrid(grid, gridVis);

    while (!boom(grid, gridVis) && !gridFilled(gridVis))
    {
        char guessType;
        int rowGuess;
        int columnGuess;

        printf("Dig or Flag? (d or f): ");
        scanf("%c", &guessType);

        printf("Guess (row column): ");
        scanf("%d %d", &rowGuess, &columnGuess);
        printf("\n\n");

        if (gridVis[rowGuess - 1][columnGuess - 1] == 1)
        {
            revealGrid(grid, gridVis);
            getchar();
        }
        else
        {
            getchar();
            playSweep(gridVis, grid, guessType, rowGuess - 1, columnGuess - 1);
            revealGrid(grid, gridVis);
        }
    }
    
    if (boom(grid, gridVis))
    {
        printf("BOOM!\nYou Lost! Try again!\n");
    }
    else if (gridFilled(gridVis))
    {
        puts("You Won! Great Job!");
    }
}

void placeMines(int grid[][columns], int rowGuess, int columnGuess)
{
    int minesLeft = 0;
    while (minesLeft < mines)
    {
        int i = rand() % (rows-1);
        int j = rand() % (columns - 1);
        
        if ((i == rowGuess - 1 && j == columnGuess - 1) || (i == rowGuess - 1 && j == columnGuess) || (i == rowGuess - 1 && j == columnGuess + 1)){}
        else if ((i == rowGuess && j == columnGuess - 1) || (i == rowGuess && j == columnGuess) || (i == rowGuess && j == columnGuess + 1)){}
        else if ((i == rowGuess + 1 && j == columnGuess - 1) || (i == rowGuess + 1 && j == columnGuess) || (i == rowGuess + 1 && j == columnGuess + 1)){}
        else if (grid[i][j] == -1) {}
        else
        {
            grid[i][j] = -1;
            minesLeft++;
        }
    }
}

void fillGrid(int grid[][columns])
{
    for(int i = 0; i < rows;i++)
    {
        for(int j = 0; j < columns; j++)
        {
            if (grid[i][j] == -1)
            {
                if (inBounds(i-1,j+1) && grid[i-1][j+1] != -1)
                    grid[i-1][j+1]++;
                if (inBounds(i-1,j) && grid[i-1][j] != -1)
                    grid[i-1][j]++;
                if (inBounds(i-1,j-1) && grid[i-1][j-1] != -1)
                    grid[i-1][j-1]++;
                if (inBounds(i,j+1) && grid[i][j+1] != -1)
                    grid[i][j+1]++;
                if (inBounds(i,j-1) && grid[i][j-1] != -1)
                    grid[i][j-1]++;
                if (inBounds(i+1,j+1) && grid[i+1][j+1] != -1)
                    grid[i+1][j+1]++;
                if (inBounds(i+1,j) && grid[i+1][j] != -1)
                    grid[i+1][j]++;
                if (inBounds(i+1,j-1) && grid[i+1][j-1] != -1)
                    grid[i+1][j-1]++;
            }
        }
    }
}

void printGrid(int grid[][columns])
{
    for(int i = 0; i < rows;i++)
    {
        for(int j = 0; j < columns; j++)
        {
            printf("[%d] ", grid[i][j]);
        }
        puts("");
    }
}

void updateGrid(int grid[][columns], int gridVis[][columns], int rowGuess, int columnGuess)
{    
    gridVis[rowGuess][columnGuess] = 1;

    if (grid[rowGuess][columnGuess] == 0)
    {
        if (inBounds(rowGuess-1,columnGuess+1) && gridVis[rowGuess-1][columnGuess+1] == 0)
        {
            gridVis[rowGuess-1][columnGuess+1] = 1;
            if (grid[rowGuess-1][columnGuess+1] == 0)
                updateGrid(grid, gridVis, rowGuess - 1, columnGuess + 1);
        }
        if (inBounds(rowGuess-1, columnGuess) && gridVis[rowGuess-1][columnGuess] == 0)
        {
            gridVis[rowGuess-1][columnGuess] = 1;
            if (grid[rowGuess-1][columnGuess] == 0)
                updateGrid(grid, gridVis, rowGuess - 1, columnGuess);
        }
        if (inBounds(rowGuess-1, columnGuess-1) && gridVis[rowGuess-1][columnGuess-1] == 0)
        {
            gridVis[rowGuess-1][columnGuess-1] = 1;
            if (grid[rowGuess-1][columnGuess-1] == 0)
            updateGrid(grid, gridVis, rowGuess - 1, columnGuess - 1);
        }
        if (inBounds(rowGuess, columnGuess+1) && gridVis[rowGuess][columnGuess+1] == 0)
        {
            gridVis[rowGuess][columnGuess+1] = 1;
            if (grid[rowGuess][columnGuess+1] == 0)
                updateGrid(grid, gridVis, rowGuess, columnGuess + 1);
        }
        if (inBounds(rowGuess, columnGuess-1) && gridVis[rowGuess][columnGuess-1] == 0)
        {
            gridVis[rowGuess][columnGuess-1] = 1;
            if (grid[rowGuess][columnGuess-1] == 0)
                updateGrid(grid, gridVis, rowGuess, columnGuess - 1);
        }
        if (inBounds(rowGuess+1, columnGuess+1) && gridVis[rowGuess+1][columnGuess+1] == 0)
        {
            gridVis[rowGuess+1][columnGuess+1] = 1;
            if (grid[rowGuess+1][columnGuess+1] == 0)
                updateGrid(grid, gridVis, rowGuess + 1, columnGuess + 1);
        }
        if (inBounds(rowGuess+1, columnGuess) && gridVis[rowGuess+1][columnGuess] == 0)
        {
            gridVis[rowGuess+1][columnGuess] = 1;
            if (grid[rowGuess+1][columnGuess] == 0)
                updateGrid(grid, gridVis, rowGuess + 1, columnGuess);
        }
        if (inBounds(rowGuess+1, columnGuess-1) && gridVis[rowGuess+1][columnGuess-1] == 0)
        {
            gridVis[rowGuess+1][columnGuess-1] = 1;
            if (grid[rowGuess+1][columnGuess-1] == 0)
                updateGrid(grid, gridVis, rowGuess + 1, columnGuess - 1);
        }
    }
}

void revealGrid(int grid[][columns], int gridVis[][columns])
{
    printf("  ");
    for (int i = 0; i < columns; i++)
    {
        printf("   %i", i+1);
    }
    puts("");

    for (int i = 0; i < rows; i++)
    {
        if (i < 9)
        {
            printf("%i   ", i+1);
        }
        else
        {
            printf("%i  ", i+1);
        }

        for (int j = 0; j < columns; j++)
        {
            if (gridVis[i][j] == 1)
            {
                printf("[%d] ", grid[i][j]);
            }
            else if (gridVis[i][j] == 2)
            {
                printf("[*] ");
            }
            else
            {
                printf("[ ] ");
            }
        }
        printf("\n");
    }
}

bool inBounds(int i, int j)
{
    if (i >= 0 && i < rows && j >= 0 && j < columns)
    {
        return true;
    }
    return false;
}

void playSweep(int gridVis[][columns], int grid[][columns], char guessType, int rowGuess, int columnGuess)
{
    if (guessType == 'd')
    {
        gridVis[rowGuess][columnGuess] = 1;
        if (grid[rowGuess][columnGuess] == 0)
        {
            updateGrid(grid, gridVis, rowGuess, columnGuess);
        }
    }
    else if (guessType == 'f')
    {
        if (gridVis[rowGuess][columnGuess] == 2)
        {
            gridVis[rowGuess][columnGuess] = 0;
        }
        else
        {
            gridVis[rowGuess][columnGuess] = 2;
        }
    }
}

bool boom(int grid[][columns], int gridVis[][columns])
{
    for(int i = 0; i < rows;i++)
    {
        for(int j = 0; j < columns; j++)
        {
            if (grid[i][j] == -1 && gridVis[i][j] == 1)
                return true;
        }
    }
    return false;
}

bool gridFilled(int gridVis[][columns])
{
    for(int i = 0; i < rows;i++)
    {
        for(int j = 0; j < columns; j++)
        {
            if (gridVis[i][j] == 0)
                return false;
        }
    }
    return true;
}