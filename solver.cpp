#include "solver.h"
#include <unistd.h>
#include <iostream>

Solver::Solver(QObject *parent) : QObject(parent)
{

}

Solver::~Solver()
{
}

void Solver::startSolver(int _fieldWidth, int _fieldHeight)
{
    fieldWidth = _fieldWidth;
    fieldHeight = _fieldHeight;
    naiveSinglePointSolver();
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
        if(fieldStatus[row - 1][column - 1] != REVEALED_CELL)
        {
            amountOfNeighborMines += 1;
        }
    }

    //Top center
    if ( (row - 1) != -1)
    {
        if(fieldStatus[row - 1][column] != REVEALED_CELL)
        {
            amountOfNeighborMines += 1;
        }
    }

    //Top right
    if ( (row - 1) != -1 && (column + 1) != fieldWidth)
    {
        if(fieldStatus[row - 1][column + 1] != REVEALED_CELL)
        {
            amountOfNeighborMines += 1;
        }
    }

    //Left
    if ( (column - 1) != -1)
    {
        if(fieldStatus[row][column - 1] != REVEALED_CELL)
        {
            amountOfNeighborMines += 1;
        }
    }

    //Right
    if ( (column + 1) != fieldWidth)
    {
        if(fieldStatus[row][column + 1] != REVEALED_CELL)
        {
            amountOfNeighborMines += 1;
        }
    }

    //Bottom left
    if ( (row + 1) != fieldHeight && (column - 1) != -1)
    {
        if(fieldStatus[row + 1][column - 1] != REVEALED_CELL)
        {
            amountOfNeighborMines += 1;
        }
    }

    //Bottom center
    if ( (row + 1) != fieldHeight)
    {
        if(fieldStatus[row + 1][column] != REVEALED_CELL)
        {
            amountOfNeighborMines += 1;
        }
    }

    //Bottom right
    if ( (row + 1) != fieldHeight && (column + 1) != fieldWidth)
    {
        if(fieldStatus[row + 1][column + 1] != REVEALED_CELL)
        {
            amountOfNeighborMines += 1;
        }
    }

    if(/*game->getValue(row, column)*/ mineboard[row][column] == amountOfNeighborMines)
    {
        result = true;
    }

    return result;
}

void Solver::getAllUnmarkedNeighbors(int row, int column)
{
    QVector <int> temp(QVector<int>(2));

    //Top left
    if ( ((row - 1) != -1 && (row - 1) != -1) )
    {
        if((fieldStatus[row - 1][column - 1] != REVEALED_CELL) && (fieldStatus[row - 1][column - 1] != FLAGGED_CELL) )
        {
            temp[0] = row - 1;
            temp[1] = column - 1;
            unmarkedNeighbors.append(temp);
        }
    }

    //Top center
    if ( (row - 1) != -1)
    {
        if((fieldStatus[row - 1][column] != REVEALED_CELL) && (fieldStatus[row - 1][column] != FLAGGED_CELL))
        {
            temp[0] = row - 1;
            temp[1] = column;
            unmarkedNeighbors.append(temp);
        }
    }

    //Top right
    if ( (row - 1) != -1 && (column + 1) != fieldWidth)
    {
        if((fieldStatus[row - 1][column + 1] != REVEALED_CELL) && (fieldStatus[row - 1][column + 1] != FLAGGED_CELL))
        {
            temp[0] = row - 1;
            temp[1] = column + 1;
            unmarkedNeighbors.append(temp);
        }
    }

    //Left
    if ( (column - 1) != -1)
    {
        if((fieldStatus[row ][column - 1] != REVEALED_CELL) && (fieldStatus[row][column - 1] != FLAGGED_CELL))
        {
            temp[0] = row;
            temp[1] = column - 1;
            unmarkedNeighbors.append(temp);
        }
    }

    //Right
    if ( (column + 1) != fieldWidth)
    {
        if((fieldStatus[row + 1][column] != REVEALED_CELL) && (fieldStatus[row + 1][column] != FLAGGED_CELL))
        {
            temp[0] = row;
            temp[1] = column + 1;
            unmarkedNeighbors.append(temp);
        }
    }

    //Bottom left
    if ( (row + 1) != fieldHeight && (column - 1) != -1)
    {
        if((fieldStatus[row + 1][column - 1] != REVEALED_CELL) && (fieldStatus[row + 1][column - 1] != FLAGGED_CELL))
        {
            temp[0] = row + 1;
            temp[1] = column - 1;
            unmarkedNeighbors.append(temp);
        }
    }

    //Bottom center
    if ( (row + 1) != fieldHeight)
    {
        if((fieldStatus[row + 1][column] != REVEALED_CELL) && (fieldStatus[row + 1][column] != FLAGGED_CELL))
        {
            temp[0] = row + 1;
            temp[1] = column;
            unmarkedNeighbors.append(temp);
        }
    }

    //Bottom right
    if ( (row + 1) != fieldHeight && (column + 1) != fieldWidth)
    {
        if((fieldStatus[row + 1][column + 1] != REVEALED_CELL) && (fieldStatus[row + 1][column + 1] != FLAGGED_CELL))
        {
            temp[0] = row + 1;
            temp[1] = column + 1;
            unmarkedNeighbors.append(temp);
        }
    }
}

