#include "../header_files/header.hpp"

MergeFrameRender::MergeFrameRender(string title, int w, QWidget *parent) 
    : MatRender(title, w, parent) {}

MergeFrameRender::MergeFrameRender(Frame *video, Camera *camera, string title, int w, QWidget *parent)
    : MatRender(title, w, parent) {
    this->video = video;
    this->camera = camera;
}

void MergeFrameRender::merge(){
    std::lock_guard<std::mutex> guard(frame_mutex);
    cv::Mat cameraOriginal = camera->getFrame().clone();
    applyColorSpace(cameraOriginal, FRAME_TO_RGB);

    cameraFiltered = cameraOriginal.clone();

    applyColorSpace(cameraFiltered, colorSpace);

    cameraThreshold = makeBinaryThresholding(cameraFiltered);

    applyFilter(cameraThreshold);
    applyEdgeDetector(cameraThreshold);
    applyMorphologicalOperation(cameraThreshold);

    cv::resize(video->getFrame(), videoRS, cv::Size(camera->width, camera->height));

    applyChromaEffect(cameraOriginal, videoRS, cameraThreshold, result);
    // result = cv::abs(videoRS - cameraFiltered);
    // result = cameraFiltered;
    // result = this->cameraThreshold;
}

// Color Space

void MergeFrameRender::applyRGB(cv::Mat frame){
    cv::cvtColor(frame, frame, COLOR_BGR2RGB);
}

void MergeFrameRender::applyHSV(cv::Mat frame){
    cv::cvtColor(frame, frame, COLOR_BGR2HSV);
}

void MergeFrameRender::applyBGR(cv::Mat frame){
    // Same
}

void MergeFrameRender::applyYCbCr(cv::Mat frame){
    cv::cvtColor(frame, frame, COLOR_BGR2YCrCb);
}

void MergeFrameRender::applyLab(cv::Mat frame){
    cv::cvtColor(frame, frame, COLOR_BGR2Lab);
}

// Filter
cv::Mat MergeFrameRender::applyMedianBlur(cv::Mat frame){
    medianBlur(frame, frame, kernelSize);
    return frame;
}

cv::Mat MergeFrameRender::applyGaussianBlur(cv::Mat frame){
    double sigma = 0.3 * (((double) kernelSize - 1) * 0.5 - 1) + 0.8;
    Size kernel = Size(kernelSize, kernelSize);

    GaussianBlur(frame, frame, kernel, sigma, sigma);
    return frame;
}

// Edge detector
cv::Mat MergeFrameRender::applyCanny(cv::Mat frame){
    cv::Mat cannyEdge;
    int thres = 10;
    int ratio = 3;
    int kernelCanny = kernelSize;

    if (kernelCanny < 3) {
        kernelCanny = 3;
    } else if (kernelCanny > 7){
        kernelCanny = 7;
    } else {
        kernelCanny = kernelSize;
    }

    Canny(frame.clone(), cannyEdge, thres, thres*ratio, kernelCanny);
    return cannyEdge;
}

cv::Mat MergeFrameRender::applySobel(cv::Mat frame){
    cv::Mat sobelX;
    cv::Mat sobelY;
    cv::Mat sobelXAbs;
    cv::Mat sobelYAbs;

    Sobel(frame, sobelX, CV_16S, 1, 0, kernelSize);
    Sobel(frame, sobelY, CV_16S, 0, 1, kernelSize);
    convertScaleAbs(sobelX, sobelXAbs);
    convertScaleAbs(sobelY, sobelYAbs);
    addWeighted(sobelXAbs, 0.5, sobelYAbs, 0.5, 0, frame);

    return frame;
}

cv::Mat MergeFrameRender::applyLaplacian(cv::Mat frame){
    cv::Mat laplacian;

    Laplacian(frame, laplacian, CV_16S, kernelSize);
    convertScaleAbs(laplacian, frame);

    return frame;
}

cv::Mat MergeFrameRender::makeBinaryThresholding(cv::Mat frame){
    cv::Mat thresholdized;

    // cv::inRange(frame, Scalar(145, 34, 0), 
    //             Scalar(145, 255, 255), thresholdized);
    cv::inRange(frame, Scalar(channel1Min, channel2Min, channel3Min), 
                Scalar(channel1Max, channel2Max, channel3Max), thresholdized);

    return thresholdized;
}

