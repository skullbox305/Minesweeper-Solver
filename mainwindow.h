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
#include <time.h>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::MainWindow *ui;
    Solver *so;

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
    void clear(int, int, bool, bool firstClearRequest);
    void changeIcon(MineSweeperButton*, int, int);
    void buttonResize();
    void setButtonTooltip(int row, int column, float probability);

private slots:
    void hasRightClicked(QString);
    void hasLeftClicked(QString coordinates);
    void reset();
    void showGamemenu();
    void handleColorLegendClick();
    void setSolverMode();
    void solverControl();
    void showSolution();
    void hideSolution();
    void showPerformanceAnalysisWindow();
    void showHelpWindow();
    void showAboutWindow();
    void showMinesIfChecked();

public slots:
    void markCell(int row, int column, int markAs);
    void refreshWindow();

};

#endif // MAINWINDOW_H
