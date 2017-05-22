#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSignalMapper>
#include <QDebug>
#include <QRect>

/**
  * Constructor for MainWindow. It will initialize the entire board and create the necessary starting elements for the game
  */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    so = new Solver(this);

    fieldWidth = 10;
    fieldHeight = 10;
    amountOfMines = 10;

    buttonStyleFlatBlue = "background-color: rgb(111, 193, 255); border-style: solid; border-color: black; border-width: 1px; border-radius: 5px;";
    buttonStyleFlatGrey = "background-color: rgb(212, 212, 212); border-style: solid; border-color: black; border-width: 1px; border-radius: 5px;";
    buttonStyleFlatDarkGrey = "background-color: rgb(150, 150, 150); border-style: solid; border-color: black; border-width: 1px; border-radius: 5px;";
    buttonFontSmall = "font: 10pt 'Arial';";
    buttonFontBig = "font: 14pt 'Arial';";

    ui->setupUi(this);
    QMainWindow::showMaximized();

    //Layout designs
    ui->mineContainer->setSpacing(2); //Forces the board cells to be spaced next to each other
    ui->solverStartButton->setStyleSheet(buttonStyleFlatBlue + buttonFontSmall + "color: rgb(0,0,0);");
    ui->newGame->setStyleSheet(buttonStyleFlatBlue + buttonFontSmall + "color: rgb(0,0,0);");

    //Connect the UI elements
    connect(ui->newGame, SIGNAL(released()), this, SLOT(reset()));
    connect(ui->showMines, SIGNAL(clicked()), this, SLOT(showMinesIfChecked()));
    connect(ui->button_GameOptions, SIGNAL(released()), this, SLOT(showGamemenu()));
    connect(ui->cbShowSolution, SIGNAL(clicked(bool)), this, SLOT(showColorLegend()));
    connect(ui->radioButton_SolverAuto, SIGNAL(clicked()), this, SLOT(setSolverMode()));
    connect(ui->radioButton_SolverManual, SIGNAL(clicked()), this, SLOT(setSolverMode()));
    connect(ui->solverStartButton, SIGNAL(clicked()), this, SLOT(solverControl()));
    connect(ui->button_PerformanceAnalysis, SIGNAL(released()), this, SLOT(showPerformanceAnalysisWindow()));
    connect(ui->button_Help, SIGNAL(released()), this, SLOT(showHelpWindow()));
    connect(ui->button_About, SIGNAL(released()), this, SLOT(showAboutWindow()));

    connect(so, SIGNAL(probe(QString)), this, SLOT(hasLeftClicked(QString)));
    connect(so, SIGNAL(markCell(int,int)), this, SLOT(markCell(int,int)));
    connect(so, SIGNAL(refreshWindow()), this, SLOT(refreshWindow()));

    showColorLegend();
    setSolverMode();

    //We will need to map the click to an object's coordinates
    signalMapperLeftClick = new QSignalMapper(this);
    signalMapperRightClick = new QSignalMapper(this);
    signalMapperShowMines = new QSignalMapper(this);

    //Connect the signal mapper to this class so that we can handle its clicks
    connect(signalMapperLeftClick, SIGNAL(mapped(QString)), this, SLOT(hasLeftClicked(QString)));
    connect(signalMapperRightClick, SIGNAL(mapped(QString)), this, SLOT(hasRightClicked(QString)));

    initMainWindow(false);

}

