#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSignalMapper>
#include "minesweeper.h"
#include "minesweeperbutton.h"
#include <QDebug>

/**
  * Constructor for MainWindow. It will initialize the entire board and create the necessary starting elements for the game
  */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    fieldWidth = 10;
    fieldHeight = 10;
    amountOfMines = 10;

    buttonStyleFlatBlue = "background-color: rgb(111, 193, 255); border-style: solid; border-color: black; border-width: 1px; border-radius: 5px;";
    buttonStyleFlatGrey = "background-color: rgb(212, 212, 212); border-style: solid; border-color: black; border-width: 1px; border-radius: 5px;";
    buttonFontSmall = "font: 10pt 'Arial';";
    buttonFontBig = "font: 12pt 'Arial';";

    ui->setupUi(this);
    QMainWindow::resize(900,850);

    //Layout designs
    ui->mineContainer->setSpacing(2); //Forces the board cells to be spaced next to each other

    //Connect the UI elements
    connect(ui->newGame, SIGNAL(pressed()), this, SLOT(reset()));
    connect(ui->showMines, SIGNAL(clicked()), this, SLOT(showMinesIfChecked()));


    //We will need to map the click to an object's coordinates
    signalMapperLeftClick = new QSignalMapper(this);
    signalMapperRightClick = new QSignalMapper(this);
    signalMapperShowMines = new QSignalMapper(this);

    //Connect the signal mapper to this class so that we can handle its clicks
    connect(signalMapperLeftClick, SIGNAL(mapped(QString)), this, SLOT(revealCell(QString)));
    connect(signalMapperRightClick, SIGNAL(mapped(QString)), this, SLOT(hasRightClicked(QString)));

    initMainWindow(false);
}

void MainWindow::initMainWindow(bool reinitialize)
{
    //Global variables
    hasFinished = false; //Has the game finished?
    hasStarted = false;
    cellsRevealed = 0; //Number of current cells revealed
    flagsFlagged = 0; //Number of flags that have been flagged
    minesFlagged = 0; //Number of mines that have been flagged

    //The display of the number of flags that have been put up (Mines left to solve)
    ui->lcdFlagCount->display ( amountOfMines - flagsFlagged );
    //Verloren oder Game Over Text löschen
    ui->gameStatus->setText(QString(""));

    //Create Vectors with the given size
    createGameVectors(fieldWidth, fieldHeight);

    //Now handle the actual game.. enough of this extra feature stuff. Now for the real deal!
    game = new Minesweeper(fieldWidth, fieldHeight, amountOfMines, mineBoard);

    //Initialize statuses
    // 0 = Empty, 1 = flagged, 2 = ?
    for ( int i = 0; i < fieldHeight; i++)
    {
        for ( int j = 0; j < fieldWidth; j++)
        {
            fieldStatus[i][j] = BLANK_CELL;
            probabilities[i][j] = i+j;

            MineSweeperButton* button = new MineSweeperButton("");

            //Button Styling
            //button->setAttribute(Qt::WA_LayoutUsesWidgetRect); //Forces Mac OS X styled minesweeper to look like linux/windows
            button->setMaximumHeight(50);
            button->setMaximumWidth(50);
            button->setIconSize (QSize(50,50));
            button->setStyleSheet(buttonStyleFlatBlue);




            //Actually add the button to the container
            ui->mineContainer->addWidget(button, i, j);

            QString coordinates = QString::number(i)+","+QString::number(j); //Coordinate of the button
            //Map the coordinates to a particular MineSweeperButton
            signalMapperRightClick->setMapping(button, coordinates);
            signalMapperLeftClick->setMapping(button, coordinates);

            //Connections for the buttons
            connect(button, SIGNAL(clicked()), signalMapperLeftClick, SLOT(map()));
            connect(button, SIGNAL(rightButtonClicked()), signalMapperRightClick, SLOT(map()));
            connect(button, SIGNAL(pressed()), this, SLOT(handleButtonPressed()));
            connect(button, SIGNAL(released()), this, SLOT(handleButtonReleased()));
        }
    }
    setButtonTooltip(1,1);
}


