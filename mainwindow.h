/**
 * Minesweeper header files
 * @author Stephen Liang
 * @author Aisha Halim
 * Created for CS 340 Fall 2010 - Professor Troy
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#define NUMBER_OF_MINES 10
#define BLANK_CELL 0
#define FLAGGED_CELL 1
#define QUESTION_CELL 2
#define MINE 9

#include <iostream>
#include <QMainWindow>
#include <QSignalMapper>
#include "minesweeper.h"
#include "minesweeperbutton.h"
#include <QVector>

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
    Minesweeper *game;
    QSignalMapper *signalMapper;
    QSignalMapper *signalMapper2;

    int currentTime;
    bool hasStarted;
    bool hasFinished;
    int cellsRevealed;
    int minesFlagged;
    int flagsFlagged;

    void initMainWindow(bool reinitialize);
    void lost();
    void won();
    void clear(int, int, bool);


    int fieldHeight;
    int fieldWidth;
    int amountOfMines;
    QVector< QVector<int> > fieldStatus;
    QVector< QVector<int> > mineBoard;
    QVector< QVector<float> > probabilities;

    void createGameVectors(int fieldWidth, int fieldHeight);

    void changeIcon(MineSweeperButton*, int, int);


private slots:
    void hasRightClicked(QString);
    void reset();
    void handleSmileyFace();
    void handleButtonPressed();
    void handleButtonReleased();
    //void revealAll(QString);

public slots:

    void revealCell(QString);


};

#endif // MAINWINDOW_H