void MainWindow::initMainWindow(bool reinitialize)
{
    //Global variables
    //hasFinished = false; //Has the game finished?

    //so->setGameStatus(hasFinished);

    //hasStarted = false;
    //cellsRevealed = 0; //Number of current cells revealed

    flagsFlagged = 0; //Number of flags that have been flagged
    minesFlagged = 0; //Number of mines that have been flagged

    //The display of the number of flags that have been put up (Mines left to solve)
    ui->lcdFlagCount->display ( amountOfMines - flagsFlagged );

    //Create Vectors with the given size
    //createGameVectors(fieldWidth, fieldHeight);

    //Now handle the actual game.. enough of this extra feature stuff. Now for the real deal!
    so->game = new Minesweeper(fieldWidth, fieldHeight, amountOfMines);


    //Initialize statuses
    // 0 = Empty, 1 = flagged, 2 = ?
    for ( int i = 0; i < fieldHeight; i++)
    {
        for ( int j = 0; j < fieldWidth; j++)
        {
            //fieldStatus[i][j] = BLANK_CELL;


            //probabilities[i][j] = i+j;
            //amountOfNeighbour[i][j] = 0;


            if(reinitialize)
            {
                QString coordinates = QString::number(i)+","+QString::number(j);
                MineSweeperButton *button = qobject_cast<MineSweeperButton *>(signalMapperLeftClick->mapping(coordinates));

                button->setStyleSheet(buttonStyleFlatBlue);
                button->setText("");
            } else
            {
                MineSweeperButton *button = new MineSweeperButton(this);

                if(fieldHeight <= 9 && fieldWidth <= 11)
                {
                    button->setFixedSize(70,70);
                } else if(fieldHeight <= 10 && fieldWidth <= 12)
                {
                    button->setFixedSize(65,65);
                } else if(fieldHeight <= 11 && fieldWidth <= 13)
                {
                    button->setFixedSize(60,60);
                } else if(fieldHeight <= 12 && fieldWidth <= 14)
                {
                    button->setFixedSize(55,55);
                } else if(fieldHeight <= 13 && fieldWidth <= 15)
                {
                    button->setFixedSize(50,50);
                } else if(fieldHeight <= 14 && fieldWidth <= 18)
                {
                    button->setFixedSize(45,45);
                } else if(fieldHeight <= 15 && fieldWidth <= 20)
                {
                    button->setFixedSize(40,40);
                } else if(fieldHeight <= 18 && fieldWidth <= 22)
                {
                    button->setFixedSize(35,35);
                } else if(fieldHeight <= 21 && fieldWidth <= 26)
                {
                    button->setFixedSize(30,30);
                } else if(fieldHeight <= 24 && fieldWidth <= 30)
                {
                    button->setFixedSize(26,26);
                }

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
            }
        }

        if(!reinitialize)
        {
            if( (i % 2) == 0 )
            {
                ui->scrollArea->show();
                qApp->processEvents();
            } else if((i % 2) == 1 && i < fieldHeight - 1)
            {
                ui->scrollArea->hide();
            }
        } else
        {
            ui->scrollArea->hide();
        }

    }
    //so->setFieldStatus(fieldStatus);
    ui->scrollArea->show();
}


//void MainWindow::createGameVectors(int fieldWidth, int fieldHeight)
//{
//    QVector< QVector<int> > result;
//    QVector< QVector<float> > result1;

//    for(int i = 0; i < fieldHeight; i++)
//    {
//        result.append(QVector<int>());
//        result1.append(QVector<float>());

//        for(int j = 0; j < fieldWidth; j++)
//        {
//            result[i].append(int());
//            result1[i].append(float());
//        }
//    }
//    fieldStatus = result;
//    so->setFieldStatus(fieldStatus);
//    mineBoard = result;
//    //    amountOfNeighbour = result;
//    //    probabilities = result1;
//}



/**
  * hasRightClicked()
  * This is a slot that takes a coordinate of what was just right clicked and then handles the action
  * @param QString - The coordinates of the button that was clicked
  */
