#ifndef SOLVER_H
#define SOLVER_H
#define BLANK_CELL 0
#define FLAGGED_CELL 1
#define QUESTION_CELL 2
#define REVEALED_CELL 3
#define MINE 9

#include <QVector>
#include "mainwindow.h"

class Solver
{
public:
    Solver();
    ~Solver();


    bool allNeighboursAreMines(int xCoordinate, int yCoordinate);
    bool allNeighboursAreFree(int xCoordinate, int yCoordinate);
    void getAllUnmarkedNeighbours(int xCoordinate, int yCoordinate);
    void markCell(QVector<int> unmarkedNeighbor);
    void naiveSinglePointSolver();

private:

};

#endif // SOLVER_H
