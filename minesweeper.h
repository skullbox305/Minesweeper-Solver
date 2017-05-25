#ifndef MINESWEEPER_H
#define MINESWEEPER_H

#define BLANK_CELL 0
#define FLAGGED_CELL 1
#define QUESTION_CELL 2
#define REVEALED_CELL 3
#define MARKED_AS_MINE 4
#define SAFE 5
#define UNCERTAIN 6
#define BEST_GUESS 7

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
    int revealCell(int row, int column);

    //Setter
    void markAsFlag(int row, int column);
    void markAsQuestionCell(int row, int column);
    void markAsBlankCell(int row, int column);
    void markAsMine(int row, int column);
    void markAsSafe(int row, int column);
    void markAsUncertain(int row, int column);
    void startGame();

    //Getter/Gamestatus
    int getFieldWidth();
    int getFieldHeight();
    int getFieldValue(int row, int column);
    int getFieldStatus(int row, int column);

    bool gameIsLost();
    bool gameIsWon();
    bool gameHasFinished();
    bool gameHasStarted();

private:
    bool hasStarted;
    bool hasFinished;
    bool hasLost;
    bool hasWon;
    int cellsRevealed;

    int fieldHeight;
    int fieldWidth;
    int amountOfMines;

    QVector< QVector<int> > fieldStatus; //BLANK_CELL, FLAGGED_CELL, QUESTION_CELL or REVEALED_CELL
    QVector< QVector<int> > mineBoard; //Zahl zwischen 0-9. 0-8 ist klar, 9 Mine

    void initGameVectors();
    void initGame();
};

#endif // MINESWEEPER_H
