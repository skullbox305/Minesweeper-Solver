#ifndef MINESWEEPERBUTTON_H
#define MINESWEEPERBUTTON_H

#include<QPushButton>

class MineSweeperButton : public QPushButton
{
    Q_OBJECT
public:
    MineSweeperButton(QWidget* = 0);
    //MineSweeperButton(QString);

signals:
    void rightButtonClicked();

protected:
    void mousePressEvent(QMouseEvent*);
};

#endif // MINESWEEPERBUTTON_H