void MainWindow::createGameVectors(int fieldWidth, int fieldHeight)
{
    QVector< QVector<int> > result;
    QVector< QVector<float> > result1;

    for(int i = 0; i < fieldHeight; i++)
    {
        result.append(QVector<int>());
        result1.append(QVector<float>());

        for(int j = 0; j < fieldWidth; j++)
        {
            result[i].append(int());
            result1[i].append(float());
        }
    }
    fieldStatus = result;
    mineBoard = result;
    probabilities = result1;
}



/**
  * hasRightClicked()
  * This is a slot that takes a coordinate of what was just right clicked and then handles the action
  * @param QString - The coordinates of the button that was clicked
  */
void MainWindow::hasRightClicked(QString coordinates)
{
    //If we've finished the game, we don't do anything
    if (hasFinished) return;

    //Retrieve the button that was pushed from the signal mapper
    MineSweeperButton *buttonPushed = qobject_cast<MineSweeperButton *>(signalMapperLeftClick->mapping(coordinates));

    // Obtain its coordinates
    QStringList results = coordinates.split(",");

    int row = results.at(0).toInt();
    int column = results.at(1).toInt();


    //If we're not flat, it means that this button has not been pushed yet
    if (! buttonPushed->isFlat() )
    {
        //0 = flat, 1 = flat with flag, 2 = ?
        if ( fieldStatus[row][column] == BLANK_CELL)
        {
            //We are now flagging the cell as it was blank
            flagsFlagged++;

            buttonPushed->setText(QString("⚑"));
            buttonPushed->setStyleSheet("background-color: rgb(231, 197, 77); color: rgb(199, 0, 0); border-style: solid; border-color: black; border-width: 1px; border-radius: 5px;" + buttonFontBig);
            fieldStatus[row][column] = FLAGGED_CELL;

            ui->lcdFlagCount->display( NUMBER_OF_MINES - flagsFlagged );

            //Did we get a mine?
            if ( game->getValue(row, column) == MINE ) {
                minesFlagged++;
            }

        } else if ( fieldStatus[row][column] == FLAGGED_CELL )
        {
            //We are now making the thing a question mark
            flagsFlagged--;
            ui->lcdFlagCount->display( NUMBER_OF_MINES - flagsFlagged ); //No longer flagged so we are going back up

            buttonPushed->setText(QString("?"));
            buttonPushed->setStyleSheet("background-color: rgb(169, 225, 70); border-style: solid; border-color: black; border-width: 1px; border-radius: 5px;" + buttonFontBig);

            fieldStatus[row][column] = QUESTION_CELL;
        } else if ( fieldStatus[row][column] == QUESTION_CELL )
        {
            ui->lcdFlagCount->display( NUMBER_OF_MINES - flagsFlagged );
            if(ui->showMines->isChecked())
            {
                buttonPushed->setText(QString("💣"));
                buttonPushed->setStyleSheet(buttonStyleFlatBlue + buttonFontSmall);
            }
            else
            {
                buttonPushed->setText(QString(""));
                buttonPushed->setStyleSheet(buttonStyleFlatBlue);
            }

            fieldStatus[row][column] = BLANK_CELL;
        }
    } else {
        //The cell has been pushed now
        buttonPushed->setFlat(true);
    }
}


/**
  * revealCell()
  * Reveals the cell at the given coordinate, also handles losing / winning
  */
void MainWindow::revealCell(QString coordinates)
{
    //Obtain the coordinates
    QStringList results = coordinates.split(",");

    if ( results.size() != 2) //Ensure that we receive two coordinates
        qFatal("Less than two numbers received");

    int xCoordinate = results.at(0).toInt();
    int yCoordinate = results.at(1).toInt();

    if(!hasStarted)
    {
        hasStarted = true;
        game->generateBoard(ui->firstClickSafe->isChecked(), xCoordinate, yCoordinate);
        showMinesIfChecked();

    }


    //If we already finished the game.. we won't do anything here
    if (hasFinished) //schlecht, ändern
    {
        return;
    }
    else
    {
        cellsRevealed++;
    }

    //If it is flagged, we will ignore the mine
    if ( fieldStatus[xCoordinate][yCoordinate] == FLAGGED_CELL || fieldStatus[xCoordinate][yCoordinate] == QUESTION_CELL )
    {
        cellsRevealed--;
        return;
    }

    //Get the button we just pushed
    MineSweeperButton *buttonPushed = qobject_cast<MineSweeperButton *>(signalMapperLeftClick->mapping(coordinates));


    //If it is flat.. we already pushed it so ignore it
    if (buttonPushed->isFlat())
    {
        cellsRevealed--;
        return;
    }

    //If we have 90 cells revealed (10 mines, 90 not mines), we win the game!
    if (cellsRevealed == ((fieldWidth*fieldHeight) - amountOfMines) && game->getValue(xCoordinate, yCoordinate) != MINE)
    {
        changeIcon(buttonPushed, xCoordinate, yCoordinate);
        won();
        return;
    }

    //Recrusively clear the squares if we reveal a zero
    if ( game->getValue (xCoordinate, yCoordinate) == 0 ) {
        cellsRevealed--;
        clear(xCoordinate, yCoordinate, true);
    }

    //Set the image according to the value of the cell
    changeIcon(buttonPushed, xCoordinate, yCoordinate);

    buttonPushed->setFlat(true);    // if revealed, set button flat !"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    //If we reveal a mine, we just lost :(
    if ( game->isMine( xCoordinate, yCoordinate ) )
    {
        lost();
        cellsRevealed--;
        return;
    }
}

