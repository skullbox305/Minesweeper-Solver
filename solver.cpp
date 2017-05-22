#include "solver.h"
#include <unistd.h>
#include <iostream>
#include <QThread>

Solver::Solver(QObject *parent) : QObject(parent)
{

}

Solver::~Solver()
{
}

void Solver::startSolver(int algorithmID)
{
    fieldWidth = game->getFieldWidth();
    fieldHeight = game->getFieldHeight();
    solverRunning = true;

    if(algorithmID == 1)
    {
      naiveSinglePointSolver();
    }
    else if(algorithmID == 2)
    {
        doubleSetSinglePointSolver();
    }
    else if(algorithmID == 3)
    {

    }
}

void Solver::stopSolver()
{
    solverRunning = false;

}

void Solver::resetSolver()
{
    newGame = true;
}

bool Solver::isSolverRunning()
{
    return solverRunning;
}

void Solver::bestMove()
{

}

void Solver::setDelay(int msec)
{
    delay = msec;
}

void Solver::setMineBoard(QVector<QVector<int> > _mineboard)
{
    mineboard = _mineboard;
}

void Solver::setGameStatus(bool finished)
{
    hasFinished = finished;
    //std::cout << "test" << std::endl;
}

void Solver::setFieldStatus(QVector<QVector<int> > _fieldStatus)
{
    fieldStatus = _fieldStatus;
    //std::cout << "test" << std::endl;
}




bool Solver::allNeighborsAreMines(int row, int column)
{
    int amountOfNeighborMines = 0;
    bool result = false;

    //Top left
    if ( ((row - 1) != -1 && (column - 1) != -1) )
    {
        if(game->getFieldStatus(row - 1,column - 1) == BLANK_CELL || game->getFieldStatus(row - 1, column - 1) == FLAGGED_CELL)
        {
            amountOfNeighborMines += 1;
        }
    }

    //Top center
    if ( (row - 1) != -1)
    {
        if(game->getFieldStatus(row - 1,column) == BLANK_CELL || game->getFieldStatus(row - 1,column) == FLAGGED_CELL)
        {
            amountOfNeighborMines += 1;
        }
    }

    //Top right
    if ( (row - 1) != -1 && (column + 1) != fieldWidth)
    {
        if(game->getFieldStatus(row - 1,column + 1) == BLANK_CELL || game->getFieldStatus(row - 1,column + 1) == FLAGGED_CELL)
        {
            amountOfNeighborMines += 1;
        }
    }

    //Left
    if ( (column - 1) != -1)
    {
        if(game->getFieldStatus(row,column - 1) == BLANK_CELL || game->getFieldStatus(row,column - 1) == FLAGGED_CELL)
        {
            amountOfNeighborMines += 1;
        }
    }

    //Right
    if ( (column + 1) != fieldWidth)
    {
        if(game->getFieldStatus(row,column + 1) == BLANK_CELL || game->getFieldStatus(row,column + 1) == FLAGGED_CELL)
        {
            amountOfNeighborMines += 1;
        }
    }

    //Bottom left
    if ( (row + 1) != fieldHeight && (column - 1) != -1)
    {
        if(game->getFieldStatus(row + 1,column - 1) == BLANK_CELL || game->getFieldStatus(row + 1,column - 1) == FLAGGED_CELL)
        {
            amountOfNeighborMines += 1;
        }
    }

    //Bottom center
    if ( (row + 1) != fieldHeight)
    {
        if(game->getFieldStatus(row + 1,column) == BLANK_CELL || game->getFieldStatus(row + 1,column) == FLAGGED_CELL)
        {
            amountOfNeighborMines += 1;
        }
    }

    //Bottom right
    if ( (row + 1) != fieldHeight && (column + 1) != fieldWidth)
    {
        if(game->getFieldStatus(row + 1,column + 1) == BLANK_CELL || game->getFieldStatus(row + 1,column + 1) == FLAGGED_CELL)
        {
            amountOfNeighborMines += 1;
        }
    }

    if(/*game->getValue(row, column)*/ game->getFieldValue(row,column) == amountOfNeighborMines)
    {
        result = true;
    }

    return result;
}

