#ifndef MINESWEEPER_H
#define MINESWEEPER_H
#define MINE 9

class Minesweeper
{
public:
    Minesweeper();
    ~Minesweeper();
    bool isMine(int, int);
    int getValue(int, int);
private:
    void generateBoard();
    int mineBoard[30][20];
};

#endif // MINESWEEPER_H
