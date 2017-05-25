#ifndef SOLVER_H
#define SOLVER_H


#define IS_SAFE 1
#define IS_UNCERTAIN 2
#define IS_MINE 3


#include <QObject>
#include <QVector>
#include "time.h"
#include "minesweeper.h"

class Solver : public QObject
{
    Q_OBJECT
public:
    Solver(QObject *parent);
    ~Solver();
    Minesweeper* game;

    void startSolver(int algorithmID);
    void stopSolver();
    void resetSolver();
    bool isSolverRunning();
    bool hasSolvingStarted();
    void bestMove();
    void setDelay(int msec);
    void calculateBestSolution();

private:
    void naiveSinglePointSolver();
    void doubleSetSinglePointSolver();
    void bestMoveSolver();
    bool allNeighborsAreMines(int row, int column);
    bool allNeighborsAreFree(int row, int column);
    bool allUncertainNeighborsAreMines(int row, int column);
    QVector< QVector<int> > getAllUnmarkedNeighbors(int row, int column);
    bool isUnmarkedCell(int row, int column); //falls vorher flagge oder ?

    QVector< QVector<int> > safeCells;
    QVector< QVector<int> > questionableCells;    

    int fieldHeight;
    int fieldWidth;
    int delay;
    bool solverRunning;
    bool solvingStarted;

signals:
    void probe(QString coordinate);
    void markCell(int row, int column, int markAs);
    void refreshWindow();

};

#endif // SOLVER_H
