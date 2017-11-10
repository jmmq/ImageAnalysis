#ifndef IMAGEFUNCTIONS_H
#define IMAGEFUNCTIONS_H
#include<QImage>
#include<QList>
class ImageFunctions
{
public:
    enum Channel {Red,Green,Blue};
    static QImage *toGrayScale(QImage* image, bool copy=true);
    static QImage *binarize(QImage* image, bool copy,int threshold,bool inverse);
    static QImage *binarizeChannel(QImage* image, bool copy,int threshold,bool inverse,Channel channel);
    static QImage *openChannel(QImage* image, bool copy,Channel channel);
    static QImage *addition(QImage *imageA,QImage *imageB,bool copy=true);
    static QImage *subtraction(QImage *imageA,QImage *imageB,bool copy=true);
    static QImage *multiplication(QImage *imageA,QImage *imageB,bool copy=true);
    static QImage *logicNot(QImage* image, bool copy=true);
    static QImage *logicAND(QImage *imageA,QImage *imageB,bool copy=true);
    static QImage *logicOR(QImage *imageA,QImage *imageB,bool copy=true);
    static QImage *logicXOR(QImage *imageA,QImage *imageB,bool copy=true);
    static QList<int> *grayHistogramList(QImage *image);
    static QList<int> *redHistogramList(QImage *image);
    static QList<int> *greenHistogramList(QImage *image);
    static QList<int> *blueHistogramList(QImage *image);
    static double grayMean(QImage *image,QList<int> *histogramList);
    static double grayVariance(QImage *image,QList<int> *histogramList,double mean);
    static double grayEnergy(QImage *image, QList<int> *histogramList);
    static double grayAsymmetry(QImage *image,QList<int> *histogramList,double mean);
    static QImage *meanFilter(QImage *image,bool copy);
    static QImage *gaussianFilter(QImage *image,bool copy);
    static QImage *applyMask(QImage *image,int mask[3][3],int divisor);
    static QImage *highPassAlgorithm(QImage *image,int maskX[3][3],int maskY[3][3],int divisor);
    static QImage *heavyMeanFilter(QImage *image,int N,bool copy);
    static QImage *sobel(QImage *image,bool copy);
    static QImage *prewitt(QImage *image,bool copy);
    static QImage *roberts(QImage *image,bool copy);
    static QImage *pixelDifference(QImage *image,bool copy);
    static QImage *mode(QImage *image,bool copy);
    static QRgb mode(QRgb v1,QRgb v2,QRgb v3,QRgb v4,QRgb v5,QRgb v6,QRgb v7,QRgb v8,QRgb v9);
    static QImage *median(QImage *image,bool copy);
    static QRgb median(QRgb v1,QRgb v2,QRgb v3,QRgb v4,QRgb v5,QRgb v6,QRgb v7,QRgb v8,QRgb v9);
    static QImage max(QImage *image,bool copy);
    static QImage min(QImage *image,bool copy);
};
#endif // IMAGEFUNCTIONS_H
