#include "../header_files/header.hpp"

MorphologicalOperation::MorphologicalOperation(int width, int height){
    this->kernel = cv::getStructuringElement(MORPH_CROSS, Size(width, height));
}

cv::Mat MorphologicalOperation::applyErosion(cv::Mat & image){
    Mat result;
    cv::morphologyEx(image, result, MORPH_ERODE, kernel);
    return result;
}

cv::Mat MorphologicalOperation::applyDilatation(cv::Mat image){
    Mat result;
    cv::morphologyEx(image, result, MORPH_DILATE, kernel);
    return result;
}

cv::Mat MorphologicalOperation::applyOpen(cv::Mat image){
    Mat result;
    cv::morphologyEx(image, result, MORPH_OPEN, kernel);
    return result;
}

cv::Mat MorphologicalOperation::applyClose(cv::Mat image){
    Mat result;
    cv::morphologyEx(image, result, MORPH_CLOSE, kernel);
    return result;
}

cv::Mat MorphologicalOperation::applyTopHat(cv::Mat image){
    Mat result;
    cv::morphologyEx(image, result, MORPH_TOPHAT, kernel);
    return result;
}

cv::Mat MorphologicalOperation::applyBlackHat(cv::Mat image){
    Mat result;
    cv::morphologyEx(image, result, MORPH_BLACKHAT, kernel);
    return result;
}

cv::Mat MorphologicalOperation::applyEquation(cv::Mat image){
    return image + (applyTopHat(image) - applyBlackHat(image));
}

void MorphologicalOperation::createKernel(int size){
    if (size % 2 != 0 && size != 0)
        this->kernel = cv::getStructuringElement(MORPH_CROSS, Size(size, size));
    else 
        throw int(21);
}