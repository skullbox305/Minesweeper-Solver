#ifndef PERFORMANCEANALYSIS_H
#define PERFORMANCEANALYSIS_H

#include "solver.h"
#include <QWidget>

namespace Ui {
class PerformanceAnalysis;
}

class PerformanceAnalysis : public QWidget
{
    Q_OBJECT

public:
    explicit PerformanceAnalysis(QWidget *parent = 0);
    ~PerformanceAnalysis();

private:
    Ui::PerformanceAnalysis *ui;
    Solver *so;

    int fieldWidth;
    int fieldHeight;
    int amountOfMines;
    int amountOfGames;

    int gamesWon;
    int gamesLost;
    float winRate;

private slots:
    void startPerformanceAnalysis();
    void handleBackButton();

signals:
    void refreshWindow();
};

#endif // PERFORMANCEANALYSIS_H