QVector< QVector<int> > Solver::getAllUnmarkedNeighbors(int row, int column)
{
    QVector <int> temp(QVector<int>(2));
    QVector< QVector<int> > unmarkedNeighbors;


    //Top left
    if ( ((row - 1) != -1 && (column - 1) != -1) )
    {
        if(game->getFieldStatus(row - 1,column - 1) == BLANK_CELL )
        {
            temp[0] = row - 1;
            temp[1] = column - 1;
            unmarkedNeighbors.append(temp);
        }
    }

    //Top center
    if ( (row - 1) != -1)
    {
        if(game->getFieldStatus(row - 1,column) == BLANK_CELL)
        {
            temp[0] = row - 1;
            temp[1] = column;
            unmarkedNeighbors.append(temp);
        }
    }

    //Top right
    if ( (row - 1) != -1 && (column + 1) != fieldWidth)
    {
        if(game->getFieldStatus(row - 1,column + 1) == BLANK_CELL)
        {
            temp[0] = row - 1;
            temp[1] = column + 1;
            unmarkedNeighbors.append(temp);
        }
    }

    //Left
    if ( (column - 1) != -1)
    {
        if(game->getFieldStatus(row, column - 1) == BLANK_CELL)
        {
            temp[0] = row;
            temp[1] = column - 1;
            unmarkedNeighbors.append(temp);
        }
    }

    //Right
    if ( (column + 1) != fieldWidth)
    {
        if(game->getFieldStatus(row, column + 1) == BLANK_CELL)
        {
            temp[0] = row;
            temp[1] = column + 1;
            unmarkedNeighbors.append(temp);
        }
    }

    //Bottom left
    if ( (row + 1) != fieldHeight && (column - 1) != -1)
    {
        if(game->getFieldStatus(row + 1, column - 1) == BLANK_CELL)
        {
            temp[0] = row + 1;
            temp[1] = column - 1;
            unmarkedNeighbors.append(temp);
        }
    }

    //Bottom center
    if ( (row + 1) != fieldHeight)
    {
        if(game->getFieldStatus(row + 1, column) == BLANK_CELL)
        {
            temp[0] = row + 1;
            temp[1] = column;
            unmarkedNeighbors.append(temp);
        }
    }

    //Bottom right
    if ( (row + 1) != fieldHeight && (column + 1) != fieldWidth)
    {
        if(game->getFieldStatus(row + 1, column + 1) == BLANK_CELL)
        {
            temp[0] = row + 1;
            temp[1] = column + 1;
            unmarkedNeighbors.append(temp);
        }
    }
    return unmarkedNeighbors;
}

void Solver::naiveSinglePointSolver()
{
    int row, column;
    bool firstMove = true;
    qsrand(time(NULL));

    if(newGame)
    {
        safeCells.clear();
        questionableCells.clear();
        newGame = false;
    }

    while (!game->gameHasFinished() && solverRunning)
    {
        if(safeCells.isEmpty())
        {
            QVector <int> temp(QVector<int>(2));
            if(firstMove)
            {
                temp[0] = 0;
                temp[1] = 0;
                safeCells.append(temp);
                firstMove = false;
            } else
            {
                row = qrand() % fieldHeight;
                column = qrand() % fieldWidth;
                temp[0] = row;
                temp[1] = column;
                if(game->getFieldStatus(row, column) == BLANK_CELL || game->getFieldValue(row, column) > 0)
                {
                    safeCells.append(temp);
                }
            }
        }
        for(int x = 0; x < safeCells.size(); x++)
        {
            if(!solverRunning)
            {
                return;
            }
            if(game->getFieldStatus(safeCells[x][0],safeCells[x][1]) == BLANK_CELL)
            {
                QThread::msleep(delay);
            }
            QString coordinate = QString::number(safeCells[x][0])+","+QString::number(safeCells[x][1]); //Coordinate of the button
            emit probe(coordinate);
            //game->revealCell(safeCells[x][0], safeCells[x][1]);
            refreshWindow();

            if(game->gameHasFinished()) {
                return;
            }

            QVector< QVector<int> > unmarkedNeighbors;



            unmarkedNeighbors = getAllUnmarkedNeighbors(safeCells[x][0], safeCells[x][1]);

            if(allNeighborsAreFree(safeCells[x][0], safeCells[x][1]))
            {
                for(int y = 0; y < unmarkedNeighbors.size(); y++)
                {
                    safeCells.append(unmarkedNeighbors[y]);
                }


            } else if(allNeighborsAreMines(safeCells[x][0], safeCells[x][1]))
            {
                for(int y = 0; y < unmarkedNeighbors.size(); y++)
                {
                    markCell(unmarkedNeighbors[y][0],unmarkedNeighbors[y][1] );
                }


            } else
            {
                //ignore x
                //std::cout << "ignore x" << std::endl;


            }
            safeCells.remove(x);
        }
    }
}

