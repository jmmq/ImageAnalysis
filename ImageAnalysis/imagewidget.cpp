#include "imagewidget.h"
#include<QPainter>
ImageWidget::ImageWidget(QImage *image,QWidget *parent)
    :QWidget(parent),image(image),memoryIndex(0)
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
    while(memoryIndex<(imageMemory.size()-1)){
        QImage *lastImage=imageMemory.takeLast();
        delete lastImage;
    }
    imageMemory.append(newImage);
    image=newImage;
    memoryIndex++;
    update();
}

bool ImageWidget::undo()
{
    if(memoryIndex>0){
        memoryIndex--;
        image=imageMemory.at(memoryIndex);
        update();
        return true;
    }
    return false;
}

bool ImageWidget::redo()
{
    if(memoryIndex<(imageMemory.size()-1)){
        memoryIndex++;
        image=imageMemory.at(memoryIndex);
        update();
        return true;
    }
    return false;
}