/**
  * changeIcon(MineSweeperButton, int, int)
  * Changes the icon of a given button to the icon of the number that it contains / mine
  * @param MineSweeperButton - The button to change
  * @param int - The row to inspect
  * @param int - The column to inspect
  */
void MainWindow::changeIcon(MineSweeperButton *buttonPushed, int row, int column)
{


    //Set the image according to the value of the cell
    if ( game->getValue (row, column) == 0)
    {
        buttonPushed->setText(QString(" "));
        buttonPushed->setStyleSheet(buttonStyleFlatGrey);
    } else if ( game->getValue (row,column) == 1)
    {
        buttonPushed->setText(QString("1"));
        buttonPushed->setStyleSheet("color: rgb(0,0,255);" + buttonStyleFlatGrey + buttonFontBig);
    } else if (game->getValue (row,column) == 2)
    {
        buttonPushed->setText(QString("2"));
        buttonPushed->setStyleSheet("color: rgb(0,150,0);" + buttonStyleFlatGrey + buttonFontBig);
    } else if (game->getValue (row,column) == 3)
    {
        buttonPushed->setText(QString("3"));
        buttonPushed->setStyleSheet("color: rgb(255,0,0);" + buttonStyleFlatGrey + buttonFontBig);
    } else if (game->getValue (row,column) == 4)
    {
        buttonPushed->setText(QString("4"));
        buttonPushed->setStyleSheet("color: rgb(0,200,0);" + buttonStyleFlatGrey + buttonFontBig);
    } else if (game->getValue (row,column) == 5)
    {
        buttonPushed->setText(QString("5"));
        buttonPushed->setStyleSheet("color: rgb(130,0,195);" + buttonStyleFlatGrey + buttonFontBig);
    } else if (game->getValue (row,column) == 6)
    {
        buttonPushed->setText(QString("6"));
        buttonPushed->setStyleSheet("color: rgb(85, 71, 71);" + buttonStyleFlatGrey + buttonFontBig);
    } else if (game->getValue (row,column) == 7)
    {
        buttonPushed->setText(QString("7"));
        buttonPushed->setStyleSheet("color: rgb(85, 71, 71);" + buttonStyleFlatGrey + buttonFontBig);
    } else if (game->getValue (row,column) == 8)
    {
        buttonPushed->setText(QString("8"));
        buttonPushed->setStyleSheet("color: rgb(131, 9, 9);" + buttonStyleFlatGrey + buttonFontBig);
    } else if (game->getValue(row, column) == MINE) {
        buttonPushed->setText(QString("💣"));
        buttonPushed->setStyleSheet("color: rgb(255,0,0);" + buttonStyleFlatGrey + buttonFontSmall);
    }
}

void MainWindow::showMinesIfChecked()
{
    if(hasStarted)
    {
        for ( int xCoordinate = 0; xCoordinate < fieldHeight; xCoordinate++ )
        {
            for ( int yCoordinate = 0; yCoordinate < fieldWidth; yCoordinate++ )
            {
                if(game->getValue(xCoordinate, yCoordinate) == MINE)
                {
                    QString coordinates = QString::number(xCoordinate)+","+QString::number(yCoordinate);
                    MineSweeperButton *button = qobject_cast<MineSweeperButton *>(signalMapperLeftClick->mapping(coordinates));
                    if(ui->showMines->isChecked())
                    {
                        button->setText(QString("💣"));
                        button->setStyleSheet(buttonStyleFlatBlue + buttonFontSmall);
                    } else                    {
                        button->setText(QString(""));
                        button->setStyleSheet(buttonStyleFlatBlue);
                    }
                }
            }
        }
    }

}

