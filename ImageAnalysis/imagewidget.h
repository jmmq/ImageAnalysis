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
    bool undo();
    bool redo();
protected:
    void paintEvent(QPaintEvent *event);
private:
    int memoryIndex;
    QList<QImage *> imageMemory;
    QImage *image;
signals:
public slots:
};

#endif // IMAGEWIDGET_H