void MainWindow::hasRightClicked(QString coordinates)
{
    //If we've finished the game, we don't do anything
    if (!so->game->gameHasFinished() && !ui->showMines->isChecked())
    {
        //Retrieve the button that was pushed from the signal mapper
        MineSweeperButton *buttonPushed = qobject_cast<MineSweeperButton *>(signalMapperLeftClick->mapping(coordinates));

        // Obtain its coordinates
        QStringList results = coordinates.split(",");

        int row = results.at(0).toInt();
        int column = results.at(1).toInt();


        //If we're not flat, it means that this button has not been pushed yet
        if(!(so->game->getFieldStatus(row, column) == REVEALED_CELL))
        {
            //0 = flat, 1 = flat with flag, 2 = ?
            if (so->game->getFieldStatus(row, column) == BLANK_CELL)
            {
                //We are now flagging the cell as it was blank
                flagsFlagged++;

                buttonPushed->setText(QString("⚑"));
                buttonPushed->setStyleSheet("background-color: rgb(231, 197, 77); color: rgb(199, 0, 0); border-style: solid; border-color: black; border-width: 1px; border-radius: 5px;" + buttonFontBig);
                so->game->setFlagCell(row,column);
                //so->setFieldStatus(fieldStatus);

                ui->lcdFlagCount->display( amountOfMines - flagsFlagged );

//                //Did we get a mine?
//                if ( game->getFieldValue(row, column) == MINE ) {
//                    minesFlagged++;
//                }

            } else if (so->game->getFieldStatus(row, column) == FLAGGED_CELL )
            {
                //We are now making the thing a question mark
                flagsFlagged--;
                ui->lcdFlagCount->display( amountOfMines - flagsFlagged ); //No longer flagged so we are going back up

                buttonPushed->setText(QString("?"));
                buttonPushed->setStyleSheet("background-color: rgb(169, 225, 70); color: rgb(0,0,0); border-style: solid; border-color: black; border-width: 1px; border-radius: 5px;" + buttonFontBig);

                so->game->setQuestionCell(row, column);
                //so->setFieldStatus(fieldStatus);

            } else if (so->game->getFieldStatus(row, column) == QUESTION_CELL )
            {
                ui->lcdFlagCount->display( amountOfMines - flagsFlagged );

                buttonPushed->setText(QString(""));
                buttonPushed->setStyleSheet(buttonStyleFlatBlue);

                so->game->setBlankCell(row,column);
                //so->setFieldStatus(fieldStatus);
            }
        }
        buttonPushed->clearFocus();
    }
}

void MainWindow::markCell(int row, int column)
{
    QString coordinates = QString::number(row)+","+QString::number(column); //Coordinate of the button
    MineSweeperButton *button = qobject_cast<MineSweeperButton *>(signalMapperLeftClick->mapping(coordinates));

    //We are now flagging the cell as it was blank
    flagsFlagged++;

    if(ui->cbShowSolution->isChecked())
    {
        button->setText(QString(""));
        button->setStyleSheet("background-color:  rgb(211, 160, 160); color: rgb(199, 0, 0); border-style: solid; border-color: black; border-width: 1px; border-radius: 5px;" + buttonFontBig);
    }

    so->game->setFlagCell(row,column);
    //so->setFieldStatus(fieldStatus);

    ui->lcdFlagCount->display( amountOfMines - flagsFlagged );

}

/**
  * revealCell()
  * Reveals the cell at the given coordinate, also handles losing / winning
  */
void MainWindow::hasLeftClicked(QString coordinates)
{
    if(!so->game->gameHasFinished())
    {
        //Obtain the coordinates
        QStringList results = coordinates.split(",");

        if ( results.size() != 2) //Ensure that we receive two coordinates
            qFatal("Less than two numbers received");

        int row = results.at(0).toInt();
        int column = results.at(1).toInt();

        if(!so->game->gameHasStarted())
        {
            so->game->generateBoard(ui->firstClickSafe->isChecked(), row, column);
            so->game->startGame();
            showMinesIfChecked();
        }

        int _fieldStatus = so->game->getFieldStatus(row, column);
        //If it is flagged, we will ignore the mine
        if (! ( _fieldStatus == FLAGGED_CELL || _fieldStatus == QUESTION_CELL || _fieldStatus == REVEALED_CELL))
        {
            int result;
            //Get the button we just pushed
            MineSweeperButton *buttonPushed = qobject_cast<MineSweeperButton *>(signalMapperLeftClick->mapping(coordinates));
            result = so->game->revealCell(row,column);

            changeIcon(buttonPushed, row, column);

            if(result == 1)
            {
                won();
            }
            else if(result == 2)
            {
                lost();
            }
            else if(result == 0)
            {
                //Recrusively clear the squares if we reveal a zero
                if ( so->game->getFieldValue (row, column) == 0 && !so->isSolverRunning())
                {
                    clear(row, column, true, true);
                }
            }
        }
    }
}

