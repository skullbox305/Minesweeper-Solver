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
    ui->setupUi(this);
    QMainWindow::resize(800,600);

    //Global variables
    hasFinished = false; //Has the game finished?
    cellsRevealed = 0; //Number of current cells revealed
    flagsFlagged = 0; //Number of flags that have been flagged
    minesFlagged = 0; //Number of mines that have been flagged

    fieldWidth = 20;
    fieldHeight = 20;
    amountOfMines = 10;

    //Layout designs
    ui->mineContainer->setSpacing(0); //Forces the board cells to be spaced next to each other

    //The display of the number of flags that have been put up (Mines left to solve)
    ui->lcdFlagCount->display ( amountOfMines - flagsFlagged );

    //Connect the UI elements
    connect(ui->NewGame, SIGNAL(pressed()), this, SLOT(reset()));
    connect(ui->smileyFace, SIGNAL(clicked()), this, SLOT(handleSmileyFace()));


    //We will need to map the click to an object's coordinates
    signalMapper = new QSignalMapper(this);
    signalMapper2 = new QSignalMapper(this);


    initMainWindow(false);

    //Connect the signal mapper to this class so that we can handle its clicks
    connect(signalMapper, SIGNAL(mapped(QString)), this, SLOT(revealCell(QString))); //Left click
    connect(signalMapper2, SIGNAL(mapped(QString)), this, SLOT(hasRightClicked(QString))); //Right click
}

