#include "imagefunctions.h"
#include<QImage>
#include<QtMath>
QImage *ImageFunctions::toGrayScale(QImage *image,bool copy)
{
    QImage *newImage;
    if(copy){
        newImage=new QImage(*image);
    }else{
        newImage=image;
    }
    QColor oldColor;
    QColor newColor;
    int mean;
    for(int y=0;y<newImage->height();y++){
        QRgb *line=(QRgb *)newImage->scanLine(y);
        for(int x=0;x<newImage->width();x++){
            oldColor.setRgb(line[x]);
            mean=(oldColor.red()+oldColor.green()+oldColor.blue())/3;
            newColor.setRgb(mean,mean,mean);
            line[x]=newColor.rgb();
        }
    }
    return newImage;
}
QImage *ImageFunctions::binarize(QImage *image, bool copy, int threshold, bool inverse)
{
    QImage *newImage;
    if(copy){
        newImage=new QImage(*image);
    }else{
        newImage=image;
    }
    QColor oldColor;
    QColor newColor;
    int mean;
    for(int y=0;y<newImage->height();y++){
        QRgb *line=(QRgb *)newImage->scanLine(y);
        for(int x=0;x<newImage->width();x++){
            oldColor.setRgb(line[x]);
            mean=(oldColor.red()+oldColor.green()+oldColor.blue())/3;
            if(inverse){
                if(mean<=threshold){
                    newColor.setRgb(255,255,255);
                }else{
                    newColor.setRgb(0,0,0);
                }
            }else{
                if(mean>threshold){
                    newColor.setRgb(255,255,255);
                }else{
                    newColor.setRgb(0,0,0);
                }
            }
            line[x]=newColor.rgb();
        }
    }
    return newImage;
}
QImage *ImageFunctions::binarizeChannel(QImage *image, bool copy,int threshold, bool inverse, ImageFunctions::Channel channel)
{
    QImage *newImage;
     if(copy){
         newImage=new QImage(*image);
     }else{
         newImage=image;
     }
    QColor newColor;
    int channelValue;
    for(int y=0;y<newImage->height();y++){
        QRgb *line=(QRgb *)newImage->scanLine(y);
        for(int x=0;x<newImage->width();x++){
            newColor.setRgb(line[x]);
            if(channel==Red)
                channelValue=qRed(line[x]);
            if(channel==Green)
                channelValue=qGreen(line[x]);
            if(channel==Blue)
                channelValue=qBlue(line[x]);
            if(inverse){
                if(channelValue<=threshold){
                    if(channel==Red)
                        newColor.setRed(255);
                    if(channel==Green)
                        newColor.setGreen(255);
                    if(channel==Blue)
                        newColor.setBlue(255);
                }else{
                    if(channel==Red)
                        newColor.setRed(0);
                    if(channel==Green)
                        newColor.setGreen(0);
                    if(channel==Blue)
                        newColor.setBlue(0);
                }
            }else{
                if(channelValue>threshold){
                    if(channel==Red)
                        newColor.setRed(255);
                    if(channel==Green)
                        newColor.setGreen(255);
                    if(channel==Blue)
                        newColor.setBlue(255);
                }else{
                    if(channel==Red)
                        newColor.setRed(0);
                    if(channel==Green)
                        newColor.setGreen(0);
                    if(channel==Blue)
                        newColor.setBlue(0);
                }
            }
            line[x]=newColor.rgb();
        }
    }
     return newImage;
}
QImage *ImageFunctions::openChannel(QImage *image, bool copy, ImageFunctions::Channel channel)
{
    QImage *newImage;
    if(copy){
     newImage=new QImage(*image);
    }else{
     newImage=image;
    }
    QColor oldColor;
    QColor newColor;
    for(int y=0;y<newImage->height();y++){
        QRgb *line=(QRgb *)newImage->scanLine(y);
        for(int x=0;x<newImage->width();x++){
            oldColor.setRgb(line[x]);
            newColor.setRgb(0,0,0);
            if(channel==Red)
                newColor.setRed(oldColor.red());
            if(channel==Green)
                newColor.setGreen(oldColor.green());
            if(channel==Blue)
                newColor.setBlue(oldColor.blue());
            line[x]=newColor.rgb();
        }
    }
    return newImage;
}
QImage *ImageFunctions::addition(QImage *imageA, QImage *imageB, bool copy)
{
    QImage *newImage;
    if(copy){
     newImage=new QImage(*imageA);
    }else{
     newImage=imageA;
    }
    QImage imageBScaled=imageB->scaled(newImage->size());
    int red,green,blue;
    for(int y=0;y<newImage->height();y++){
        QRgb *imageBLine=(QRgb *)imageBScaled.scanLine(y);
        QRgb *newImageLine=(QRgb *)newImage->scanLine(y);
        for(int x=0;x<newImage->width();x++){
            red=(qRed(newImageLine[x])+qRed(imageBLine[x]))/2;
            green=(qGreen(newImageLine[x])+qGreen(imageBLine[x]))/2;
            blue=(qBlue(newImageLine[x])+qBlue(imageBLine[x]))/2;
            newImageLine[x]=qRgb(red,green,blue);
        }
    }
    return newImage;
}
QImage *ImageFunctions::subtraction(QImage *imageA, QImage *imageB, bool copy)
{
    QImage *newImage;
    if(copy){
     newImage=new QImage(*imageA);
    }else{
     newImage=imageA;
    }
    QImage imageBScaled=imageB->scaled(newImage->size());
    int red,green,blue;
    for(int y=0;y<newImage->height();y++){
        QRgb *imageBLine=(QRgb *)imageBScaled.scanLine(y);
        QRgb *newImageLine=(QRgb *)newImage->scanLine(y);
        for(int x=0;x<newImage->width();x++){
            red=qAbs(qRed(newImageLine[x])-qRed(imageBLine[x]))%256;
            green=qAbs(qGreen(newImageLine[x])-qGreen(imageBLine[x]))%256;
            blue=qAbs(qBlue(newImageLine[x])-qBlue(imageBLine[x]))%256;
            newImageLine[x]=qRgb(red,green,blue);
        }
    }
    return newImage;
}
QImage *ImageFunctions::multiplication(QImage *imageA, QImage *imageB, bool copy)
{
    QImage *newImage;
    if(copy){
     newImage=new QImage(*imageA);
    }else{
     newImage=imageA;
    }
    QImage imageBScaled=imageB->scaled(newImage->size());
    int red,green,blue;
    for(int y=0;y<newImage->height();y++){
        QRgb *imageBLine=(QRgb *)imageBScaled.scanLine(y);
        QRgb *newImageLine=(QRgb *)newImage->scanLine(y);
        for(int x=0;x<newImage->width();x++){
            red=(qRed(newImageLine[x])*qRed(imageBLine[x]))/255;
            green=(qGreen(newImageLine[x])*qGreen(imageBLine[x]))/255;
            blue=(qBlue(newImageLine[x])*qBlue(imageBLine[x]))/255;
            newImageLine[x]=qRgb(red,green,blue);
        }
    }
    return newImage;
}
QImage *ImageFunctions::logicNot(QImage *image, bool copy)
{
    QImage *newImage;
    if(copy){
     newImage=new QImage(*image);
    }else{
     newImage=image;
    }
    for(int y=0;y<newImage->height();y++){
        QRgb *line=(QRgb *)newImage->scanLine(y);
        for(int x=0;x<newImage->width();x++){
            line[x]=~line[x];
        }
    }
    return newImage;
}
QImage *ImageFunctions::logicAND(QImage *imageA, QImage *imageB, bool copy)
{
    QImage *newImage;
    if(copy){
     newImage=new QImage(*imageA);
    }else{
     newImage=imageA;
    }
    QImage imageBScaled=imageB->scaled(newImage->size());
    for(int y=0;y<newImage->height();y++){
        QRgb *imageBLine=(QRgb *)imageBScaled.scanLine(y);
        QRgb *newImageLine=(QRgb *)newImage->scanLine(y);
        for(int x=0;x<newImage->width();x++){
            newImageLine[x]&=imageBLine[x];
        }
    }
    return newImage;
}
QImage *ImageFunctions::logicOR(QImage *imageA, QImage *imageB, bool copy)
{
    QImage *newImage;
    if(copy){
     newImage=new QImage(*imageA);
    }else{
     newImage=imageA;
    }
    QImage imageBScaled=imageB->scaled(newImage->size());
    for(int y=0;y<newImage->height();y++){
        QRgb *imageBLine=(QRgb *)imageBScaled.scanLine(y);
        QRgb *newImageLine=(QRgb *)newImage->scanLine(y);
        for(int x=0;x<newImage->width();x++){
            newImageLine[x]|=imageBLine[x];
        }
    }
    return newImage;
}
QImage *ImageFunctions::logicXOR(QImage *imageA, QImage *imageB, bool copy)
{
    QImage *newImage;
    if(copy){
     newImage=new QImage(*imageA);
    }else{
     newImage=imageA;
    }
    QImage imageBScaled=imageB->scaled(newImage->size());
    for(int y=0;y<newImage->height();y++){
        QRgb *imageBLine=(QRgb *)imageBScaled.scanLine(y);
        QRgb *newImageLine=(QRgb *)newImage->scanLine(y);
        for(int x=0;x<newImage->width();x++){
            newImageLine[x]^=imageBLine[x];
        }
    }
    return newImage;
}
QList<int> *ImageFunctions::grayHistogramList(QImage *image)
{
    QList<int> *list=new QList<int>();
    for(int i=0;i<256;i++){
        list->append(0);
    }
    int mean;
    for(int y=0;y<image->height();y++){
        QRgb *line=(QRgb *)image->scanLine(y);
        for(int x=0;x<image->width();x++){
            mean=(qRed(line[x])+qGreen(line[x])+qBlue(qBlue(line[x])))/3;
            (*list)[mean]+=1;
        }
    }
    return list;
}
QList<int> *ImageFunctions::redHistogramList(QImage *image)
{
    QList<int> *list=new QList<int>();
    for(int i=0;i<256;i++){
        list->append(0);
    }
    for(int y=0;y<image->height();y++){
        QRgb *line=(QRgb *)image->scanLine(y);
        for(int x=0;x<image->width();x++){
            (*list)[qRed(line[x])]+=1;
        }
    }
    return list;
}
QList<int> *ImageFunctions::greenHistogramList(QImage *image)
{
    QList<int> *list=new QList<int>();
    for(int i=0;i<256;i++){
        list->append(0);
    }
    for(int y=0;y<image->height();y++){
        QRgb *line=(QRgb *)image->scanLine(y);
        for(int x=0;x<image->width();x++){
            (*list)[qGreen(line[x])]+=1;
        }
    }
    return list;
}
QList<int> *ImageFunctions::blueHistogramList(QImage *image)
{
    QList<int> *list=new QList<int>();
    for(int i=0;i<256;i++){
        list->append(0);
    }
    for(int y=0;y<image->height();y++){
        QRgb *line=(QRgb *)image->scanLine(y);
        for(int x=0;x<image->width();x++){
            (*list)[qBlue(line[x])]+=1;
        }
    }
    return list;
}
double ImageFunctions::grayMean(QImage *image, QList<int> *histogramList)
{
    double mean=0;
    for(int i=0;i<256;i++){
        mean+=i*(histogramList->at(i));
    }
    return mean/(image->height()*image->width());
}
double ImageFunctions::grayVariance(QImage *image, QList<int> *histogramList, double mean)
{
    double variance=0;
    for(int i=0;i<256;i++){
        variance+=(i-mean)*(i-mean)*(histogramList->at(i));
    }
    return variance/(image->height()*image->width());
}
double ImageFunctions::grayEnergy(QImage *image, QList<int> *histogramList)
{
    double energy=0;
    for(int i=0;i<256;i++){
        energy+=(histogramList->at(i))*(histogramList->at(i))/(image->height()*image->width());
    }
    return energy/(image->height()*image->width());
}
double ImageFunctions::grayAsymmetry(QImage *image, QList<int> *histogramList, double mean)
{
    double asymmetry=0;
    for(int i=0;i<256;i++){
        asymmetry+=(i-mean)*(i-mean)*(i-mean)*(histogramList->at(i));
    }
    return asymmetry/(image->height()*image->width());
}
QImage *ImageFunctions::meanFilter(QImage *image, bool copy)
{
    int divisor=9;
    int mask[3][3]={
        {1,1,1},
        {1,1,1},
        {1,1,1}
    };
    QImage *newImage;
    if(copy){
        newImage=new QImage(*image);
    }else{
        newImage=image;
    }
    applyMask(newImage,mask,divisor);
    return newImage;
}
QImage *ImageFunctions::gaussianFilter(QImage *image, bool copy)
{
    int divisor=16;
    int mask[3][3]={
        {1,2,1},
        {2,4,2},
        {1,2,1}
    };
    QImage *newImage;
    if(copy){
        newImage=new QImage(*image);
    }else{
        newImage=image;
    }
    applyMask(newImage,mask,divisor);
    return newImage;
}
QImage *ImageFunctions::applyMask(QImage *image, int mask[3][3], int divisor)
{
    QImage *newImage=new QImage(*image);
    QRgb *newImageData=(QRgb*)newImage->bits();
    QRgb *imageData=(QRgb*)image->bits();
    int ys[3],xs[3];
    for(int y=0;y<image->height();y++){
        ys[0]=newImage->width()*qMax(y-1,0);
        ys[1]=newImage->width()*y;
        ys[2]=newImage->width()*qMin(y+1,image->height()-1);
        for(int x=0;x<image->width();x++){
            xs[0]=qMax(x-1,0);
            xs[1]=x;
            xs[2]=qMin(x+1,image->width()-1);
            imageData[x+newImage->width()*y]=
                    qRgb(
                        (
                        (
                        mask[0][0]*qRed(newImageData[ys[0]+xs[0]])+
                        mask[0][1]*qRed(newImageData[ys[0]+xs[1]])+
                        mask[0][2]*qRed(newImageData[ys[0]+xs[2]])+
                        mask[1][0]*qRed(newImageData[ys[1]+xs[0]])+
                        mask[1][1]*qRed(newImageData[ys[1]+xs[1]])+
                        mask[1][2]*qRed(newImageData[ys[1]+xs[2]])+
                        mask[2][0]*qRed(newImageData[ys[2]+xs[0]])+
                        mask[2][1]*qRed(newImageData[ys[2]+xs[1]])+
                        mask[2][2]*qRed(newImageData[ys[2]+xs[2]])
                        )/divisor
                        )%256
                        ,
                        (
                        (
                        mask[0][0]*qGreen(newImageData[ys[0]+xs[0]])+
                        mask[0][1]*qGreen(newImageData[ys[0]+xs[1]])+
                        mask[0][2]*qGreen(newImageData[ys[0]+xs[2]])+
                        mask[1][0]*qGreen(newImageData[ys[1]+xs[0]])+
                        mask[1][1]*qGreen(newImageData[ys[1]+xs[1]])+
                        mask[1][2]*qGreen(newImageData[ys[1]+xs[2]])+
                        mask[2][0]*qGreen(newImageData[ys[2]+xs[0]])+
                        mask[2][1]*qGreen(newImageData[ys[2]+xs[1]])+
                        mask[2][2]*qGreen(newImageData[ys[2]+xs[2]])
                        )/divisor
                        )%256
                        ,
                        (
                        (
                        mask[0][0]*qBlue(newImageData[ys[0]+xs[0]])+
                        mask[0][1]*qBlue(newImageData[ys[0]+xs[1]])+
                        mask[0][2]*qBlue(newImageData[ys[0]+xs[2]])+
                        mask[1][0]*qBlue(newImageData[ys[1]+xs[0]])+
                        mask[1][1]*qBlue(newImageData[ys[1]+xs[1]])+
                        mask[1][2]*qBlue(newImageData[ys[1]+xs[2]])+
                        mask[2][0]*qBlue(newImageData[ys[2]+xs[0]])+
                        mask[2][1]*qBlue(newImageData[ys[2]+xs[1]])+
                        mask[2][2]*qBlue(newImageData[ys[2]+xs[2]])
                        )/divisor
                        )%256
                    );
        }
    }
    delete newImage;
    return image;
}
QImage *ImageFunctions::highPassAlgorithm(QImage *image, int maskX[3][3], int maskY[3][3], int divisor)
{
    QImage *newImage=new QImage(*image);
    QRgb *newImageData=(QRgb*)newImage->bits();
    QRgb *imageData=(QRgb*)image->bits();
    int ys[3],xs[3];
    for(int y=0;y<image->height();y++){
        ys[0]=newImage->width()*qMax(y-1,0);
        ys[1]=newImage->width()*y;
        ys[2]=newImage->width()*qMin(y+1,image->height()-1);
        for(int x=0;x<image->width();x++){
            xs[0]=qMax(x-1,0);
            xs[1]=x;
            xs[2]=qMin(x+1,image->width()-1);
            imageData[x+newImage->width()*y]=
                    qRgb(
                        qMin((int)qSqrt(
                    qPow(
                        maskX[0][0]*qRed(newImageData[ys[0]+xs[0]])+
                        maskX[0][1]*qRed(newImageData[ys[0]+xs[1]])+
                        maskX[0][2]*qRed(newImageData[ys[0]+xs[2]])+
                        maskX[1][0]*qRed(newImageData[ys[1]+xs[0]])+
                        maskX[1][1]*qRed(newImageData[ys[1]+xs[1]])+
                        maskX[1][2]*qRed(newImageData[ys[1]+xs[2]])+
                        maskX[2][0]*qRed(newImageData[ys[2]+xs[0]])+
                        maskX[2][1]*qRed(newImageData[ys[2]+xs[1]])+
                        maskX[2][2]*qRed(newImageData[ys[2]+xs[2]])
                    ,2)/(divisor*divisor)
                    +
                    qPow(
                        maskY[0][0]*qRed(newImageData[ys[0]+xs[0]])+
                        maskY[0][1]*qRed(newImageData[ys[0]+xs[1]])+
                        maskY[0][2]*qRed(newImageData[ys[0]+xs[2]])+
                        maskY[1][0]*qRed(newImageData[ys[1]+xs[0]])+
                        maskY[1][1]*qRed(newImageData[ys[1]+xs[1]])+
                        maskY[1][2]*qRed(newImageData[ys[1]+xs[2]])+
                        maskY[2][0]*qRed(newImageData[ys[2]+xs[0]])+
                        maskY[2][1]*qRed(newImageData[ys[2]+xs[1]])+
                        maskY[2][2]*qRed(newImageData[ys[2]+xs[2]])
                    ,2)/(divisor*divisor)
                    ),255)
                        ,
                        qMin((int)qSqrt(
                    qPow(
                        maskX[0][0]*qGreen(newImageData[ys[0]+xs[0]])+
                        maskX[0][1]*qGreen(newImageData[ys[0]+xs[1]])+
                        maskX[0][2]*qGreen(newImageData[ys[0]+xs[2]])+
                        maskX[1][0]*qGreen(newImageData[ys[1]+xs[0]])+
                        maskX[1][1]*qGreen(newImageData[ys[1]+xs[1]])+
                        maskX[1][2]*qGreen(newImageData[ys[1]+xs[2]])+
                        maskX[2][0]*qGreen(newImageData[ys[2]+xs[0]])+
                        maskX[2][1]*qGreen(newImageData[ys[2]+xs[1]])+
                        maskX[2][2]*qGreen(newImageData[ys[2]+xs[2]])
                    ,2)/(divisor*divisor)
                    +
                    qPow(
                        maskY[0][0]*qGreen(newImageData[ys[0]+xs[0]])+
                        maskY[0][1]*qGreen(newImageData[ys[0]+xs[1]])+
                        maskY[0][2]*qGreen(newImageData[ys[0]+xs[2]])+
                        maskY[1][0]*qGreen(newImageData[ys[1]+xs[0]])+
                        maskY[1][1]*qGreen(newImageData[ys[1]+xs[1]])+
                        maskY[1][2]*qGreen(newImageData[ys[1]+xs[2]])+
                        maskY[2][0]*qGreen(newImageData[ys[2]+xs[0]])+
                        maskY[2][1]*qGreen(newImageData[ys[2]+xs[1]])+
                        maskY[2][2]*qGreen(newImageData[ys[2]+xs[2]])
                    ,2)/(divisor*divisor)
                    ),255)
                        ,
                        qMin((int)qSqrt(
                    qPow(
                        maskX[0][0]*qBlue(newImageData[ys[0]+xs[0]])+
                        maskX[0][1]*qBlue(newImageData[ys[0]+xs[1]])+
                        maskX[0][2]*qBlue(newImageData[ys[0]+xs[2]])+
                        maskX[1][0]*qBlue(newImageData[ys[1]+xs[0]])+
                        maskX[1][1]*qBlue(newImageData[ys[1]+xs[1]])+
                        maskX[1][2]*qBlue(newImageData[ys[1]+xs[2]])+
                        maskX[2][0]*qBlue(newImageData[ys[2]+xs[0]])+
                        maskX[2][1]*qBlue(newImageData[ys[2]+xs[1]])+
                        maskX[2][2]*qBlue(newImageData[ys[2]+xs[2]])
                    ,2)/(divisor*divisor)
                    +
                    qPow(
                        maskY[0][0]*qBlue(newImageData[ys[0]+xs[0]])+
                        maskY[0][1]*qBlue(newImageData[ys[0]+xs[1]])+
                        maskY[0][2]*qBlue(newImageData[ys[0]+xs[2]])+
                        maskY[1][0]*qBlue(newImageData[ys[1]+xs[0]])+
                        maskY[1][1]*qBlue(newImageData[ys[1]+xs[1]])+
                        maskY[1][2]*qBlue(newImageData[ys[1]+xs[2]])+
                        maskY[2][0]*qBlue(newImageData[ys[2]+xs[0]])+
                        maskY[2][1]*qBlue(newImageData[ys[2]+xs[1]])+
                        maskY[2][2]*qBlue(newImageData[ys[2]+xs[2]])
                    ,2)/(divisor*divisor)
                    ),255)
                    );
        }
    }
    delete newImage;
    return image;
}
QImage *ImageFunctions::heavyMeanFilter(QImage *image, int N, bool copy)
{
    int divisor=N+8;
    int mask[3][3]={
        {1,1,1},
        {1,1,1},
        {1,1,1}
    };
    mask[1][1]=N;
    QImage *newImage;
    if(copy){
        newImage=new QImage(*image);
    }else{
        newImage=image;
    }
    applyMask(newImage,mask,divisor);
    return newImage;
}
QImage *ImageFunctions::sobel(QImage *image, bool copy)
{
    int divisor=4;
    int maskX[3][3]={
        {1,0,-1},
        {2,0,-2},
        {1,0,-1}
    };
    int maskY[3][3]={
        {-1,-2,-1},
        {0,0,0},
        {1,2,1}
    };
    QImage *newImage;
    if(copy){
        newImage=new QImage(*image);
    }else{
        newImage=image;
    }
    highPassAlgorithm(newImage,maskX,maskY,divisor);
    return newImage;
}
QImage *ImageFunctions::prewitt(QImage *image, bool copy)
{
    int divisor=3;
    int maskX[3][3]={
        {1,0,-1},
        {1,0,-1},
        {1,0,-1}
    };
    int maskY[3][3]={
        {-1,-1,-1},
        {0,0,0},
        {1,1,1}
    };
    QImage *newImage;
    if(copy){
        newImage=new QImage(*image);
    }else{
        newImage=image;
    }
    highPassAlgorithm(newImage,maskX,maskY,divisor);
    return newImage;
}
QImage *ImageFunctions::roberts(QImage *image, bool copy)
{
    int divisor=1;
    int maskX[3][3]={
        {0,0,-1},
        {0,1,0},
        {0,0,0}
    };
    int maskY[3][3]={
        {-1,0,0},
        {0,1,0},
        {0,0,0}
    };
    QImage *newImage;
    if(copy){
        newImage=new QImage(*image);
    }else{
        newImage=image;
    }
    highPassAlgorithm(newImage,maskX,maskY,divisor);
    return newImage;
}
QImage *ImageFunctions::pixelDifference(QImage *image, bool copy)
{
    int divisor=1;
    int maskX[3][3]={
        {0,0,},
        {0,1,-1},
        {0,0,0}
    };
    int maskY[3][3]={
        {0,-1,0},
        {0,1,0},
        {0,0,0}
    };
    QImage *newImage;
    if(copy){
        newImage=new QImage(*image);
    }else{
        newImage=image;
    }
    highPassAlgorithm(newImage,maskX,maskY,divisor);
    return newImage;
}
QImage *ImageFunctions::mode(QImage *image, bool copy)
{
    QImage *newImage=new QImage(*image);
    if(!copy){
        QImage *swap;
        swap=newImage;
        newImage=image;
        image=swap;
    }
    int ys[3],xs[3];
    QRgb *imageArray=(QRgb*)image->bits();
    QRgb *newImageArray=(QRgb*)newImage->bits();
    for(int y=0;y<image->height();y++){
        ys[0]=newImage->width()*qMax(y-1,0);
        ys[1]=newImage->width()*y;
        ys[2]=newImage->width()*qMin(y+1,image->height()-1);
        for(int x=0;x<image->width();x++){
            xs[0]=qMax(x-1,0);
            xs[1]=x;
            xs[2]=qMin(x+1,image->width()-1);
            newImageArray[y*image->width()+x]=mode(
                imageArray[ys[0]+xs[0]],
                imageArray[ys[0]+xs[1]],
                imageArray[ys[0]+xs[2]],
                imageArray[ys[1]+xs[0]],
                imageArray[ys[1]+xs[1]],
                imageArray[ys[1]+xs[2]],
                imageArray[ys[2]+xs[0]],
                imageArray[ys[2]+xs[1]],
                imageArray[ys[2]+xs[2]]
            );
        }
    }
    if(!copy){
        delete image;
    }
    return newImage;
}
QRgb ImageFunctions::mode(QRgb v1, QRgb v2, QRgb v3, QRgb v4, QRgb v5, QRgb v6, QRgb v7, QRgb v8, QRgb v9)
{
    QRgb values[9];
    values[0]=v1;
    values[1]=v2;
    values[2]=v3;
    values[3]=v4;
    values[4]=v5;
    values[5]=v6;
    values[6]=v7;
    values[7]=v8;
    values[8]=v9;
    int max=0;
    int count[9]={0,0,0,0,0,0,0,0,0};
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            if(values[i]==values[j]){
                count[i]+=1;
            }
        }
        if(count[i]>count[max]){
            max=i;
        }
    }
    return values[max];
}
QImage *ImageFunctions::median(QImage *image, bool copy)
{
    QImage *newImage=new QImage(*image);
    if(!copy){
        QImage *swap;
        swap=newImage;
        newImage=image;
        image=swap;
    }
    int ys[3],xs[3];
    QRgb *imageArray=(QRgb*)image->bits();
    QRgb *newImageArray=(QRgb*)newImage->bits();
    for(int y=0;y<image->height();y++){
        ys[0]=newImage->width()*qMax(y-1,0);
        ys[1]=newImage->width()*y;
        ys[2]=newImage->width()*qMin(y+1,image->height()-1);
        for(int x=0;x<image->width();x++){
            xs[0]=qMax(x-1,0);
            xs[1]=x;
            xs[2]=qMin(x+1,image->width()-1);
            newImageArray[y*image->width()+x]=median(
                imageArray[ys[0]+xs[0]],
                imageArray[ys[0]+xs[1]],
                imageArray[ys[0]+xs[2]],
                imageArray[ys[1]+xs[0]],
                imageArray[ys[1]+xs[1]],
                imageArray[ys[1]+xs[2]],
                imageArray[ys[2]+xs[0]],
                imageArray[ys[2]+xs[1]],
                imageArray[ys[2]+xs[2]]
            );
        }
    }
    if(!copy){
        delete image;
    }
    return newImage;
}
QRgb ImageFunctions::median(QRgb v1, QRgb v2, QRgb v3, QRgb v4, QRgb v5, QRgb v6, QRgb v7, QRgb v8, QRgb v9)
{
    QRgb values[9];
    values[0]=v1;
    values[1]=v2;
    values[2]=v3;
    values[3]=v4;
    values[4]=v5;
    values[5]=v6;
    values[6]=v7;
    values[7]=v8;
    values[8]=v9;
    QRgb swap;
    bool swapped=true;
    while(swapped){
        swapped=false;
        for(int j=0;j<8;j++){
            if((qRed(values[j])+qBlue(values[j])+qGreen(values[j]))>(qRed(values[j+1])+qBlue(values[j+1])+qGreen(values[j+1]))){
                swap=values[j];
                values[j]=values[j+1];
                values[j+1]=swap;
                swapped=true;
            }
        }
    }
    return values[4];
}
QImage *ImageFunctions::max(QImage *image, bool copy)
{
    QImage *newImage=new QImage(*image);
    if(!copy){
        QImage *swap;
        swap=newImage;
        newImage=image;
        image=swap;
    }
    int ys[3],xs[3];
    QRgb *imageArray=(QRgb*)image->bits();
    QRgb *newImageArray=(QRgb*)newImage->bits();
    for(int y=0;y<image->height();y++){
        ys[0]=newImage->width()*qMax(y-1,0);
        ys[1]=newImage->width()*y;
        ys[2]=newImage->width()*qMin(y+1,image->height()-1);
        for(int x=0;x<image->width();x++){
            xs[0]=qMax(x-1,0);
            xs[1]=x;
            xs[2]=qMin(x+1,image->width()-1);
            newImageArray[y*image->width()+x]=max(
                imageArray[ys[0]+xs[0]],
                imageArray[ys[0]+xs[1]],
                imageArray[ys[0]+xs[2]],
                imageArray[ys[1]+xs[0]],
                imageArray[ys[1]+xs[1]],
                imageArray[ys[1]+xs[2]],
                imageArray[ys[2]+xs[0]],
                imageArray[ys[2]+xs[1]],
                imageArray[ys[2]+xs[2]]
            );
        }
    }
    if(!copy){
        delete image;
    }
    return newImage;
}
QRgb ImageFunctions::max(QRgb v1, QRgb v2, QRgb v3, QRgb v4, QRgb v5, QRgb v6, QRgb v7, QRgb v8, QRgb v9)
{
    QRgb values[9];
    values[0]=v1;
    values[1]=v2;
    values[2]=v3;
    values[3]=v4;
    values[4]=v5;
    values[5]=v6;
    values[6]=v7;
    values[7]=v8;
    values[8]=v9;
    int max=0;
    for(int j=1;j<9;j++){
        if((qRed(values[j])+qBlue(values[j])+qGreen(values[j]))>(qRed(values[max])+qBlue(values[max])+qGreen(values[max]))){
            max=j;
        }
    }
    return values[max];
}
QImage *ImageFunctions::min(QImage *image, bool copy)
{
    QImage *newImage=new QImage(*image);
    if(!copy){
        QImage *swap;
        swap=newImage;
        newImage=image;
        image=swap;
    }
    int ys[3],xs[3];
    QRgb *imageArray=(QRgb*)image->bits();
    QRgb *newImageArray=(QRgb*)newImage->bits();
    for(int y=0;y<image->height();y++){
        ys[0]=newImage->width()*qMax(y-1,0);
        ys[1]=newImage->width()*y;
        ys[2]=newImage->width()*qMin(y+1,image->height()-1);
        for(int x=0;x<image->width();x++){
            xs[0]=qMax(x-1,0);
            xs[1]=x;
            xs[2]=qMin(x+1,image->width()-1);
            newImageArray[y*image->width()+x]=min(
                imageArray[ys[0]+xs[0]],
                imageArray[ys[0]+xs[1]],
                imageArray[ys[0]+xs[2]],
                imageArray[ys[1]+xs[0]],
                imageArray[ys[1]+xs[1]],
                imageArray[ys[1]+xs[2]],
                imageArray[ys[2]+xs[0]],
                imageArray[ys[2]+xs[1]],
                imageArray[ys[2]+xs[2]]
            );
        }
    }
    if(!copy){
        delete image;
    }
    return newImage;
}
QRgb ImageFunctions::min(QRgb v1, QRgb v2, QRgb v3, QRgb v4, QRgb v5, QRgb v6, QRgb v7, QRgb v8, QRgb v9)
{
    QRgb values[9];
    values[0]=v1;
    values[1]=v2;
    values[2]=v3;
    values[3]=v4;
    values[4]=v5;
    values[5]=v6;
    values[6]=v7;
    values[7]=v8;
    values[8]=v9;
    int min=0;
    for(int j=1;j<9;j++){
        if((qRed(values[j])+qBlue(values[j])+qGreen(values[j]))<(qRed(values[min])+qBlue(values[min])+qGreen(values[min]))){
            min=j;
        }
    }
    return values[min];
}