/**
  * clear(int row, int column, bool allowedClear)
  * clear recursively reveals all zeros and reveals its neighbors that are not mines
  * @param int - The row to inspect
  * @param int - The column to inspect
  * @param bool - If we are allowed to clear the particular cell. This is used when recursively clearing something not a zero -- it is a neighbor of a zero so it is not allowed to clear others
  */
void MainWindow::clear(int row, int column, bool allowedClear)
{
    //Retrieve the button
    QString coordinates = QString::number(row) + "," + QString::number(column);

    MineSweeperButton *buttonPushed = qobject_cast<MineSweeperButton *>(signalMapperLeftClick->mapping(coordinates));

    //Ensure that the button is not flat, that the value is not a mine, that it is allowed to clear (not something that isn't a zero) and it isn't flagged
    if ( buttonPushed->isFlat () == false && game->getValue (row, column) != MINE && allowedClear == true && fieldStatus[row][column] != FLAGGED_CELL)
    {
        buttonPushed->setFlat (true); //We're now flat

        //Set the image according to the value of the cell
        changeIcon(buttonPushed, row, column);

        cellsRevealed++;

        //If we have 90 cells revealed (10 mines, 90 not mines), we win the game!
        if (cellsRevealed == ((fieldWidth*fieldHeight) - amountOfMines) && game->getValue(row, column) != MINE)
        {
            changeIcon(buttonPushed, row, column);
            won();
            return;
        }   // in case clearing occurs in the end of the game.

        //Make sure that we are allowed to clear
        if ( game->getValue (row, column) == 0)
            allowedClear = true;
        else
            allowedClear = false;

        //Recrusively call clear on all adjacent cells
        //Top left
        if ( (row-1) != -1 && (column -1) != -1)
            clear(row-1, column-1, allowedClear);
        //Top center
        if ( (row-1) != -1)
            clear(row-1, column, allowedClear);
        //Top right
        if ( (row-1) != -1 && (column + 1) != fieldWidth)
            clear(row-1, column+1, allowedClear);
        //Left
        if ( (column -1) != -1)
            clear(row, column-1, allowedClear);
        //Right
        if ( (column + 1) != fieldWidth)
            clear(row, column+1, allowedClear);
        //Bottom left
        if ( (row+1) != fieldHeight && (column -1) != -1)
            clear(row+1, column-1, allowedClear);
        //Bottom center
        if ( (row+1) != fieldHeight)
            clear(row+1, column, allowedClear);
        //Bottom right
        if ( (row+1) != fieldHeight && (column+1) != fieldWidth)
        {
            clear(row+1, column+1, allowedClear);
        }
    }
}

/**
  * lost()
  * Handles losing the game
  */
void MainWindow::lost() {
    hasFinished = true;
    ui->gameStatus->setText(QString("FAIL"));
    ui->gameStatus->setStyleSheet("color: rgb(255,0,0); font: 75 20pt 'Arial'");

    //Go through all the cells and reveal all the mines
    for ( int i = 0; i < fieldHeight; i++ )
    {
        for ( int j = 0; j < fieldWidth; j++ )
        {
            //Get the coordinates and obtain the button
            QString coordinates = QString::number(i)+","+QString::number(j);
            MineSweeperButton *button = qobject_cast<MineSweeperButton *>(signalMapperLeftClick->mapping(coordinates));

            //Check if it's a mine, if it is.. show it!
            if (! button->isFlat () && game->getValue (i,j) == MINE )
            {
                button->setFlat (true);

                //Are we flagged? Good job! you find a mine and flagged it :)
                if ( fieldStatus[i][j] == FLAGGED_CELL ) {
                    button->setText(QString("💣"));
                    button->setStyleSheet("color: rgb(0,150,0);" + buttonStyleFlatGrey + buttonFontSmall);
                } else {
                    //BOO!! You didn't find this mine!
                    button->setText(QString("💣"));
                    button->setStyleSheet("color: rgb(0,0,0);" + buttonStyleFlatGrey + buttonFontSmall);
                }
            }

        }
    }

    //We're done with this!
    hasFinished = true;
}