///**
//  * revealCell()
//  * Reveals the cell at the given coordinate, also handles losing / winning
//  */ // teeest
//void MainWindow::revealCell(QString coordinates)
//{
//    if(!hasFinished)
//    {
//        //Obtain the coordinates
//        QStringList results = coordinates.split(",");

//        if ( results.size() != 2) //Ensure that we receive two coordinates
//            qFatal("Less than two numbers received");

//        int xCoordinate = results.at(0).toInt();
//        int yCoordinate = results.at(1).toInt();

//        if(!hasStarted)
//        {
//            hasStarted = true;
//            game->generateBoard(ui->firstClickSafe->isChecked(), xCoordinate, yCoordinate);
//            so->setMineBoard(game->getMineBoard());

//            showMinesIfChecked();

//        }

//        cellsRevealed++;

//        //If it is flagged, we will ignore the mine
//        if ( fieldStatus[xCoordinate][yCoordinate] == FLAGGED_CELL || fieldStatus[xCoordinate][yCoordinate] == QUESTION_CELL )
//        {
//            cellsRevealed--;
//            return;
//        }

//        //Get the button we just pushed
//        MineSweeperButton *buttonPushed = qobject_cast<MineSweeperButton *>(signalMapperLeftClick->mapping(coordinates));


//        //If it is flat.. we already pushed it so ignore it
//        if(fieldStatus[xCoordinate][yCoordinate] == REVEALED_CELL)
//        {
//            cellsRevealed--;
//            return;
//        }

//        //If we have 90 cells revealed (10 mines, 90 not mines), we win the game!
//        if (cellsRevealed == ((fieldWidth*fieldHeight) - amountOfMines) && game->getFieldValue(xCoordinate, yCoordinate) != MINE)
//        {
//            changeIcon(buttonPushed, xCoordinate, yCoordinate);
//            won();
//            return;
//        }

//        //Recrusively clear the squares if we reveal a zero
//        if ( game->getFieldValue (xCoordinate, yCoordinate) == 0 && !so->isSolverRunning()) {
//            cellsRevealed--;
//            clear(xCoordinate, yCoordinate, true);
//        }

//        //Set the image according to the value of the cell
//        changeIcon(buttonPushed, xCoordinate, yCoordinate);

//        //buttonPushed->setFlat(true);    // if revealed, set button flat !"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//        fieldStatus[xCoordinate][yCoordinate] = REVEALED_CELL;
//        so->setFieldStatus(fieldStatus);

//        //If we reveal a mine, we just lost :(
//        if ( game->isMine( xCoordinate, yCoordinate ) )
//        {
//            lost();
//            cellsRevealed--;
//            return;
//        }
//    }



//    //calculateProbabilitiesForAll();
//}

/**
  * changeIcon(MineSweeperButton, int, int)
  * Changes the icon of a given button to the icon of the number that it contains / mine
  * @param MineSweeperButton - The button to change
  * @param int - The row to inspect
  * @param int - The column to inspect
  */
