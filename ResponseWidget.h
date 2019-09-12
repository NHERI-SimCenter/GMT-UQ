#ifndef RESPONSEWIDGET_H
#define RESPONSEWIDGET_H

#include <QWidget>
#include <QVector>

class QCustomPlot;
class QLineEdit;
class QCPGraph;
class QCPCurve;
class QCPItemTracer;
class MainWindow;
class QSpinBox;


//
// a widget to plot the response,
//  NOTE to cut down on memory currently the widget does not keep data, requires a call back to main window
//  THIS is proving problematic to say the least! .. will redo
//

class ResponseWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ResponseWidget(QString &xAxis, QString &yAxis, QWidget *parent = 0);
    ~ResponseWidget();

    void clear();
    void addData(QVector<double> &data, QVector<double> &time, int numSteps, double dt);
    void addData(QVector<double> &data, QVector<double> &x);

signals:

public slots:



private:
    QCustomPlot *thePlot;

    QCPGraph *graph;
    QCPItemTracer *groupTracer;
    QCPCurve *curve;
    int numGraphs;
    double minValue;
    double maxValue;
    double xMinValue;
    double xMaxValue;
};

#endif // NODERESPONSEWIDGET_H
