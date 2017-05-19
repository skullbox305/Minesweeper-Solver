#include "performanceanalysis.h"
#include "ui_performanceanalysis.h"

PerformanceAnalysis::PerformanceAnalysis(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PerformanceAnalysis)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    connect(ui->button_StartPerformanceTest, SIGNAL(released()), this, SLOT(startPerformanceAnalysis()));
    connect(ui->button_Back, SIGNAL(released()), this, SLOT(handleBackButton()));
}

PerformanceAnalysis::~PerformanceAnalysis()
{
    delete ui;
}

void PerformanceAnalysis::startPerformanceAnalysis()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void PerformanceAnalysis::handleBackButton()
{
    ui->stackedWidget->setCurrentIndex(0);
}
