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
void Minesweeper::generateBoard(bool firstClickSafe, int xFirstClick, int yFirstClick)
{
     qsrand(time(NULL));
    //Generate ten mines and create its adjacent number list
    for( int i = 0; i < amountOfMines; i++)
    {
        int xCoordinate, yCoordinate;

        //Prevent generating mine on another mine
        do
        {

            xCoordinate = qrand() % fieldHeight;
            yCoordinate = qrand() % fieldWidth;
        }while ( (mineBoard[xCoordinate][yCoordinate] == MINE) || ( (firstClickSafe && (xCoordinate == xFirstClick) && (yCoordinate == yFirstClick)) ));


        mineBoard[xCoordinate][yCoordinate] = MINE; //Number 9 indiciates a mine

        //Now that we have a mine created, increment its surroundings
        //There are eight of them

        //Top left
        if ( (xCoordinate-1) != -1 && (yCoordinate -1) != -1 && mineBoard[xCoordinate-1][yCoordinate-1] != MINE)
            mineBoard[xCoordinate-1][yCoordinate-1]++;
        //Top center
        if ( (xCoordinate-1) != -1 && mineBoard[xCoordinate-1][yCoordinate] != MINE)
            mineBoard[xCoordinate-1][yCoordinate]++;
        //Top right
        if ( (xCoordinate-1) != -1 && (yCoordinate + 1) != fieldWidth && mineBoard[xCoordinate-1][yCoordinate+1] != MINE)
            mineBoard[xCoordinate-1][yCoordinate+1]++;
        //Left
        if ( (yCoordinate -1) != -1 && mineBoard[xCoordinate][yCoordinate-1] != MINE)
            mineBoard[xCoordinate][yCoordinate-1]++;
        //Right
        if ( (yCoordinate + 1) != fieldWidth && mineBoard[xCoordinate][yCoordinate+1] != MINE)
            mineBoard[xCoordinate][yCoordinate+1]++;
        //Bottom left
        if ( (xCoordinate+1) != fieldHeight && (yCoordinate -1) != -1 && mineBoard[xCoordinate+1][yCoordinate-1] != MINE)
            mineBoard[xCoordinate+1][yCoordinate-1]++;
        //Bottom center
        if ( (xCoordinate+1) != fieldHeight && mineBoard[xCoordinate+1][yCoordinate] != MINE)
            mineBoard[xCoordinate+1][yCoordinate]++;
        //Bottom right
        if ( (xCoordinate+1) != fieldHeight && (yCoordinate+1) != fieldWidth && mineBoard[xCoordinate+1][yCoordinate+1] != MINE)
            mineBoard[xCoordinate+1][yCoordinate+1]++;
    }
}


/**
  * isMine(int, int)
  * Determines if there is a mine
  * @param int - Row to inspect
  * @param int - Column to inspect
  * @return bool - Whether or not the location is a mine
  */
bool Minesweeper::isMine(int xCoordinate, int yCoordinate)
{
    //Ensure the input is sanitary
    if ( xCoordinate < 0 || xCoordinate > (fieldHeight - 1))
        qFatal("Error in input");
    if ( yCoordinate < 0 || yCoordinate > (fieldWidth - 1) )
        qFatal("Error in input");

    return mineBoard[xCoordinate][yCoordinate] == MINE;
}

/**
  * getValue(int, int)
  * Returns the value of the given cell
  * @param int - Row to inspect
  * @param int - Column to inspect
  * @return int - The value at the row
  */
int Minesweeper::getValue(int xCoordinate, int yCoordinate)
{
    //Ensure the input is sanitary
    if ( xCoordinate < 0 || xCoordinate > (fieldHeight - 1))
        qFatal("Error in input");
    if ( yCoordinate < 0 || yCoordinate > (fieldWidth - 1) )
        qFatal("Error in input");

    return mineBoard[xCoordinate][yCoordinate];
}

