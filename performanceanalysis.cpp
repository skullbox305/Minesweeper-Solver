#include "performanceanalysis.h"
#include "ui_performanceanalysis.h"
#include <QTime>

PerformanceAnalysis::PerformanceAnalysis(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PerformanceAnalysis)
{    

    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    connect(ui->button_StartPerformanceTest, SIGNAL(released()), this, SLOT(startPerformanceAnalysis()));
    connect(ui->button_Back, SIGNAL(released()), this, SLOT(handleBackButton()));
    so = new Solver(this);
    so->setWithoutButtons();

}

PerformanceAnalysis::~PerformanceAnalysis()
{
    delete ui;
}

void PerformanceAnalysis::startPerformanceAnalysis()
{
    gamesLost = 0;
    gamesWon = 0;
    winRate = 0;

    fieldHeight = ui->spinBox_fieldHeight->text().toInt();
    fieldWidth = ui->spinBox_fieldWidth->text().toInt();
    amountOfMines = ui->spinBox_mineAmount->text().toInt();
    amountOfGames = ui->spinBox_amountOfTestgames->text().toInt();
    qsrand(QTime::currentTime().msec());

    so->game = new Minesweeper(fieldWidth, fieldHeight, amountOfMines);


    int algorithmID = 1;

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
        return;
    }

    ui->progressBar->setMaximum(amountOfGames - 1);
    ui->progressBar->setMinimum(0);
    ui->stackedWidget->setCurrentIndex(1);

    for(int i = 0; i < amountOfGames; i++)
    {
        so->stopSolver();
        so->resetSolver();
        so->game->initGame();
        so->game->generateBoard(true, 1, 1);
        so->game->startGame();
        so->startSolver(algorithmID);

        do
        {
            if(so->game->hasWon())
            {
                gamesWon++;
            }
            else if(so->game->hasLost())
            {
                gamesLost++;
            }
        } while(!so->game->gameHasFinished());


        ui->progressBar->setValue(i);
        //QThread::msleep(1);
        qApp->processEvents();
    }
    ui->stackedWidget->setCurrentIndex(2);
    ui->label_gamesWon->setText(QString::number(gamesWon));
    ui->label_gamesLost->setText(QString::number(gamesLost));
    winRate = ( (float)gamesWon * 100.0 ) / (float)amountOfGames;
    ui->label_winRate->setText(QString::number(winRate) + " %");
}

void PerformanceAnalysis::handleBackButton()
{
    ui->stackedWidget->setCurrentIndex(0);
    delete so->game;
    delete so;
    so = new Solver(this);
    so->setWithoutButtons();

}
