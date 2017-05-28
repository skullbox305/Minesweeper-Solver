#ifndef MINESWEEPER_H
#define MINESWEEPER_H

#define FLAGGED_CELL 2 //User flag
#define QUESTION_CELL 3 // User flag

#define BLANK_CELL 0
#define REVEALED_CELL 1
#define MARKED_AS_MINE 2 //Solver/Best Move  flag
#define SAFE 3 //Best Move flag
#define UNCERTAIN 4 //Best move flag
#define BEST_GUESS 5  //Best move flag

#define MINE 9

#include <QVector>
#include <QObject>

class Minesweeper
{
public:
    Minesweeper(int width, int height, int mineAmount);
    ~Minesweeper();
    void generateBoard(bool firstClickSafe, int rowFirstClick, int columnFirstClick);
    int revealCell(int row, int column);


    //Setter
    void markAsFlag(int row, int column);
    void markAsQuestionCell(int row, int column);
    void markAsBlankCell(int row, int column, bool isUserFlag);
    void markAsMine(int row, int column);
    void markAsSafe(int row, int column);
    void markAsUncertain(int row, int column);
    void startGame();
    void initGame();

    //Getter
    int getFieldWidth();
    int getFieldHeight();
    int getFieldValue(int row, int column);
    int getFieldStatus(int row, int column);
    int getUserFlagStatus(int row, int column);

    //Status
    bool gameHasFinished();
    bool gameHasStarted();
    bool hasWon();
    bool hasLost();
    bool isUnmarkedCell(int row, int column);

private:
    bool hasStarted;
    bool hasFinished;
    bool lost;
    bool won;
    int cellsRevealed;

    int fieldHeight;
    int fieldWidth;
    int amountOfMines;

    QVector< QVector<int> > fieldStatus; //BLANK_CELL, FLAGGED_CELL, QUESTION_CELL or REVEALED_CELL
    QVector< QVector<int> > userFlagStatus;
    QVector< QVector<int> > mineBoard; //Zahl zwischen 0-9. 0-8 ist klar, 9 Mine

    void initGameVectors();

};

#endif // MINESWEEPER_H
