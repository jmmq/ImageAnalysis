#include "imagewidget.h"
#include<QPainter>
ImageWidget::ImageWidget(QImage *image,QWidget *parent):QWidget(parent),image(image)
{
    imageMemory.append(image);
    resize(this->image->width(),this->image->height());
    update();
}
void ImageWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawImage(0,0,*image);
}

QImage *ImageWidget::getImage() const
{
    return image;
}

void ImageWidget::updateImage(QImage *newImage)
{
    imageMemory.append(newImage);
    image=newImage;
    update();
}
