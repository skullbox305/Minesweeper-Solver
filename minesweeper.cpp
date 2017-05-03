#include "minesweeper.h"
#include <QtGlobal>

/**
  * Constructor
  * Initializes the board
  */
Minesweeper::Minesweeper()
{
    //Initialize board
    for(int i = 0; i < 20; i++)
    {
        for ( int j = 0; j < 20; j++)
        {
            mineBoard[i][j] = 0;
        }
    }

    //Generates the board with mines and populates the number of mines around it
    generateBoard();
}

/**
  * Destructor
  */
Minesweeper::~Minesweeper()
{
}

/**
  * generateBoard()
  * Generates the board with mines and populates the number of mines around it
  */
void Minesweeper::generateBoard()
{
    //Generate ten mines and create its adjacent number list
    for( int i = 0; i < 10; i++)
    {
        int row, column;

        //Prevent generating mine on another mine
        do
        {
            row = qrand() % 20;
            column = qrand() % 20;
        }while (mineBoard[row][column] == MINE);

        mineBoard[row][column] = MINE; //Number 9 indiciates a mine

        //Now that we have a mine created, increment its surroundings
        //There are eight of them

        //Top left
        if ( (row-1) != -1 && (column -1) != -1 && mineBoard[row-1][column-1] != MINE)
            mineBoard[row-1][column-1]++;
        //Top center
        if ( (row-1) != -1 && mineBoard[row-1][column] != MINE)
            mineBoard[row-1][column]++;
        //Top right
        if ( (row-1) != -1 && (column + 1) != 20 && mineBoard[row-1][column+1] != MINE)
            mineBoard[row-1][column+1]++;
        //Left
        if ( (column -1) != -1 && mineBoard[row][column-1] != MINE)
            mineBoard[row][column-1]++;
        //Right
        if ( (column + 1) != 20 && mineBoard[row][column+1] != MINE)
            mineBoard[row][column+1]++;
        //Bottom left
        if ( (row+1) != 20 && (column -1) != -1 && mineBoard[row+1][column-1] != MINE)
            mineBoard[row+1][column-1]++;
        //Bottom center
        if ( (row+1) != 20 && mineBoard[row+1][column] != MINE)
            mineBoard[row+1][column]++;
        //Bottom right
        if ( (row+1) != 20 && (column+1) != 20 && mineBoard[row+1][column+1] != MINE)
            mineBoard[row+1][column+1]++;
    }
}

/**
  * isMine(int, int)
  * Determines if there is a mine
  * @param int - Row to inspect
  * @param int - Column to inspect
  * @return bool - Whether or not the location is a mine
  */
bool Minesweeper::isMine(int row, int column)
{
        //Ensure the input is sanitary
        if ( row < 0 || row > 19)
            qFatal("Error in input");
        if ( column < 0 || column > 19)
            qFatal("Error in input");

        return mineBoard[row][column] == MINE;
}

/**
  * getValue(int, int)
  * Returns the value of the given cell
  * @param int - Row to inspect
  * @param int - Column to inspect
  * @return int - The value at the row
  */
int Minesweeper::getValue(int row, int column)
{
    //Ensure the input is sanitary
    if ( row < 0 || row > 19)
        qFatal("Error in input");
    if ( column < 0 || column > 19)
        qFatal("Error in input");

    return mineBoard[row][column];
}
