#ifndef MINESWEEPERBUTTON_H
#define MINESWEEPERBUTTON_H

#include<QPushButton>

class MineSweeperButton : public QPushButton
{
    Q_OBJECT
public:
    MineSweeperButton(QWidget* parent = 0);
     ~MineSweeperButton();
    //MineSweeperButton(QString);

signals:
    void rightButtonClicked();

protected:
    void mousePressEvent(QMouseEvent *qMEvent);
};

#endif // MINESWEEPERBUTTON_H
