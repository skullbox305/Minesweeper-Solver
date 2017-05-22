#ifndef MINESWEEPER_H
#define MINESWEEPER_H
#define BLANK_CELL 0
#define FLAGGED_CELL 1
#define QUESTION_CELL 2
#define REVEALED_CELL 3
#define MINE 9

#include <QVector>
#include <QObject>
#include <time.h>

class Minesweeper
{
public:
    Minesweeper(int width, int height, int mineAmount, QVector< QVector<int> > &board);
    Minesweeper(int width, int height, int mineAmount);
    ~Minesweeper();
    void generateBoard(bool firstClickSafe, int xFirstClick, int yFirstClick);
    bool isMine(int, int);
    int getFieldValue(int row, int column);
    int getFieldStatus(int row, int column);
    QVector< QVector<int> > getMineBoard();
    void initGame();
    int revealCell(int row, int column);
    bool gameIsLost();
    bool gameIsWon();
    bool gameHasFinished();
    bool gameHasStarted();
    void setFlagCell(int row, int column);
    void setQuestionCell(int row, int column);
    void setBlankCell(int row, int column);
    void startGame();
    int getFieldWidth();
    int getFieldHeight();

private:
    bool hasStarted;
    bool hasFinished;
    bool hasLost;
    bool hasWon;


    int cellsRevealed;
    int minesFlagged;
    int flagsFlagged;

    int fieldHeight;
    int fieldWidth;
    int amountOfMines;

    QVector< QVector<int> > fieldStatus; //BLANK_CELL, FLAGGED_CELL, QUESTION_CELL or REVEALED_CELL
    QVector< QVector<int> > mineBoard; //Zahl zwischen 0-9. 0-8 ist klar, 9 Mine

    void initGameVectors();
};

#endif // MINESWEEPER_H
