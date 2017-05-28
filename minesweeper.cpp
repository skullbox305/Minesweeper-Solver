#include "minesweeper.h"
#include <QtGlobal>


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
    lost = false;
    won = false;

    //Create Vectors with the given size
    initGameVectors();
}

//-1 = game already finished, 0 = succesfully revealed, 1 = won, 2 = lost
int Minesweeper::revealCell(int row, int column)
{
    int result = 0;

    fieldStatus[row][column] = REVEALED_CELL;
    userFlagStatus[row][column] = REVEALED_CELL;

    QVector <int> x(QVector<int>(2));
    x[0] = row;
    x[1] = column;

    //revealedCells.append(x);

    cellsRevealed++;

    //If we have 90 cells revealed (10 mines, 90 not mines), we win the game!
    if (cellsRevealed == ((fieldWidth*fieldHeight) - amountOfMines) && mineBoard[row][column] != MINE)
    {
        result = 1;
        won = true;
        hasFinished = true;
    } else if ( mineBoard[row][column] == MINE) //If we reveal a mine, we just lost :(
    {
        result = 2;
        lost = true;
        hasFinished = true;
        cellsRevealed--;
    }


    return result;
}


bool Minesweeper::gameHasFinished()
{
    return hasFinished;
}

bool Minesweeper::gameHasStarted()
{
    return hasStarted;
}

bool Minesweeper::hasWon()
{
    return won;
}

bool Minesweeper::hasLost()
{
    return lost;
}

bool Minesweeper::isUnmarkedCell(int row, int column)
{
    bool result = false;

    if(fieldStatus[row][column] == BLANK_CELL || fieldStatus[row][column] == UNCERTAIN)
    {
        result = true;
    }
    return result;
}

void Minesweeper::markAsFlag(int row, int column)
{
    userFlagStatus[row][column] = FLAGGED_CELL;
}

void Minesweeper::markAsQuestionCell(int row, int column)
{
    userFlagStatus[row][column] = QUESTION_CELL;
}

void Minesweeper::markAsBlankCell(int row, int column, bool isUserFlag)
{
    if(isUserFlag)
    {
        userFlagStatus[row][column] = BLANK_CELL;
    }
    else
    {
        fieldStatus[row][column] = BLANK_CELL;
    }
}

void Minesweeper::markAsMine(int row, int column)
{
    fieldStatus[row][column] = MARKED_AS_MINE;
}

void Minesweeper::markAsSafe(int row, int column)
{
    fieldStatus[row][column] = SAFE;
}

void Minesweeper::markAsUncertain(int row, int column)
{
    fieldStatus[row][column] = UNCERTAIN;
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
    userFlagStatus = result;
    fieldStatus = result;
    mineBoard = result;
}


/**
  * generateBoard()
  * Generates the board with mines and populates the number of mines around it
  */
void Minesweeper::generateBoard(bool firstClickSafe, int rowFirstClick, int columnFirstClick)
{

    //Generate ten mines and create its adjacent number list
    for( int i = 0; i < amountOfMines; i++)
    {
        int row, column;

        do
        {

            row = qrand() % fieldHeight;
            column = qrand() % fieldWidth;
        }while ( (mineBoard[row][column] == MINE) || ( (firstClickSafe && (row == rowFirstClick) && (column == columnFirstClick)) ));


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

int Minesweeper::getUserFlagStatus(int row, int column)
{
    //Ensure the input is sanitary
    if ( row < 0 || row > (fieldHeight - 1))
        qFatal("Error in input");
    if ( column < 0 || column > (fieldWidth - 1) )
        qFatal("Error in input");

    return userFlagStatus[row][column];
}