void MainWindow::initMainWindow(bool reinitialize)
{
    createGameVectors(fieldWidth, fieldHeight);

    //Now handle the actual game.. enough of this extra feature stuff. Now for the real deal!
    game = new Minesweeper(fieldWidth, fieldHeight, amountOfMines, mineBoard);

    //Initialize statuses
    // 0 = Empty, 1 = flagged, 2 = ?
    for ( int i = 0; i < fieldHeight; i++)
    {
        for ( int j = 0; j < fieldWidth; j++)
        {
            fieldStatus[i][j] = 0;

            MineSweeperButton* button = new MineSweeperButton("");

            //Button Styling
            button->setAttribute(Qt::WA_LayoutUsesWidgetRect); //Forces Mac OS X styled minesweeper to look like linux/windows
            button->setMaximumHeight(30);
            button->setMaximumWidth(30);
            button->setIcon (QIcon(QString(":/images/not_flat_button.png")));
            button->setIconSize (QSize(30,30));
            //button->

            //Actually add the button to the container
            ui->mineContainer->addWidget(button, i, j);

            QString coordinates = QString::number(i)+","+QString::number(j); //Coordinate of the button
            //Map the coordinates to a particular MineSweeperButton
            signalMapper->setMapping(button, coordinates);
            signalMapper2->setMapping(button, coordinates);

            //Connections for the buttons
            connect(button, SIGNAL(clicked()), signalMapper, SLOT(map()));
            connect(button, SIGNAL(rightButtonClicked()), signalMapper2, SLOT(map()));
            connect(button, SIGNAL(pressed()), this, SLOT(handleButtonPressed()));
            connect(button, SIGNAL(released()), this, SLOT(handleButtonReleased()));
        }
    }
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
    MineSweeperButton *buttonPushed = qobject_cast<MineSweeperButton *>(signalMapper->mapping(coordinates));

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

                    buttonPushed->setIcon (QIcon(QString(":/images/flag_no_flat_button.png")));
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

                buttonPushed->setIcon (QIcon(":/images/unknown_no_flat_button.png"));
                fieldStatus[row][column] = QUESTION_CELL;
            } else if ( fieldStatus[row][column] == QUESTION_CELL )
            {
                ui->lcdFlagCount->display( NUMBER_OF_MINES - flagsFlagged );
                buttonPushed->setIcon (QIcon(QString(":/images/not_flat_button.png")));
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

    int row = results.at(0).toInt();
    int column = results.at(1).toInt();

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
    if ( fieldStatus[row][column] == FLAGGED_CELL || fieldStatus[row][column] == QUESTION_CELL )
    {
            cellsRevealed--;
            return;
    }

    //Get the button we just pushed
    MineSweeperButton *buttonPushed = qobject_cast<MineSweeperButton *>(signalMapper->mapping(coordinates));


    //If it is flat.. we already pushed it so ignore it
    if (buttonPushed->isFlat())
    {
            cellsRevealed--;
            return;
    }

    //If we have 90 cells revealed (10 mines, 90 not mines), we win the game!
    if (cellsRevealed == ((fieldWidth*fieldHeight) - amountOfMines) && game->getValue(row, column) != MINE)
    {
        changeIcon(buttonPushed, row, column);
        won();
        return;
    }

    //Recrusively clear the squares if we reveal a zero
    if ( game->getValue (row, column) == 0 ) {
        cellsRevealed--;
        clear(row, column, true);
    }

    //Set the image according to the value of the cell
    changeIcon(buttonPushed, row, column);

    buttonPushed->setFlat(true);    // if revealed, set button flat

    //If we reveal a mine, we just lost :(
    if ( game->isMine( row, column ) )
    {
        lost();
        cellsRevealed--;
        return;
    } else {
        //Phew, it's not a mine! We're happy!!!!!!! :D
        ui->smileyFace->setIcon(QIcon(":/images/normal_face.png"));
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
        buttonPushed->setIcon (QIcon(QString(":/images/flat_button.png")));
    } else if ( game->getValue (row,column) == 1)
    {
        buttonPushed->setIcon (QIcon(QString(":/images/one_flat_button.png")));
    } else if (game->getValue (row,column) == 2)
    {
        buttonPushed->setIcon (QIcon(QString(":/images/two_flat_button.png")));
    } else if (game->getValue (row,column) == 3)
    {
        buttonPushed->setIcon (QIcon(QString(":/images/three_flat_button.png")));
    } else if (game->getValue (row,column) == 4)
    {
        buttonPushed->setIcon (QIcon(QString(":/images/four_flat_button.png")));
    } else if (game->getValue (row,column) == 5)
    {
        buttonPushed->setIcon (QIcon(QString(":/images/five_flat_button.png")));
    } else if (game->getValue (row,column) == 6)
    {
        buttonPushed->setIcon (QIcon(QString(":/images/six_flat_button.png")));
    } else if (game->getValue (row,column) == 7)
    {
        buttonPushed->setIcon (QIcon(QString(":/images/seven_flat_button.png")));
    } else if (game->getValue (row,column) == 8)
    {
        buttonPushed->setIcon (QIcon(QString(":/images/eight_flat_button.png")));
    } else if (game->getValue(row, column) == MINE) {
        buttonPushed->setIcon (QIcon(QString(":/images/mine_exploded_flat_button.png")));
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

    MineSweeperButton *buttonPushed = qobject_cast<MineSweeperButton *>(signalMapper->mapping(coordinates));

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
    ui->smileyFace->setIcon(QIcon(":/images/sad_face.png")); //I HAS THE SADS :(
    hasFinished = true;

    //Go through all the cells and reveal all the mines
    for ( int i = 0; i < fieldHeight; i++ )
    {
        for ( int j = 0; j < fieldWidth; j++ )
        {
            //Get the coordinates and obtain the button
            QString coordinates = QString::number(i)+","+QString::number(j);
            MineSweeperButton *button = qobject_cast<MineSweeperButton *>(signalMapper->mapping(coordinates));

            //Check if it's a mine, if it is.. show it!
            if (! button->isFlat () && game->getValue (i,j) == MINE )
            {
                button->setFlat (true);

                //Are we flagged? Good job! you find a mine and flagged it :)
                if ( fieldStatus[i][j] == FLAGGED_CELL ) {
                    button->setIcon (QIcon(QString(":/images/mine_disarmed_flat_button.png")));
                } else {
                    //BOO!! You didn't find this mine!
                    button->setIcon (QIcon(QString(":/images/mine_flat_button.png")));
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


    //Reset smiley to :)
    ui->smileyFace->setIcon(QIcon(":/images/normal_face.png"));

    //Create a new game object
    //game = new Minesweeper(fieldWidth, fieldHeight, amountOfMines, mineBoard);

    //Go through all the cells and reset the icons
    for( int i = 0; i < fieldHeight; i++)
    {
        for( int j = 0; j < fieldWidth; j++ )
        {
            QString coordinates = QString::number(i)+","+QString::number(j);
            MineSweeperButton *button = qobject_cast<MineSweeperButton *>(signalMapper->mapping(coordinates));

            delete button;
//            button->setIcon (QIcon(":/images/not_flat_button.png"));
//            button->setIconSize (QSize(30,30));
//            button->setFlat(false);
//            fieldStatus[i][j] = BLANK_CELL;
        }
    }
    fieldHeight = ui->hoehe->text().toInt();
    fieldWidth = ui->breite->text().toInt();
    amountOfMines = ui->anzahlMinen->text().toInt();
    initMainWindow(false);
}

/**
  * won()
  * Handles winning the game
  */
void MainWindow::won()
{
    //Happy smiley face!!! :D
    ui->smileyFace->setIcon(QIcon(":/images/happy_face.png"));

    //Stop the timer, we're done
    hasFinished = true;

    //Set all the mines to disarmed
    for ( int i = 0; i < fieldHeight; i++ )
    {
        for ( int j = 0; j < fieldWidth; j++ )
        {
            QString coordinates = QString::number(i)+","+QString::number(j);
            MineSweeperButton *button = qobject_cast<MineSweeperButton *>(signalMapper->mapping(coordinates));

            if (! button->isFlat () && game->getValue(i, j) == MINE )
            {
                button->setFlat (true);
                button->setIcon (QIcon(QString(":/images/mine_disarmed_flat_button.png")));
            }

        }
    }

    //Show the save the score screen
    //SaveScore* scoreScreen = new SaveScore(this->currentTime);
    //scoreScreen->show();
}

/**
  * handleSmileyFace()
  * Handles the smiley face when clicked which is reset the board
  */
void MainWindow::handleSmileyFace()
{
    this->reset();
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

    ui->smileyFace->setIcon(QIcon(":/images/scared_face.png")); //Oh my.. the suspense is building! :o

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

    ui->smileyFace->setIcon(QIcon(":/images/normal_face.png")); //Phew didn't click on that!
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