void MainWindow::changeIcon(MineSweeperButton *buttonPushed, int row, int column)
{
    buttonPushed->clearFocus();

    //Set the image according to the value of the cell
    if ( so->game->getFieldValue (row, column) == 0)
    {
        buttonPushed->setText(QString(" "));
        buttonPushed->setStyleSheet(buttonStyleFlatGrey);
    } else if ( so->game->getFieldValue (row,column) == 1)
    {
        buttonPushed->setText(QString("1"));
        buttonPushed->setStyleSheet("color: rgb(0,0,255);" + buttonStyleFlatGrey + buttonFontBig);
    } else if (so->game->getFieldValue (row,column) == 2)
    {
        buttonPushed->setText(QString("2"));
        buttonPushed->setStyleSheet("color: rgb(0,150,0);" + buttonStyleFlatGrey + buttonFontBig);
    } else if (so->game->getFieldValue (row,column) == 3)
    {
        buttonPushed->setText(QString("3"));
        buttonPushed->setStyleSheet("color: rgb(255,0,0);" + buttonStyleFlatGrey + buttonFontBig);
    } else if (so->game->getFieldValue (row,column) == 4)
    {
        buttonPushed->setText(QString("4"));
        buttonPushed->setStyleSheet("color: rgb(0,200,0);" + buttonStyleFlatGrey + buttonFontBig);
    } else if (so->game->getFieldValue (row,column) == 5)
    {
        buttonPushed->setText(QString("5"));
        buttonPushed->setStyleSheet("color: rgb(130,0,195);" + buttonStyleFlatGrey + buttonFontBig);
    } else if (so->game->getFieldValue (row,column) == 6)
    {
        buttonPushed->setText(QString("6"));
        buttonPushed->setStyleSheet("color: rgb(85, 71, 71);" + buttonStyleFlatGrey + buttonFontBig);
    } else if (so->game->getFieldValue (row,column) == 7)
    {
        buttonPushed->setText(QString("7"));
        buttonPushed->setStyleSheet("color: rgb(85, 71, 71);" + buttonStyleFlatGrey + buttonFontBig);
    } else if (so->game->getFieldValue (row,column) == 8)
    {
        buttonPushed->setText(QString("8"));
        buttonPushed->setStyleSheet("color: rgb(131, 9, 9);" + buttonStyleFlatGrey + buttonFontBig);
    } else if (so->game->getFieldValue(row, column) == MINE) {
        buttonPushed->setText(QString("💣"));
        buttonPushed->setStyleSheet("color: rgb(255,0,0);" + buttonStyleFlatGrey + buttonFontSmall);
    }
}

void MainWindow::showMinesIfChecked()
{
    if(so->game->gameHasStarted() && !so->game->gameHasFinished())
    {

        ui->scrollArea->hide();
        for ( int row = 0; row < fieldHeight; row++ )
        {
            for ( int column = 0; column < fieldWidth; column++ )
            {
                if(so->game->getFieldValue(row, column) == MINE)
                {
                    QString coordinates = QString::number(row)+","+QString::number(column);
                    MineSweeperButton *button = qobject_cast<MineSweeperButton *>(signalMapperLeftClick->mapping(coordinates));
                    if(ui->showMines->isChecked())
                    {
                        button->setText(QString("💣"));
                        button->setStyleSheet("color: rgb(0,0,0);" + buttonStyleFlatBlue + buttonFontSmall);
                    } else
                    {
                        if(so->game->getFieldStatus(row, column) == FLAGGED_CELL)
                        {
                            button->setText(QString("⚑"));
                            button->setStyleSheet("background-color: rgb(231, 197, 77); color: rgb(199, 0, 0); border-style: solid; border-color: black; border-width: 1px; border-radius: 5px;" + buttonFontBig);
                        }
                        else if(so->game->getFieldStatus(row, column) == QUESTION_CELL)
                        {
                            button->setText(QString("?"));
                            button->setStyleSheet("background-color: rgb(169, 225, 70); color: rgb(0,0,0); border-style: solid; border-color: black; border-width: 1px; border-radius: 5px;" + buttonFontBig);
                        }
                        else
                        {
                            button->setText(QString(""));
                            button->setStyleSheet(buttonStyleFlatBlue);
                        }
                    }
                }
            }

        }
        ui->scrollArea->show();
    }

}

void MainWindow::refreshWindow()
{
    qApp->processEvents();
}

/**
  * clear(int row, int column, bool allowedClear)
  * clear recursively reveals all zeros and reveals its neighbors that are not mines
  * @param int - The row to inspect
  * @param int - The column to inspect
  * @param bool - If we are allowed to clear the particular cell. This is used when recursively clearing something not a zero -- it is a neighbor of a zero so it is not allowed to clear others
  */
//void MainWindow::clear(int row, int column, bool allowedClear)
//{
//    //Retrieve the button
//    QString coordinates = QString::number(row) + "," + QString::number(column);

//    MineSweeperButton *buttonPushed = qobject_cast<MineSweeperButton *>(signalMapperLeftClick->mapping(coordinates));


