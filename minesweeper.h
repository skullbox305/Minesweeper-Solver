#ifndef MINESWEEPER_H
#define MINESWEEPER_H
#define MINE 9

#include <QVector>

class Minesweeper
{
public:
    Minesweeper(int width, int height, int mineAmount, QVector< QVector<int> > &board);
    ~Minesweeper();
    void generateBoard(bool firstClickSafe, int xFirstClick, int yFirstClick);
    bool isMine(int, int);
    int getValue(int, int);
    float getProbability(int, int);
private:

    void calculateProbabilities();

    int fieldHeight;
    int fieldWidth;
    int amountOfMines;

    QVector< QVector<int> > mineBoard;

};

#endif // MINESWEEPER_H
