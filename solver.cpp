#include "solver.h"


Solver::Solver()
{


}

Solver::~Solver()
{
}

//bool solver::allNeighboursAreMines(int xCoordinate, int yCoordinate)
//{
//    int amountOfNeighbourMines = 0;
//    bool result = false;

//    //Top left
//    if ( ((xCoordinate - 1) != -1 && (yCoordinate - 1) != -1) )
//    {
//        if(fieldStatus[xCoordinate - 1][yCoordinate - 1] != REVEALED_CELL)
//        {
//            amountOfNeighbourMines += 1;
//        }
//    }

//    //Top center
//    if ( (xCoordinate - 1) != -1)
//    {
//        if(fieldStatus[xCoordinate - 1][yCoordinate] != REVEALED_CELL)
//        {
//            amountOfNeighbourMines += 1;
//        }
//    }

//    //Top right
//    if ( (xCoordinate - 1) != -1 && (yCoordinate + 1) != fieldWidth)
//    {
//        if(fieldStatus[xCoordinate - 1][yCoordinate + 1] != REVEALED_CELL)
//        {
//            amountOfNeighbourMines += 1;
//        }
//    }

//    //Left
//    if ( (yCoordinate - 1) != -1)
//    {
//        if(fieldStatus[xCoordinate][yCoordinate - 1] != REVEALED_CELL)
//        {
//            amountOfNeighbourMines += 1;
//        }
//    }

//    //Right
//    if ( (yCoordinate + 1) != fieldWidth)
//    {
//        if(fieldStatus[xCoordinate][yCoordinate + 1] != REVEALED_CELL)
//        {
//            amountOfNeighbourMines += 1;
//        }
//    }

//    //Bottom left
//    if ( (xCoordinate + 1) != fieldHeight && (yCoordinate - 1) != -1)
//    {
//        if(fieldStatus[xCoordinate + 1][yCoordinate - 1] != REVEALED_CELL)
//        {
//            amountOfNeighbourMines += 1;
//        }
//    }

//    //Bottom center
//    if ( (xCoordinate + 1) != fieldHeight)
//    {
//        if(fieldStatus[xCoordinate + 1][yCoordinate] != REVEALED_CELL)
//        {
//            amountOfNeighbourMines += 1;
//        }
//    }

//    //Bottom right
//    if ( (xCoordinate + 1) != fieldHeight && (yCoordinate + 1) != fieldWidth)
//    {
//        if(fieldStatus[xCoordinate + 1][yCoordinate + 1] != REVEALED_CELL)
//        {
//            amountOfNeighbourMines += 1;
//        }
//    }

//    if(game->getValue(xCoordinate, yCoordinate) == amountOfNeighbourMines)
//    {
//        result = true;
//    }

//    return result;
//}

//void solver::getAllUnmarkedNeighbours(int xCoordinate, int yCoordinate)
//{

//}

//void solver::markCell(QVector<int > unmarkedNeighbor)
//{

//}

//void solver::naiveSinglePointSolver()
//{
//    int xCoordinate, yCoordinate;
//    qsrand(time(NULL));

//    while (!hasFinished)
//    {
//        if(safeCells.isEmpty())
//        {
//            xCoordinate = qrand() % fieldHeight;
//            yCoordinate = qrand() % fieldWidth;

//            QVector <int> temp(QVector<int>(2));
//            temp[0] = xCoordinate;
//            temp[1] = yCoordinate;
//            safeCells.append(temp);
//        }
//        for(int x = 0; x < safeCells.size(); x++)
//        {
//            //            if(automatic radiobutton -> (safeCells[x] zu Qstring -> revealCell() <- flagge oder nicht. x aus S entfernen) an)
//            //            {

//            //            } else single step nach jedem Solve Button click
//            //            {

//            //            }
//            //safeCells.remove(x); in probe
//            getAllUnmarkedNeighbours(xCoordinate, yCoordinate);

//            if(allNeighboursAreFree(xCoordinate, yCoordinate))
//            {
//                for(int y = 0; y < unmarkedNeighbors.size(); y++)
//                {
//                    safeCells.append(unmarkedNeighbors[y]);
//                }
//            } else if(allNeighboursAreMines(xCoordinate, yCoordinate))
//            {
//                for(int y = 0; y < unmarkedNeighbors.size(); y++)
//                {
//                    markCell(unmarkedNeighbors[y]);
//                }
//            } else
//            {
//                //ignore x
//            }
//        }
//    }
//}

//bool solver::allNeighboursAreFree(int xCoordinate, int yCoordinate)
//{

//}

//void solver::calculateProbabilitiesForAll()
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

//void solver::calculateProbabilitySinglePoint()
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
