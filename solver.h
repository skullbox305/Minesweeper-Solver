#ifndef SOLVER_H
#define SOLVER_H
#define BLANK_CELL 0
#define FLAGGED_CELL 1
#define QUESTION_CELL 2
#define REVEALED_CELL 3
#define MINE 9

#include <QObject>
#include <QVector>
#include "time.h"

class Solver : public QObject
{
    Q_OBJECT
public:
    Solver(QObject *parent);
    ~Solver();

    void startSolver(int _fieldWidth, int _fieldHeight, int algorithmID);
    void stopSolver();
    bool isSolverRunning();
    void bestMove();
    void setDelay(int msec);
    void setMineBoard(QVector< QVector<int> > _mineboard);
    void setGameStatus(bool finished);
    void setFieldStatus(QVector< QVector<int> > fieldStatus);

private:
    QVector< QVector<int> > safeCells;
    QVector< QVector<int> > questionableCells;

    bool hasFinished;
    int fieldHeight;
    int fieldWidth;
    int delay;
    bool solverRunning;
    bool newGame;
    QVector< QVector<int> > fieldStatus;
    QVector< QVector<int> > mineboard;

    void naiveSinglePointSolver();
    void doubleSetSinglePointSolver();
    bool allNeighborsAreMines(int row, int column);
    bool allNeighborsAreFree(int row, int column);
    QVector< QVector<int> > getAllUnmarkedNeighbors(int row, int column);

signals:
    void probe(QString coordinate);
    void markCell(int row, int column);
    void refreshWindow();
};

#endif // SOLVER_H
