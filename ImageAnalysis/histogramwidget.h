#ifndef HISTOGRAMWIDGET_H
#define HISTOGRAMWIDGET_H

#include <QWidget>
#include"histograminfo.h"
class HistogramWidget : public QWidget
{
    Q_OBJECT
private:
    QImage *image;
public:
    explicit HistogramWidget(QString name,QImage *image,QWidget *parent = nullptr);
signals:
public slots:
};
#endif // HISTOGRAMWIDGET_H