void Solver::naiveSinglePointSolver()
{
    int row, column;
    qsrand(time(NULL));

    while (!hasFinished)
    {
        if(safeCells.isEmpty())
        {
            row = qrand() % fieldHeight;
            column = qrand() % fieldWidth;

            QVector <int> temp(QVector<int>(2));
            temp[0] = row;
            temp[1] = column;
            safeCells.append(temp);
            std::cout << "zufall" << std::endl;
            sleep(1);
        }
        for(int x = 0; x < safeCells.size(); x++)
        {
            sleep(1);
            QString coordinate = QString::number(safeCells[x][0])+","+QString::number(safeCells[x][1]); //Coordinate of the button
            emit probe(coordinate);


            if(hasFinished) {
                return;
            }


            //if(automatic radiobutton -> (safeCells[x] zu Qstring -> revealCell() <- flagge oder nicht. x aus S entfernen) an)
            //            {

            //            } else single step nach jedem Solve Button click
            //            {

            //            }
            //safeCells.remove(x); in probe

            getAllUnmarkedNeighbors(safeCells[x][0], safeCells[x][1]);

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
                std::cout << "test" << std::endl;
                //hasFinished = true;
            }
            safeCells.remove(x);
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
        if(fieldStatus[row - 1][column - 1] == FLAGGED_CELL)
        {
            amountOfNeighboursFlagged += 1;
        }
    }

    //Top center
    if ( (row - 1) != -1)
    {
        if(fieldStatus[row - 1][column] == FLAGGED_CELL)
        {
            amountOfNeighboursFlagged += 1;
        }
    }

    //Top right
    if ( (row - 1) != -1 && (column + 1) != fieldWidth)
    {
        if(fieldStatus[row - 1][column + 1] == FLAGGED_CELL)
        {
            amountOfNeighboursFlagged += 1;
        }
    }

    //Left
    if ( (column - 1) != -1)
    {
        if(fieldStatus[row][column - 1] == FLAGGED_CELL)
        {
            amountOfNeighboursFlagged += 1;
        }
    }

    //Right
    if ( (column + 1) != fieldWidth)
    {
        if(fieldStatus[row][column + 1] == FLAGGED_CELL)
        {
            amountOfNeighboursFlagged += 1;
        }
    }

    //Bottom left
    if ( (row + 1) != fieldHeight && (column - 1) != -1)
    {
        if(fieldStatus[row + 1][column - 1] == FLAGGED_CELL)
        {
            amountOfNeighboursFlagged += 1;
        }
    }

    //Bottom center
    if ( (row + 1) != fieldHeight)
    {
        if(fieldStatus[row + 1][column] == FLAGGED_CELL)
        {
            amountOfNeighboursFlagged += 1;
        }
    }

    //Bottom right
    if ( (row + 1) != fieldHeight && (column + 1) != fieldWidth)
    {
        if(fieldStatus[row + 1][column + 1] == FLAGGED_CELL)
        {
            amountOfNeighboursFlagged += 1;
        }
    }

    if(/*game->getValue(row, column)*/ mineboard[row][column] == amountOfNeighboursFlagged)
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
