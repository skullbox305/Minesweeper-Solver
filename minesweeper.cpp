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

Minesweeper::Minesweeper(int width, int height, int mineAmount)
{
    fieldHeight = height;
    fieldWidth = width;
    amountOfMines = mineAmount;
    initGame();
}

/**
  * Destructor
  */
Minesweeper::~Minesweeper()
{
}

void Minesweeper::initGame()
{
    //Global variables
    hasFinished = false; //Has the game finished?
    hasStarted = false;
    cellsRevealed = 0; //Number of current cells revealed
    flagsFlagged = 0; //Number of flags that have been flagged
    minesFlagged = 0; //Number of mines that have been flagged
    hasLost = false;
    hasWon = false;

    //Create Vectors with the given size
    initGameVectors();
}

//-1 = game already finished, 0 = succesfully revealed, 1 = won, 2 = lost
int Minesweeper::revealCell(int row, int column)
{
    int result = 0;

    fieldStatus[row][column] = REVEALED_CELL;
    cellsRevealed++;

    //If we have 90 cells revealed (10 mines, 90 not mines), we win the game!
    if (cellsRevealed == ((fieldWidth*fieldHeight) - amountOfMines) && mineBoard[row][column] != MINE)
    {
        result = 1;
        hasWon = true;
        hasFinished = true;
    } else if ( mineBoard[row][column] == MINE) //If we reveal a mine, we just lost :(
    {
        result = 2;
        hasLost = true;
        hasFinished = true;
        cellsRevealed--;
    }


    return result;
}

bool Minesweeper::gameIsLost()
{
    return hasLost;
}

bool Minesweeper::gameIsWon()
{
    return hasWon;
}

bool Minesweeper::gameHasFinished()
{
    return hasFinished;
}

bool Minesweeper::gameHasStarted()
{
    return hasStarted;
}

void Minesweeper::setFlagCell(int row, int column)
{
    fieldStatus[row][column] = FLAGGED_CELL;
}

void Minesweeper::setQuestionCell(int row, int column)
{
    fieldStatus[row][column] = QUESTION_CELL;
}

void Minesweeper::setBlankCell(int row, int column)
{
    fieldStatus[row][column] = BLANK_CELL;
}

void Minesweeper::startGame()
{
    hasStarted = true;
}

int Minesweeper::getFieldWidth()
{
    return fieldWidth;
}

int Minesweeper::getFieldHeight()
{
    return fieldHeight;
}

void Minesweeper::initGameVectors()
{
    QVector< QVector<int> > result;

    for(int i = 0; i < fieldHeight; i++)
    {
        result.append(QVector<int>());

        for(int j = 0; j < fieldWidth; j++)
        {
            result[i].append(BLANK_CELL);
        }
    }
    fieldStatus = result;
    mineBoard = result;
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
int Minesweeper::getFieldValue(int row, int column)
{
    //Ensure the input is sanitary
    if ( row < 0 || row > (fieldHeight - 1))
        qFatal("Error in input");
    if ( column < 0 || column > (fieldWidth - 1) )
        qFatal("Error in input");

    return mineBoard[row][column];
}

int Minesweeper::getFieldStatus(int row, int column)
{
    //Ensure the input is sanitary
    if ( row < 0 || row > (fieldHeight - 1))
        qFatal("Error in input");
    if ( column < 0 || column > (fieldWidth - 1) )
        qFatal("Error in input");

    return fieldStatus[row][column];
}

QVector<QVector<int> > Minesweeper::getMineBoard()
{
    return mineBoard;
}