void Solver::doubleSetSinglePointSolver()
{
    int row, column;
    QVector <int> opener(QVector<int>(2));
    qsrand(time(NULL));

    if(newGame)
    {
        safeCells.clear();
        questionableCells.clear();
        newGame = false;
    }

    opener[0] = 0;
    opener[1] = 0;

    safeCells.append(opener);

    while(!game->gameHasFinished() && solverRunning)
    {
        if(safeCells.isEmpty())
        {
            QVector <int> x(QVector<int>(2));
            row = qrand() % fieldHeight;
            column = qrand() % fieldWidth;
            x[0] = row;
            x[1] = column;

            safeCells.append(x);
        }
        while(!safeCells.isEmpty() && solverRunning)
        {

            QVector <int> x(QVector<int>(2));

            x[0] = safeCells.last()[0];
            x[1] = safeCells.last()[1];
            safeCells.removeLast();

            QString coordinate = QString::number(x[0])+","+QString::number(x[1]); //Coordinate of the button

            emit probe(coordinate);
            //game->revealCell(x[0],x[1]);
            refreshWindow();

            if(game->gameHasFinished())
            {
                return;
            }

            QVector< QVector<int> > unmarkedNeighbors = getAllUnmarkedNeighbors(x[0], x[1]);;

            if(allNeighborsAreFree(x[0], x[1]))
            {
                for(int i = 0; i < unmarkedNeighbors.size(); i++)
                {
                    safeCells.append(unmarkedNeighbors[i]);
                }
            }
            else
            {
                questionableCells.append(x);

            }
            if(!safeCells.isEmpty())
            {
                if(game->getFieldStatus((safeCells.last()[0]),(safeCells.last()[1])) == BLANK_CELL)
                {
                    QThread::msleep(delay);
                }
            }

        }
        for(int q = 0; q < questionableCells.size(); q++)
        {
            if(allNeighborsAreMines(questionableCells[q][0], questionableCells[q][1]))
            {

                QVector< QVector<int> > unmarkedNeighbors = getAllUnmarkedNeighbors(questionableCells[q][0], questionableCells[q][1]);


                for(int y = 0; y < unmarkedNeighbors.size(); y++)
                {
                    markCell(unmarkedNeighbors[y][0], unmarkedNeighbors[y][1]);
                    QThread::msleep(delay);
                    refreshWindow();
                }
                questionableCells.remove(q);
            }
        }

        for(int q = 0; q < questionableCells.size(); q++)
        {
            if(allNeighborsAreFree(questionableCells[q][0], questionableCells[q][1]))
            {
                QVector< QVector<int> > unmarkedNeighbors = getAllUnmarkedNeighbors(questionableCells[q][0], questionableCells[q][1]);

                safeCells.append(unmarkedNeighbors);
                questionableCells.remove(q);
            }
        }
    }
}

bool Solver::allNeighborsAreFree(int row, int column)
{
    int amountOfNeighboursFlagged = 0;
    bool result = false;

    //Top left
    if ( ((row - 1) != -1 && (column - 1) != -1) )
    {
        if(game->getFieldStatus(row - 1,column - 1) == FLAGGED_CELL)
        {
            amountOfNeighboursFlagged += 1;
        }
    }

    //Top center
    if ( (row - 1) != -1)
    {
        if(game->getFieldStatus(row - 1,column) == FLAGGED_CELL)
        {
            amountOfNeighboursFlagged += 1;
        }
    }

    //Top right
    if ( (row - 1) != -1 && (column + 1) != fieldWidth)
    {
        if(game->getFieldStatus(row - 1,column + 1) == FLAGGED_CELL)
        {
            amountOfNeighboursFlagged += 1;
        }
    }

    //Left
    if ( (column - 1) != -1)
    {
        if(game->getFieldStatus(row,column - 1) == FLAGGED_CELL)
        {
            amountOfNeighboursFlagged += 1;
        }
    }

    //Right
    if ( (column + 1) != fieldWidth)
    {
        if(game->getFieldStatus(row,column + 1) == FLAGGED_CELL)
        {
            amountOfNeighboursFlagged += 1;
        }
    }

    //Bottom left
    if ( (row + 1) != fieldHeight && (column - 1) != -1)
    {
        if(game->getFieldStatus(row + 1,column - 1) == FLAGGED_CELL)
        {
            amountOfNeighboursFlagged += 1;
        }
    }

    //Bottom center
    if ( (row + 1) != fieldHeight)
    {
        if(game->getFieldStatus(row + 1,column) == FLAGGED_CELL)
        {
            amountOfNeighboursFlagged += 1;
        }
    }

    //Bottom right
    if ( (row + 1) != fieldHeight && (column + 1) != fieldWidth)
    {
        if(game->getFieldStatus(row + 1,column + 1) == FLAGGED_CELL)
        {
            amountOfNeighboursFlagged += 1;
        }
    }

    if(/*game->getValue(row, column)*/ game->getFieldValue(row,column) == amountOfNeighboursFlagged)
    {
        result = true;
    }

    return result;
}







