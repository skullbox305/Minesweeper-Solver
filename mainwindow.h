#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>
#include <QMainWindow>
#include <QSignalMapper>

#include "minesweeperbutton.h"
#include "performanceanalysis.h"
#include "aboutwindow.h"
#include "helpwindow.h"
#include "solver.h"

#include <QMessageBox>
#include <QVector>

#include <QTime>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Solver *so;
    PerformanceAnalysis *pf;

    QSignalMapper *signalMapperLeftClick;
    QSignalMapper *signalMapperRightClick;
    //QSignalMapper *signalMapperShowMines;

    QString buttonStyleFlatBlue;
    QString buttonStyleFlatGrey;
    QString buttonStyleFlatDarkGrey;
    QString buttonFontSmall;
    QString buttonFontBig;

    int minesFlagged;
    int flagsFlagged;

    int fieldHeight;
    int fieldWidth;
    int amountOfMines;

    void initMainWindow(bool reinitialize);
    void lost();
    void won();
    void clear(int row, int column, bool allowedClear, bool firstClearRequest);
    void changeIcon(MineSweeperButton *buttonPushed, int row, int column);
    void buttonResize();
    void setButtonTooltip(int row, int column, float probability);

private slots:
    void hasRightClicked(QString coordinates);
    void hasLeftClicked(QString coordinates);
    void reset();
    void showGamemenu();
    void handleColorLegendClick();
    void setSolverMode();
    void solverControl();
    void showUserFlags();
    void showSolution();
    void hideSolution();
    void showPerformanceAnalysisWindow();
    void showHelpWindow();
    void showAboutWindow();
    void showMinesIfChecked();
    void resetFieldStatus();

public slots:
    void markCell(int row, int column, int markAs);
    void refreshWindow();
};

#endif // MAINWINDOW_H
