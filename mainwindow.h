/**
 * Minesweeper header files
 * @author Stephen Liang
 * @author Aisha Halim
 * Created for CS 340 Fall 2010 - Professor Troy
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#define BLANK_CELL 0
#define FLAGGED_CELL 1
#define QUESTION_CELL 2
#define REVEALED_CELL 3
#define MINE 9

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
    QSignalMapper *signalMapperShowMines;

    QString buttonStyleFlatBlue;
    QString buttonStyleFlatGrey;
    QString buttonStyleFlatDarkGrey;
    QString buttonFontSmall;
    QString buttonFontBig;

//    bool hasStarted;
//    bool hasFinished;
//    int cellsRevealed;
    int minesFlagged;
    int flagsFlagged;

    int fieldHeight;
    int fieldWidth;
    int amountOfMines;

//    QVector< QVector<int> > fieldStatus; //BLANK_CELL, FLAGGED_CELL, QUESTION_CELL or REVEALED_CELL
//    QVector< QVector<int> > mineBoard; //Zahl zwischen 0-9. 0-8 ist klar, 9 Mine

//    QVector< QVector<float> > probabilities; //wahrscheinlichkeiten für eine Mine
//    QVector< QVector<int> > amountOfNeighbour; // Anzahl der nicht aufgedeckten Nachbarn eines aufgedeckten Feldes mit einer Zahl zwischen 1-8

    void initMainWindow(bool reinitialize);

    void lost();
    void won();
    //void clear(int, int, bool);

    void clear(int, int, bool, bool firstClearRequest);

    //void createGameVectors(int fieldWidth, int fieldHeight);

    void changeIcon(MineSweeperButton*, int, int);
    void buttonResize();
    void setButtonTooltip(int row, int column, float probability);

private slots:
    void hasRightClicked(QString);
    void reset();

    void showGamemenu();
    void showColorLegend();
    void setSolverMode();
    void solverControl();
    void showSolution();
    void showPerformanceAnalysisWindow();
    void showHelpWindow();
    void showAboutWindow();

public slots:

    //void revealCell(QString); // wieso public?
    void hasLeftClicked(QString coordinates);
    void markCell(int row, int column);

    void showMinesIfChecked();
    void refreshWindow();

};

#endif // MAINWINDOW_H
