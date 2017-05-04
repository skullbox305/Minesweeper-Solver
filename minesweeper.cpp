#include "minesweeper.h"
#include <QtGlobal>

/**
  * Constructor
  * Initializes the board
  */
Minesweeper::Minesweeper(int width, int height, int mineAmount, QVector< QVector<int> > &board)
{
    fieldHeight = height;
    fieldWidth = width;
    amountOfMines = mineAmount;
    mineBoard = board;

    //Initialize board
    for(int i = 0; i < fieldHeight; i++)
    {
        for ( int j = 0; j < fieldWidth; j++)
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
    for( int i = 0; i < amountOfMines; i++)
    {
        int row, column;

        //Prevent generating mine on another mine
        do
        {
            row = qrand() % fieldHeight;
            column = qrand() % fieldWidth;
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
        if ( (row-1) != -1 && (column + 1) != fieldWidth && mineBoard[row-1][column+1] != MINE)
            mineBoard[row-1][column+1]++;
        //Left
        if ( (column -1) != -1 && mineBoard[row][column-1] != MINE)
            mineBoard[row][column-1]++;
        //Right
        if ( (column + 1) != fieldWidth && mineBoard[row][column+1] != MINE)
            mineBoard[row][column+1]++;
        //Bottom left
        if ( (row+1) != fieldHeight && (column -1) != -1 && mineBoard[row+1][column-1] != MINE)
            mineBoard[row+1][column-1]++;
        //Bottom center
        if ( (row+1) != fieldHeight && mineBoard[row+1][column] != MINE)
            mineBoard[row+1][column]++;
        //Bottom right
        if ( (row+1) != fieldHeight && (column+1) != fieldWidth && mineBoard[row+1][column+1] != MINE)
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
        if ( row < 0 || row > (fieldHeight - 1))
            qFatal("Error in input");
        if ( column < 0 || column > (fieldWidth - 1) )
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
    if ( row < 0 || row > (fieldHeight - 1))
        qFatal("Error in input");
    if ( column < 0 || column > (fieldWidth - 1) )
        qFatal("Error in input");

    return mineBoard[row][column];
}
