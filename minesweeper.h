#ifndef MINESWEEPER_H
#define MINESWEEPER_H
#define MINE 9

#include <QVector>

class Minesweeper
{
public:
    Minesweeper(int width, int height, int mineAmount, QVector< QVector<int> > &board);
    ~Minesweeper();
    bool isMine(int, int);
    int getValue(int, int);
private:
    void generateBoard();
    //int mineBoard[20][20];

    int fieldHeight;
    int fieldWidth;
    int amountOfMines;

    QVector< QVector<int> > mineBoard;
//    QVector< QVector<int> > createMineStatusVector(int fieldWidth, int fieldHeight);
};

#endif // MINESWEEPER_H