/**
  * reset()
  * Resets the game board
  */
void MainWindow::reset() {
    //Reset global variables
    hasFinished = false;
    cellsRevealed = 0;
    flagsFlagged = 0;
    minesFlagged = 0;


    //Go through all the cells and reset the icons
    for( int i = 0; i < fieldHeight; i++)
    {
        for( int j = 0; j < fieldWidth; j++ )
        {
            QString coordinates = QString::number(i)+","+QString::number(j);
            MineSweeperButton *button = qobject_cast<MineSweeperButton *>(signalMapperLeftClick->mapping(coordinates));

            delete button;
        }
    }


    fieldHeight = ui->hoehe->text().toInt();
    fieldWidth = ui->breite->text().toInt();
    amountOfMines = ui->anzahlMinen->text().toInt();

    if(fieldWidth > 30)
    {
        QMessageBox messageBox;
        messageBox.information(0, "Eingabefehler", "Wert für die Feldbreite ist zu hoch. Maximal: 30");
        messageBox.setFixedSize(600,300);
        ui->breite->setText("30");
        fieldWidth = 30;
        ui->newGame->setDown(false);
    }

    if(fieldHeight > 24)
    {
        QMessageBox messageBox;
        messageBox.information(0, "Eingabefehler", "Wert für die Feldhöhe ist zu hoch. Maximal: 24");
        messageBox.setFixedSize(600,300);
        ui->hoehe->setText("24");
        fieldHeight = 24;
        ui->newGame->setDown(false);
    }

    if( ((fieldHeight * fieldWidth) * 0.927) < amountOfMines )
    {
        QMessageBox messageBox;
        int mineAmount =  (fieldHeight * fieldWidth) * 0.927;
        messageBox.information(0, "Eingabefehler", "Minenanzahl zu hoch. Maximal 92% des Feldes dürfen Minen sein. Neuer Wert: " + QString::number(mineAmount));
        messageBox.setFixedSize(600,300);
        ui->anzahlMinen->setText(QString::number(mineAmount));
        amountOfMines = mineAmount;
        ui->newGame->setDown(false);

    }


    initMainWindow(false);

}

/**
  * won()
  * Handles winning the game
  */
void MainWindow::won()
{
    ui->gameStatus->setText(QString("Gewonnen"));
    ui->gameStatus->setStyleSheet("color: rgb(0,150,0); font: 75 20pt 'Arial'");

    //Set all the mines to disarmed
    for ( int i = 0; i < fieldHeight; i++ )
    {
        for ( int j = 0; j < fieldWidth; j++ )
        {
            QString coordinates = QString::number(i)+","+QString::number(j);
            MineSweeperButton *button = qobject_cast<MineSweeperButton *>(signalMapperLeftClick->mapping(coordinates));

            if (! button->isFlat () && game->getValue(i, j) == MINE )
            {
                button->setFlat (true);
                button->setText(QString("💣"));
                button->setStyleSheet("color: rgb(0,150,0);" + buttonStyleFlatGrey + buttonFontSmall);
            }

        }
    }
}


/**
  * handleButtonPressed()
  * Handles when a button is pressed
  */
void MainWindow::handleButtonPressed()
{
    //If we have finished.. nothing happens
    if (hasFinished)
    {
        return;
    }
}


/**
  * handleButtonReleased()
  * Handles when a cell is released
  */
void MainWindow::handleButtonReleased()
{
    if (hasFinished) {
        return;
    }
}

void MainWindow::setButtonTooltip(int xCoordinate, int yCoordinate)
{
    QString coordinates = QString::number(xCoordinate)+","+QString::number(yCoordinate);
    MineSweeperButton *button = qobject_cast<MineSweeperButton *>(signalMapperLeftClick->mapping(coordinates));

    button->setToolTip("Wahrscheinlichkeit für Mine: " + QString::number(game->getProbability(0,0)) + "%");

}

/**
  * Destructor
  */
MainWindow::~MainWindow()
{
    delete ui;
}

/**
  * changeEvent()
  * Changes based on given event
  */
void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