//    //Ensure that the button is not flat, that the value is not a mine, that it is allowed to clear (not something that isn't a zero) and it isn't flagged
//    if (fieldStatus[row][column] != REVEALED_CELL && game->getFieldValue (row, column) != MINE && allowedClear == true && fieldStatus[row][column] != FLAGGED_CELL)
//    {
//        //buttonPushed->setFlat (true); //We're now flat
//        //        if(!so->isSolverRunning())
//        //        {
//        fieldStatus[row][column] = REVEALED_CELL;
//        //}

//        so->setFieldStatus(fieldStatus);
//        //Set the image according to the value of the cell
//        changeIcon(buttonPushed, row, column);

//        cellsRevealed++;

//        //If we have 90 cells revealed (10 mines, 90 not mines), we win the game!
//        if (cellsRevealed == ((fieldWidth*fieldHeight) - amountOfMines) && game->getFieldValue(row, column) != MINE)
//        {
//            changeIcon(buttonPushed, row, column);
//            won();
//            return;
//        }   // in case clearing occurs in the end of the game.

//        //Make sure that we are allowed to clear
//        if ( game->getFieldValue (row, column) == 0)
//            allowedClear = true;
//        else
//            allowedClear = false;

//        //Recrusively call clear on all adjacent cells
//        //Top left
//        if ( (row-1) != -1 && (column -1) != -1)
//            clear(row-1, column-1, allowedClear);
//        //Top center
//        if ( (row-1) != -1)
//            clear(row-1, column, allowedClear);
//        //Top right
//        if ( (row-1) != -1 && (column + 1) != fieldWidth)
//            clear(row-1, column+1, allowedClear);
//        //Left
//        if ( (column -1) != -1)
//            clear(row, column-1, allowedClear);
//        //Right
//        if ( (column + 1) != fieldWidth)
//            clear(row, column+1, allowedClear);
//        //Bottom left
//        if ( (row+1) != fieldHeight && (column -1) != -1)
//            clear(row+1, column-1, allowedClear);
//        //Bottom center
//        if ( (row+1) != fieldHeight)
//            clear(row+1, column, allowedClear);
//        //Bottom right
//        if ( (row+1) != fieldHeight && (column+1) != fieldWidth)
//        {
//            clear(row+1, column+1, allowedClear);
//        }
//    }
//    qApp->processEvents();
//}

/**
  * clear(int row, int column, bool allowedClear)
  * clear recursively reveals all zeros and reveals its neighbors that are not mines
  * @param int - The row to inspect
  * @param int - The column to inspect
  * @param bool - If we are allowed to clear the particular cell. This is used when recursively clearing something not a zero -- it is a neighbor of a zero so it is not allowed to clear others
  */ //TEEEEST
void MainWindow::clear(int row, int column, bool allowedClear, bool firstClearRequest)
{
    //Retrieve the button
    QString coordinates = QString::number(row) + "," + QString::number(column);

    MineSweeperButton *buttonPushed = qobject_cast<MineSweeperButton *>(signalMapperLeftClick->mapping(coordinates));


    //Ensure that the button is not flat, that the value is not a mine, that it is allowed to clear (not something that isn't a zero) and it isn't flagged
    if (firstClearRequest || (so->game->getFieldStatus(row, column) != REVEALED_CELL && so->game->getFieldValue (row, column) != MINE && allowedClear == true && so->game->getFieldStatus(row, column) != FLAGGED_CELL))
    {

        int result = 0;
        if(!firstClearRequest)
        {
            result =  so->game->revealCell(row, column);
        }

        //Set the image according to the value of the cell
        changeIcon(buttonPushed, row, column);

        //If we have 90 cells revealed (10 mines, 90 not mines), we win the game!
        if (result == 1)// in case clearing occurs in the end of the game.
        {
            won();
        }
        else
        {
            //Make sure that we are allowed to clear
            if ( so->game->getFieldValue (row, column) == 0)
                allowedClear = true;
            else
                allowedClear = false;

            //Recrusively call clear on all adjacent cells
            //Top left
            if ( (row-1) != -1 && (column -1) != -1)
                clear(row-1, column-1, allowedClear, false);
            //Top center
            if ( (row-1) != -1)
                clear(row-1, column, allowedClear, false);
            //Top right
            if ( (row-1) != -1 && (column + 1) != fieldWidth)
                clear(row-1, column+1, allowedClear, false);
            //Left
            if ( (column -1) != -1)
                clear(row, column-1, allowedClear, false);
            //Right
            if ( (column + 1) != fieldWidth)
                clear(row, column+1, allowedClear, false);
            //Bottom left
            if ( (row+1) != fieldHeight && (column -1) != -1)
                clear(row+1, column-1, allowedClear, false);
            //Bottom center
            if ( (row+1) != fieldHeight)
                clear(row+1, column, allowedClear, false);
            //Bottom right
            if ( (row+1) != fieldHeight && (column+1) != fieldWidth)
            {
                clear(row+1, column+1, allowedClear, false);
            }
        }
    }
    qApp->processEvents();
}

