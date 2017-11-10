#include "histograminfo.h"
#include"imagefunctions.h"
HistogramInfo::HistogramInfo(QImage *image)
{
    grayHistogramList=ImageFunctions::grayHistogramList(image);
    redHistogramList=ImageFunctions::redHistogramList(image);
    greenHistogramList=ImageFunctions::greenHistogramList(image);
    blueHistogramList=ImageFunctions::blueHistogramList(image);
    grayMean=ImageFunctions::grayMean(image,grayHistogramList);
    grayVariance=ImageFunctions::grayVariance(image,grayHistogramList,grayMean);
    grayAsymmetry=ImageFunctions::grayAsymmetry(image,grayHistogramList,grayMean);
    grayEnergy=ImageFunctions::grayEnergy(image,grayHistogramList);
}