//void Solver::calculateProbabilitiesForAll()
//{
//    for(int xCoordinate = 0; xCoordinate < fieldHeight; xCoordinate++)
//    {
//        for ( int yCoordinate = 0; yCoordinate < fieldWidth; yCoordinate++)
//        {
//            if( (game->getValue(xCoordinate, yCoordinate) > 0) && (game->getValue(xCoordinate, yCoordinate) < 9) && (fieldStatus[xCoordinate][yCoordinate] == REVEALED_CELL))
//            {

//            }
//            probabilities[xCoordinate][yCoordinate] = 100.0 * (amountOfMines / (float)( (fieldHeight * fieldWidth) - cellsRevealed));
//            setButtonTooltip(xCoordinate,yCoordinate, probabilities[xCoordinate][yCoordinate]);
//        }
//    }
//    calculateProbabilitySinglePoint();
//}

//void Solver::calculateProbabilitySinglePoint()
//{




//    //    for(int xCoordinate = 0; xCoordinate < fieldHeight; xCoordinate++)
//    //    {
//    //        for ( int yCoordinate = 0; yCoordinate < fieldWidth; yCoordinate++)
//    //        {
//    //            amountOfNeighbour[xCoordinate][yCoordinate] = 0;

//    //            if( (game->getValue(xCoordinate, yCoordinate) > 0) && (game->getValue(xCoordinate, yCoordinate) < 9) && (fieldStatus[xCoordinate][yCoordinate] == REVEALED_CELL))
//    //            {
//    //                //Top left
//    //                if ( ((xCoordinate - 1) != -1 && (yCoordinate - 1) != -1) )
//    //                {
//    //                    if(fieldStatus[xCoordinate - 1][yCoordinate - 1] != REVEALED_CELL)
//    //                    {
//    //                        amountOfNeighbour[xCoordinate][yCoordinate] += 1;
//    //                    }
//    //                }

//    //                //Top center
//    //                if ( (xCoordinate - 1) != -1)
//    //                {
//    //                    if(fieldStatus[xCoordinate - 1][yCoordinate] != REVEALED_CELL)
//    //                    {
//    //                        amountOfNeighbour[xCoordinate][yCoordinate] += 1;
//    //                    }
//    //                }

//    //                //Top right
//    //                if ( (xCoordinate - 1) != -1 && (yCoordinate + 1) != fieldWidth)
//    //                {
//    //                    if(fieldStatus[xCoordinate - 1][yCoordinate + 1] != REVEALED_CELL)
//    //                    {
//    //                        amountOfNeighbour[xCoordinate][yCoordinate] += 1;
//    //                    }
//    //                }

//    //                //Left
//    //                if ( (yCoordinate - 1) != -1)
//    //                {
//    //                    if(fieldStatus[xCoordinate][yCoordinate - 1] != REVEALED_CELL)
//    //                    {
//    //                        amountOfNeighbour[xCoordinate][yCoordinate] += 1;
//    //                    }
//    //                }

//    //                //Right
//    //                if ( (yCoordinate + 1) != fieldWidth)
//    //                {
//    //                    if(fieldStatus[xCoordinate][yCoordinate + 1] != REVEALED_CELL)
//    //                    {
//    //                        amountOfNeighbour[xCoordinate][yCoordinate] += 1;
//    //                    }
//    //                }

//    //                //Bottom left
//    //                if ( (xCoordinate + 1) != fieldHeight && (yCoordinate - 1) != -1)
//    //                {
//    //                    if(fieldStatus[xCoordinate + 1][yCoordinate - 1] != REVEALED_CELL)
//    //                    {
//    //                        amountOfNeighbour[xCoordinate][yCoordinate] += 1;
//    //                    }
//    //                }

