#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>

const int rows = 10;
const int columns = 8;
const int mines = 10;

void placeMines();
void fillGrid();
void printGrid();
//int playSweep();
//bool boom();
//void firstSweep();
void updateGrid();
void revealGrid();
bool inBounds();

int main()
{
    char buf[4];
    int urandom = open("/dev/urandom", O_RDONLY);
    read(urandom, buf, 4);
    close(urandom);
    srand(*(unsigned int *)buf);


    int rowGuess;
    int columnGuess;

    int grid[rows][columns];
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            grid[i][j] = 0;
        }
    }
    bool gridVis[rows][columns];
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            gridVis[i][j] = false;
        }
    }
    
    printf("Guess (row column): ");
    scanf("%d %d", &rowGuess, &columnGuess);
    int c;
    while((c = getchar()) != '\n' && c != EOF) continue;

    placeMines(grid, rowGuess, columnGuess);

    fillGrid(grid);
    printGrid(grid);

    printf("\n\n");
    updateGrid(grid, gridVis, rowGuess, columnGuess);
    revealGrid(grid, gridVis);

    //while(!boom())
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

// void firstSweep(int grid[][columns], int rowGuess, int columnGuess)
// {
//     int tempGrid[rows][columns];

//     for (int i = 0; i < rows; i++)
//     {
//         for (int j = 0; j < columns; j++)
//         {
//             tempGrid[i][j] = grid[i][j];
//         }
//     }
//     for (int i = 0; i < rows; i++)
//     {
//         for (int j = 0; j < columns; j++)
//         {
//             int tempX = rowGuess + i;
//             int tempY = columnGuess + j;

//             if (tempX > rows)
//                 tempX -= rows;
//             if (tempY > columns)
//                 tempY -= columns;

//             grid[tempX][tempY] = tempGrid[i][j];
//         }
//     }
// }

void updateGrid(int grid[][columns], bool gridVis[][columns], int rowGuess, int columnGuess)
{
    if (grid[rowGuess][columnGuess] == -1)
    {
        gridVis[rowGuess][columnGuess] = false;
        return;
    }
    else if (grid[rowGuess][columnGuess] == 0)
    {
        if (inBounds(rowGuess-1,columnGuess+1) && gridVis[rowGuess-1][columnGuess+1] == false)
        {
            gridVis[rowGuess-1][columnGuess+1] = true;
            if (grid[rowGuess-1][columnGuess+1] == 0)
                updateGrid(grid, gridVis, rowGuess - 1, columnGuess + 1);
        }
        if (inBounds(rowGuess-1, columnGuess) && gridVis[rowGuess-1][columnGuess] == false)
        {
            gridVis[rowGuess-1][columnGuess] = true;
            if (grid[rowGuess-1][columnGuess] == 0)
                updateGrid(grid, gridVis, rowGuess - 1, columnGuess);
        }
        if (inBounds(rowGuess-1, columnGuess-1) && gridVis[rowGuess-1][columnGuess-1] == false)
        {
            gridVis[rowGuess-1][columnGuess-1] = true;
            if (grid[rowGuess-1][columnGuess-1] == 0)
            updateGrid(grid, gridVis, rowGuess - 1, columnGuess - 1);
        }
        if (inBounds(rowGuess, columnGuess+1) && gridVis[rowGuess][columnGuess+1] == false)
        {
            gridVis[rowGuess][columnGuess+1] = true;
            if (grid[rowGuess][columnGuess+1] == 0)
                updateGrid(grid, gridVis, rowGuess, columnGuess + 1);
        }
        if (inBounds(rowGuess, columnGuess-1) && gridVis[rowGuess][columnGuess-1] == false)
        {
            gridVis[rowGuess][columnGuess-1] = true;
            if (grid[rowGuess][columnGuess-1] == 0)
                updateGrid(grid, gridVis, rowGuess, columnGuess - 1);
        }
        if (inBounds(rowGuess+1, columnGuess+1) && gridVis[rowGuess+1][columnGuess+1] == false)
        {
            gridVis[rowGuess+1][columnGuess+1] = true;
            if (grid[rowGuess+1][columnGuess+1] == 0)
                updateGrid(grid, gridVis, rowGuess + 1, columnGuess + 1);
        }
        if (inBounds(rowGuess+1, columnGuess) && gridVis[rowGuess+1][columnGuess] == false)
        {
            gridVis[rowGuess+1][columnGuess] = true;
            if (grid[rowGuess+1][columnGuess] == 0)
                updateGrid(grid, gridVis, rowGuess + 1, columnGuess);
        }
        if (inBounds(rowGuess+1, columnGuess-1) && gridVis[rowGuess+1][columnGuess-1] == false)
        {
            gridVis[rowGuess+1][columnGuess-1] = true;
            if (grid[rowGuess+1][columnGuess-1] == 0)
                updateGrid(grid, gridVis, rowGuess + 1, columnGuess - 1);
        }
    }
}

void revealGrid(int grid[][columns], bool gridVis[][columns])
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            if (gridVis[i][j] == true)
            {
                printf("[%d] ", grid[i][j]);
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
        return true;
    return false;
}
/*
int playSweep(int grid[][columns])
{

}
bool boom()
{

}
*/