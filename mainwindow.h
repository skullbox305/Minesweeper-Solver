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
#include "minesweeper.h"
#include "minesweeperbutton.h"
#include <QVector>
#include <QMessageBox>

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
    QSignalMapper *signalMapperLeftClick;
    QSignalMapper *signalMapperRightClick;
    QSignalMapper *signalMapperShowMines;

    QString buttonStyleFlatBlue;
    QString buttonStyleFlatGrey;
    QString buttonStyleFlatDarkGrey;
    QString buttonFontSmall;
    QString buttonFontBig;

    int currentTime;
    bool hasStarted;
    bool hasFinished;
    int cellsRevealed;
    int minesFlagged;
    int flagsFlagged;

    int fieldHeight;
    int fieldWidth;
    int amountOfMines;

    QVector< QVector<int> > fieldStatus;
    QVector< QVector<int> > mineBoard;
    QVector< QVector<float> > probabilities;
    QVector< QVector<int> > amountOfNeighbour;

    void initMainWindow();
    void lost();
    void won();
    void clear(int, int, bool);
    void createGameVectors(int fieldWidth, int fieldHeight);
    void changeIcon(MineSweeperButton*, int, int);
    void buttonResize();
    void calculateProbabilitiesForAll();
    void calculateProbabilitySinglePoint();
    void setButtonTooltip(int xCoordinate, int yCoordinate, float probability);

private slots:
    void hasRightClicked(QString);
    void reset();

public slots:

    void revealCell(QString);
    void showMinesIfChecked();


};

#endif // MAINWINDOW_H