void MergeFrameRender::applyChromaEffect(cv::Mat camera, cv::Mat video, cv::Mat thresholdized, cv::Mat &output){
    int pixel = 0;
                
    for (int row = 0; row < thresholdized.rows; row++){
        for (int col = 0; col < thresholdized.cols; col++){
            pixel = thresholdized.at<uchar>(row, col);
            if (pixel != 0) {
                camera.at<Vec3b>(row, col) = Vec3b(0,0,0);
            } else {
                video.at<Vec3b>(row, col) = Vec3b(0,0,0);
            }
        }
    }

    output = camera + video;
}

void MergeFrameRender::applyColorSpace(cv::Mat frame, int colorSpace){
    switch (colorSpace) {
    case FRAME_TO_RGB:
        applyRGB(frame);
        break;
    case FRAME_TO_HSV:
        applyHSV(frame);
        break;
    case FRAME_TO_BGR:
        applyBGR(frame);
        break;
    case FRAME_TO_YCbCr:
        applyYCbCr(frame);
        break;
    case FRAME_TO_Lab:
        applyLab(frame);
        break;
    default:
        break;
    }
}

void MergeFrameRender::applyFilter(cv::Mat &frame){
    switch (filter) {
    case FRAME_TO_MEDBLUR:
        frame = applyMedianBlur(frame);
        break;
    case FRAME_TO_GAUBLUR:
        frame = applyGaussianBlur(frame);
        break;
    default:
        break;
    }
}

void MergeFrameRender::applyEdgeDetector(cv::Mat &frame){
    switch (edgeDetector) {
    case FRAME_TO_CANNY:
        frame = applyCanny(frame);
        break;
    case FRAME_TO_SOBEL:
        frame = applySobel(frame);
        break;
    case FRAME_TO_LAPLACIAN:
        frame = applyLaplacian(frame);
        break;
    default:
        break;
    }
}

void MergeFrameRender::applyMorphologicalOperation(cv::Mat & frame){
    // cout << morphologicalOperation << endl;
    switch (morphologicalOperation) {
    case FRAME_TO_DILATATION:
        frame = mo.applyDilatation(frame);
        break;
    case FRAME_TO_EROSION:
        frame = mo.applyErosion(frame);
        break;
    case FRAME_TO_OPEN:
        frame = mo.applyOpen(frame);
        break;
    case FRAME_TO_CLOSE:
        frame = mo.applyClose(frame);
        break;
    case FRAME_TO_BLACKHAT:
        frame = mo.applyBlackHat(frame);
        break;
    case FRAME_TO_TOPHAT:
        frame = mo.applyTopHat(frame);
        break;
    case FRAME_TO_EQUATION:
        frame = mo.applyEquation(frame);
        break;
    default:
        break;
    }
}

cv::Mat MergeFrameRender::getResult(){
    return result;
}

void MergeFrameRender::setVideo(Frame *video){
    this->video = video;
}

void MergeFrameRender::setCamera(Camera *camera){
    this->camera = camera;
}

void MergeFrameRender::setColorSpace(int colorSpace){
    this->colorSpace = colorSpace;
}

void MergeFrameRender::setFilter(int filter){
    this->filter = filter;
}

void MergeFrameRender::setEdgeDetector(int edgeDetector){
    this->edgeDetector = edgeDetector;
}

void MergeFrameRender::setMorphologicalOperation(int morphologicalOperation){
    this->morphologicalOperation = morphologicalOperation;
}

void MergeFrameRender::setKernelSize(int kernelSize){
    this->kernelSize = kernelSize;
    mo.createKernel(kernelSize);
}

void MergeFrameRender::setMinimunChannelValues(int channel1Min, int channel2Min, int channel3Min){
    this->channel1Min = channel1Min;
    this->channel2Min = channel2Min;
    this->channel3Min = channel3Min;
}


void MergeFrameRender::setMaximunChannelValues(int channel1Max, int channel2Max, int channel3Max){
    this->channel1Max = channel1Max;
    this->channel2Max = channel2Max;
    this->channel3Max = channel3Max;
}