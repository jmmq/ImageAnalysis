#ifndef HISTOGRAMDIALOG_H
#define HISTOGRAMDIALOG_H

#include <QDialog>

class HistogramDialog : public QDialog
{
    Q_OBJECT
public:
    HistogramDialog(QImage *image,QWidget *parent = Q_NULLPTR);
};

#endif // HISTOGRAMDIALOG_H
