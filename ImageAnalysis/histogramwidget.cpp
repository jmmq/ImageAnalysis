#include "histogramwidget.h"
#include<QVBoxLayout>
#include<QValueAxis>
#include<QLineSeries>
#include<QScatterSeries>
#include<QChart>
#include<QChartView>
#include<QLabel>
QT_CHARTS_USE_NAMESPACE
HistogramWidget::HistogramWidget(QString name, QImage *image, QWidget *parent)
    : QWidget(parent),image(image)
{
    setWindowTitle("Histograma: "+name);
    QChart *chart = new QChart();
    chart->setTitle("");
    chart->legend()->hide();
    HistogramInfo *info=new HistogramInfo(image);
    //QScatterSeries *greySeries=new QScatterSeries;
    QLineSeries *greySeries=new QLineSeries;
    for(int i=0;i<256;i++){
        greySeries->append(i,info->grayHistogramList->at(i));
    }
    greySeries->setColor(QColor(16,16,16));
    greySeries->setName("Valores de gris");
    //greySeries->setMarkerShape(QScatterSeries::MarkerShapeRectangle);
    //greySeries->setMarkerSize(4);
    chart->addSeries(greySeries);
    /*QLineSeries *redSeries=new QLineSeries();
    for(int i=0;i<256;i++){
        redSeries->append(i,info->redChannel->at(i));
    }
    redSeries->setPen(QPen(QBrush(QColor(192,0,0)),1));
    chart->addSeries(redSeries);
    QLineSeries *greenSeries=new QLineSeries();
    for(int i=0;i<256;i++){
        greenSeries->append(i,info->greenChannel->at(i));
    }
    greenSeries->setPen(QPen(QBrush(QColor(0,192,0)),1));
    chart->addSeries(greenSeries);
    QLineSeries *blueSeries=new QLineSeries();
    for(int i=0;i<256;i++){
        blueSeries->append(i,info->blueChannel->at(i));
    }
    blueSeries->setPen(QPen(QBrush(QColor(0,0,192)),1));
    chart->addSeries(blueSeries);*/
    int maxY=0;
    for(int i=0;i<256;i++){
        if(info->grayHistogramList->at(i)>maxY){
            maxY=info->grayHistogramList->at(i);
        }/*
        if(info->redChannel->at(i)>maxY){
            maxY=info->redChannel->at(i);
        }
        if(info->greenChannel->at(i)>maxY){
            maxY=info->greenChannel->at(i);
        }
        if(info->blueChannel->at(i)>maxY){
            maxY=info->blueChannel->at(i);
        }*/
    }
    QValueAxis *axisX = new QValueAxis;
    axisX->setRange(0,255);
    axisX->setMinorTickCount(15);
    axisX->setLabelFormat("%d");
    chart->setAxisX(axisX);

    QValueAxis *axisY = new QValueAxis;
    axisY->setRange(0,maxY);
    axisY->setMinorTickCount(15);
    axisY->setLabelFormat("%d");
    chart->setAxisY(axisY);
    //chart->createDefaultAxes();

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(chartView);
    QLabel *meanLabel=new QLabel("Media: "+QString::number(info->grayMean));
    QLabel *varianceLabel=new QLabel("Varianza: "+QString::number(info->grayVariance));
    QLabel *asymmetryLabel=new QLabel("Asimetría: "+QString::number(info->grayAsymmetry));
    QLabel *energyLabel=new QLabel("Energía: "+QString::number(info->grayEnergy));
    mainLayout->addWidget(meanLabel);
    mainLayout->addWidget(varianceLabel);
    mainLayout->addWidget(asymmetryLabel);
    mainLayout->addWidget(energyLabel);
    setLayout(mainLayout);
    resize(600,400);
}
