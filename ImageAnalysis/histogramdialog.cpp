#include "histogramdialog.h"
#include"histogramwidget.h"
#include<QVBoxLayout>
HistogramDialog::HistogramDialog(QImage *image, QWidget *parent):QDialog(parent)
{
    QVBoxLayout *mlt=new QVBoxLayout();
    HistogramWidget *hw=new HistogramWidget(image);
    mlt->addWidget(hw);
    setLayout(mlt);
    setMinimumSize(800,600);
    adjustSize();
}
