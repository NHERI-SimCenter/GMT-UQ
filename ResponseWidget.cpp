#include "ResponseWidget.h"
#include <QLineEdit>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <MainWindow.h>
#include <QSpinBox>

#include <qcustomplot.h>


ResponseWidget::ResponseWidget(
                               QString &xLabel,
                               QString &yLabel,
                               QWidget *parent)
    : QWidget(parent), numGraphs(0)
{
    minValue = 0;
    maxValue = 0;
    xMinValue = 0;
    xMaxValue = 0;
    // create a main layout
    QVBoxLayout *mainLayout = new QVBoxLayout();


    //connect(saveData, SIGNAL(pressed()), mainWindow, SLOT(saveData()));


    //
    // graphic window
    //

    thePlot=new QCustomPlot();
    thePlot->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
    thePlot->setInteractions(QCP::iSelectPlottables);

    QRect rec = QApplication::desktop()->screenGeometry();

    int height = 0.2*rec.height();
    int width = 0.5*rec.width();

    thePlot->setMinimumWidth(width);
    thePlot->setMinimumHeight(height);
    mainLayout->addWidget(thePlot);

    thePlot->xAxis->setLabel(xLabel);
    thePlot->yAxis->setLabel(yLabel);

    this->setLayout(mainLayout);
}

ResponseWidget::~ResponseWidget()
{

}

void
ResponseWidget::clear() {
   thePlot->clearGraphs();
    thePlot->clearPlottables();
    numGraphs=0;
    minValue = 0;
    maxValue = 0;
    xMinValue = 0;
    xMaxValue = 0;
}



void
ResponseWidget::addData(QVector<double> &data, QVector<double> &time, int numSteps, double dt) {

    if (time.size() != data.size()) {
        qDebug() << "ResponseWidget - setData vectors of differing sizes";
        return;
    }

    if (time.size() != numSteps) {
        qDebug() << "ResponseWidget - setData vector and step size do not agree";
        return;
    }

    graph = thePlot->addGraph();
    thePlot->graph(numGraphs)->setData(time, data);
    numGraphs++;


    for (int i=0; i<numSteps; i++) {
        double value = data.at(i);
        if (value < minValue)
            minValue = value;
        if (value > maxValue)
            maxValue = value;
    }

    thePlot->xAxis->setRange(0, numSteps*dt);
    thePlot->yAxis->setRange(minValue, maxValue);
    //thePlot->axisRect()->setAutoMargins(QCP::msNone);
    thePlot->axisRect()->setMargins(QMargins(0,0,0,0));
    thePlot->replot();

}

void
ResponseWidget::addData(QVector<double> &data, QVector<double> &x) {

    qDebug() << data.size();
    qDebug() << x.size();
    qDebug() << data;
    qDebug() << x;


    /*
     *
     * thePlot->clearGraphs();
    thePlot->clearPlottables();
    curve = new QCPCurve(thePlot->xAxis, thePlot->yAxis);

    curve->setData(x,data);
*/
    graph = thePlot->addGraph();
    thePlot->graph(numGraphs)->setData(x, data, true);
    numGraphs++;

    int numSteps = data.size();

    for (int i=0; i<numSteps; i++) {
        double value = data.at(i);
        double xValue = x.at(i);
        if (value < minValue)
            minValue = value;
        if (value > maxValue)
            maxValue = value;
        if (xValue < xMinValue)
            xMinValue = xValue;
        if (xValue > xMaxValue)
            xMaxValue = xValue;
    }

   thePlot->yAxis->setRange(minValue, maxValue);
   thePlot->xAxis->setRange(xMinValue, xMaxValue);

    thePlot->replot();
}