//    //                //Bottom center
//    //                if ( (xCoordinate + 1) != fieldHeight)
//    //                {
//    //                    if(fieldStatus[xCoordinate + 1][yCoordinate] != REVEALED_CELL)
//    //                    {
//    //                        amountOfNeighbour[xCoordinate][yCoordinate] += 1;
//    //                    }
//    //                }

//    //                //Bottom right
//    //                if ( (xCoordinate + 1) != fieldHeight && (yCoordinate + 1) != fieldWidth)
//    //                {
//    //                    if(fieldStatus[xCoordinate + 1][yCoordinate + 1] != REVEALED_CELL)
//    //                    {
//    //                        amountOfNeighbour[xCoordinate][yCoordinate] += 1;
//    //                    }
//    //                }
//    //            }
//    //            if((game->getValue(xCoordinate, yCoordinate) > 0) && (game->getValue(xCoordinate, yCoordinate) < 9) && (fieldStatus[xCoordinate][yCoordinate] == REVEALED_CELL))
//    //            {
//    //                if(game->getValue(xCoordinate,yCoordinate) == amountOfNeighbour[xCoordinate][yCoordinate])
//    //                {
//    //                    probabilities[xCoordinate][yCoordinate] = 100;
//    //                }
//    //            } else if(fieldStatus[xCoordinate][yCoordinate] != REVEALED_CELL)
//    //            {
//    //                //Top left
//    //                if ( ((xCoordinate - 1) != -1 && (yCoordinate - 1) != -1) )
//    //                {
//    //                    if(fieldStatus[xCoordinate - 1][yCoordinate - 1] != REVEALED_CELL)
//    //                    {
//    //                        amountOfNeighbour[xCoordinate][yCoordinate] += 1;
//    //                    }
//    //                }

//    //                //Top center
//    //                if ( (xCoordinate - 1) != -1)
//    //                {
//    //                    if(fieldStatus[xCoordinate - 1][yCoordinate] != REVEALED_CELL)
//    //                    {
//    //                        amountOfNeighbour[xCoordinate][yCoordinate] += 1;
//    //                    }
//    //                }

//    //                //Top right
//    //                if ( (xCoordinate - 1) != -1 && (yCoordinate + 1) != fieldWidth)
//    //                {
//    //                    if(fieldStatus[xCoordinate - 1][yCoordinate + 1] != REVEALED_CELL)
//    //                    {
//    //                        amountOfNeighbour[xCoordinate][yCoordinate] += 1;
//    //                    }
//    //                }

//    //                //Left
//    //                if ( (yCoordinate - 1) != -1)
//    //                {
//    //                    if(fieldStatus[xCoordinate][yCoordinate - 1] != REVEALED_CELL)
//    //                    {
//    //                        amountOfNeighbour[xCoordinate][yCoordinate] += 1;
//    //                    }
//    //                }

//    //                //Right
//    //                if ( (yCoordinate + 1) != fieldWidth)
//    //                {
//    //                    if(fieldStatus[xCoordinate][yCoordinate + 1] != REVEALED_CELL)
//    //                    {
//    //                        amountOfNeighbour[xCoordinate][yCoordinate] += 1;
//    //                    }
//    //                }

//    //                //Bottom left
//    //                if ( (xCoordinate + 1) != fieldHeight && (yCoordinate - 1) != -1)
//    //                {
//    //                    if(fieldStatus[xCoordinate + 1][yCoordinate - 1] != REVEALED_CELL)
//    //                    {
//    //                        amountOfNeighbour[xCoordinate][yCoordinate] += 1;
//    //                    }
//    //                }

//    //                //Bottom center
//    //                if ( (xCoordinate + 1) != fieldHeight)
//    //                {
//    //                    if(fieldStatus[xCoordinate + 1][yCoordinate] != REVEALED_CELL)
//    //                    {
//    //                        amountOfNeighbour[xCoordinate][yCoordinate] += 1;
//    //                    }
//    //                }

//    //                //Bottom right
//    //                if ( (xCoordinate + 1) != fieldHeight && (yCoordinate + 1) != fieldWidth)
//    //                {
//    //                    if(fieldStatus[xCoordinate + 1][yCoordinate + 1] != REVEALED_CELL)
//    //                    {
//    //                        amountOfNeighbour[xCoordinate][yCoordinate] += 1;
//    //                    }
//    //                }
//    //            }

//    ////            setButtonTooltip(xCoordinate,yCoordinate, probabilities[xCoordinate][yCoordinate]);
//    //        }
//    //    }
//}
