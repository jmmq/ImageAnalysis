#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H
#include<QWidget>
#include<QImage>
#include<QList>
class ImageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ImageWidget(QImage *image,QWidget *parent = nullptr);
    QImage *getImage() const;
    void updateImage(QImage *newImage);
protected:
    void paintEvent(QPaintEvent *event);
private:
    QList<QImage *> imageMemory;
    QImage *image;
signals:
public slots:
};

#endif // IMAGEWIDGET_H
