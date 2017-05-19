#ifndef PERFORMANCEANALYSIS_H
#define PERFORMANCEANALYSIS_H

#include <QWidget>

namespace Ui {
class PerformanceAnalysis;
}

class PerformanceAnalysis : public QWidget
{
    Q_OBJECT

public:
    explicit PerformanceAnalysis(QWidget *parent = 0);
    ~PerformanceAnalysis();

private:
    Ui::PerformanceAnalysis *ui;

private slots:
    void startPerformanceAnalysis();
    void handleBackButton();
};

#endif // PERFORMANCEANALYSIS_H