/**
  * lost()
  * Handles losing the game
  */
void MainWindow::lost() {
    //hasFinished = true;
    //so->setGameStatus(hasFinished);

    //Go through all the cells and reveal all the mines
    for ( int row = 0; row < fieldHeight; row++ )
    {
        for ( int column = 0; column < fieldWidth; column++ )
        {
            //Get the coordinates and obtain the button
            QString coordinates = QString::number(row)+","+QString::number(column);
            MineSweeperButton *button = qobject_cast<MineSweeperButton *>(signalMapperLeftClick->mapping(coordinates));

            //Check if it's a mine, if it is.. show it!
            if (so->game->getFieldStatus(row,column) != REVEALED_CELL && so->game->getFieldValue (row,column) == MINE )
            {
                so->game->revealCell(row,column);
                //so->setFieldStatus(fieldStatus);

                if(!so->isSolverRunning())
                {
                    //Are we flagged? Good job! you find a mine and flagged it :)
                    if (so->game->getFieldStatus(row,column) == FLAGGED_CELL ) {
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
        //qApp->processEvents();
    }
    solverControl(); //button wieder rücksetzen
    QMessageBox messageBox;
    messageBox.setFixedSize(400,100);
    messageBox.information(0, "Fail", "You loose :(");

}

/**
  * reset()
  * Resets the game board
  */
void MainWindow::reset() {
    //Reset global variables
    so->stopSolver();
    ui->solverStartButton->setText("Start Solver");
    //so->setGameStatus(hasFinished);
    so->resetSolver();

    flagsFlagged = 0;
    minesFlagged = 0;

    if( (( ui->fieldHeigth->text().toInt() * ui->fieldWidth->text().toInt() * 0.927) < ui->mineAmount->text().toInt() ))
    {

        QMessageBox messageBox;
        int mineAmount =  (ui->fieldHeigth->text().toInt() * ui->fieldWidth->text().toInt()) * 0.927;
        messageBox.information(0, "Eingabefehler", "Minenanzahl zu hoch. Maximal 92% des Feldes dürfen Minen sein. Neuer Wert: " + QString::number(mineAmount));
        messageBox.setFixedSize(600,300);
        ui->mineAmount->setValue(mineAmount);

        ui->newGame->setDown(false);
        ui->centralWidget->repaint();
    } else if(ui->fieldHeigth->text().toInt() == fieldHeight &&  ui->fieldWidth->text().toInt() == fieldWidth)
    {
        amountOfMines = ui->mineAmount->text().toInt();
        initMainWindow(true);
    }
    else
    {
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
        fieldHeight = ui->fieldHeigth->text().toInt();
        fieldWidth = ui->fieldWidth->text().toInt();
        amountOfMines = ui->mineAmount->text().toInt();

        initMainWindow(false);
    }
}

void MainWindow::showGamemenu()
{
    if(ui->widget->isVisible())
    {
        ui->widget_Menu->hide();
        ui->widget->hide();
        ui->button_GameOptions->setText("Show menu");
        ui->widget_Menu->show();
        refreshWindow();
    } else
    {
        ui->widget_Menu->hide();
        ui->widget->show();
        ui->button_GameOptions->setText("Hide menu");
        ui->widget_Menu->show();
        refreshWindow();
    }
}

void MainWindow::showColorLegend()
{
    if(ui->cbShowSolution->isChecked())
    {

        ui->widgetColorLegend->show();
        //refreshWindow();
    }
    else
    {
        ui->widgetColorLegend->hide();
        //refreshWindow();
    }
}

void MainWindow::setSolverMode()
{
    if(ui->radioButton_SolverAuto->isChecked())
    {
        ui->solverStartButton->setText("Start Solver");
        ui->labelDelayPerStep->show();
        ui->labelDelayPerStep_Sec->show();
        ui->spinBox_SolverDelaySeconds->show();
    }
    else if( ui->radioButton_SolverManual->isChecked())
    {
        ui->labelDelayPerStep->hide();
        ui->labelDelayPerStep_Sec->hide();
        ui->spinBox_SolverDelaySeconds->hide();
        ui->solverStartButton->setText("Best Move");
    }
}

void MainWindow::solverControl()
{
    if(ui->radioButton_SolverAuto->isChecked())
    {
        if(ui->solverStartButton->text() == "Start Solver" && !so->game->gameHasFinished())
        {
            ui->solverStartButton->setText("Stop Solver");
            ui->spinBox_SolverDelaySeconds->setEnabled(false);
            refreshWindow();

            int algorithmID;

            if(ui->radioButton_naiveSinglePoint->isChecked())
            {
                algorithmID = 1;
            }
            else if(ui->radioButton_doubleSetSinglePoint->isChecked())
            {
                algorithmID = 2;
            }
            else if(ui->radioButton_linear->isChecked())
            {
                algorithmID = 3;
            }
            so->setDelay(ui->spinBox_SolverDelaySeconds->text().toInt());
            so->startSolver(algorithmID);

        }
        else
        {
            ui->solverStartButton->setText("Start Solver");
            ui->spinBox_SolverDelaySeconds->setEnabled(true);
            so->stopSolver();
            refreshWindow();
        }
    }
    else if( ui->radioButton_SolverManual->isChecked())
    {
        QMessageBox messageBox;
        messageBox.information(0, "test1", "agagtegg");
    }
}

void MainWindow::showSolution()
{

}

void MainWindow::showPerformanceAnalysisWindow()
{
    PerformanceAnalysis *pf = new PerformanceAnalysis();
    pf->show();
}

void MainWindow::showHelpWindow()
{
    HelpWindow* helpWindow = new HelpWindow();
    helpWindow->show();

}

void MainWindow::showAboutWindow()
{
    AboutWindow* aboutWindow = new AboutWindow();
    aboutWindow->show();
}

/**
  * won()
  * Handles winning the game
  */
void MainWindow::won()
{
    //hasFinished = true;
    //so->setGameStatus(hasFinished);

    ui->lcdFlagCount->display (0);

    //Set all the mines to disarmed
    for ( int row = 0; row < fieldHeight; row++ )
    {
        for ( int column = 0; column < fieldWidth; column++ )
        {
            QString coordinates = QString::number(row)+","+QString::number(column);
            MineSweeperButton *button = qobject_cast<MineSweeperButton *>(signalMapperLeftClick->mapping(coordinates));
            if (so->game->getFieldStatus(row, column) != REVEALED_CELL && so->game->getFieldValue(row, column) == MINE )
            {
                //button->setFlat (true);
                so->game->revealCell(row,column);
                //so->setFieldStatus(fieldStatus);

                button->setText(QString("💣"));
                button->setStyleSheet(buttonStyleFlatBlue + buttonFontSmall + "color: rgb(0,0,0);");
                qApp->processEvents();
            }
        }
    }
    solverControl();
    QMessageBox messageBox;
    messageBox.setFixedSize(400,100);
    messageBox.information(0, "Contratulations", "You win :)");
}

void MainWindow::setButtonTooltip(int row, int column, float probability)
{
    QString coordinates = QString::number(row)+","+QString::number(column);
    MineSweeperButton *button = qobject_cast<MineSweeperButton *>(signalMapperLeftClick->mapping(coordinates));

    if(!(so->game->getFieldStatus(row,column) == REVEALED_CELL))
    {
        button->setToolTip("Wahrscheinlichkeit für Mine: " + QString::number(probability, 'f', 2) + "%");

    } else
    {
        button->setToolTip("");
    }
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