QImage *ImageFunctions::expansion(QImage *image)
{
    QImage *grayImage=toGrayScale(image);
    QRgb *grayImageBits=(QRgb *)grayImage->bits();
    QList<int> *histogramList=grayHistogramList(image);
    int grayImageMin,grayImageMax;
    for(int i=0;i<256;i++){
        if(histogramList->at(i)>0){
            grayImageMin=i;
            break;
        }
    }
    for(int i=255;i>-1;i--){
        if(histogramList->at(i)>0){
            grayImageMax=i;
            break;
        }
    }
    int width=image->width();
    int height=image->height();
    QImage *newImage=new QImage(*grayImage);
    QRgb *newImageBits=(QRgb *)newImage->bits();
    int newGrayValue;
    for(int y=0;y<height;y++){
        for(int x=0;x<width;x++){
            if(grayImageMax>grayImageMin){
                newGrayValue=(qRed(grayImageBits[y*width+x])-grayImageMin)*255/(grayImageMax-grayImageMin);
            }else{
                newGrayValue=(qRed(grayImageBits[y*width+x])-grayImageMin)*255;
            }
            newImageBits[y*width+x]=qRgb(newGrayValue,newGrayValue,newGrayValue);
        }
    }
    return newImage;
}

QImage *ImageFunctions::compression(QImage *image, int min, int max)
{
    QImage *grayImage=toGrayScale(image);
    QRgb *grayImageBits=(QRgb *)grayImage->bits();
    QList<int> *histogramList=grayHistogramList(image);
    int grayImageMin,grayImageMax;
    for(int i=0;i<256;i++){
        if(histogramList->at(i)>0){
            grayImageMin=i;
            break;
        }
    }
    for(int i=255;i>-1;i--){
        if(histogramList->at(i)>0){
            grayImageMax=i;
            break;
        }
    }
    int width=image->width();
    int height=image->height();
    QImage *newImage=new QImage(*grayImage);
    QRgb *newImageBits=(QRgb *)newImage->bits();
    int newGrayValue;
    for(int y=0;y<height;y++){
        for(int x=0;x<width;x++){
            if(grayImageMax>grayImageMin){
                newGrayValue=(qRed(grayImageBits[y*width+x])-grayImageMin)*(max-min)/(grayImageMax-grayImageMin)+min;
            }else{
                newGrayValue=(qRed(grayImageBits[y*width+x])-grayImageMin)*(max-min)+min;
            }
            newImageBits[y*width+x]=qRgb(newGrayValue,newGrayValue,newGrayValue);
        }
    }
    return newImage;
}

QImage *ImageFunctions::displacement(QImage *image, int value)
{
    QImage *grayImage=toGrayScale(image);
    QRgb *grayImageBits=(QRgb *)grayImage->bits();
    int width=image->width();
    int height=image->height();
    QImage *newImage=new QImage(*grayImage);
    QRgb *newImageBits=(QRgb *)newImage->bits();
    int newGrayValue;
    for(int y=0;y<height;y++){
        for(int x=0;x<width;x++){
            newGrayValue=(qRed(grayImageBits[y*width+x])+value);
            if(newGrayValue>255){
                newGrayValue=255;
            }else if(newGrayValue<0){
                newGrayValue=0;
            }
            newImageBits[y*width+x]=qRgb(newGrayValue,newGrayValue,newGrayValue);
        }
    }
    return newImage;
}

QImage *ImageFunctions::equalization()
{

}
