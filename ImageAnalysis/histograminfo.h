#ifndef HISTOGRAMINFO_H
#define HISTOGRAMINFO_H
#include<QImage>
#include<QList>
class HistogramInfo
{
public:
    HistogramInfo(QImage *image);
    QList<int> *grayHistogramList;
    QList<int> *redHistogramList;
    QList<int> *greenHistogramList;
    QList<int> *blueHistogramList;
    float grayMean;
    float grayVariance;
    float grayEnergy;
    float grayAsymmetry;

};

#endif // HISTOGRAMINFO_H
