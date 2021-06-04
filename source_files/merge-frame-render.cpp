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
    cameraFiltered = camera->getFrame().clone();


    applyColorSpace(cameraFiltered, colorSpace);
    applyMorphologicalOperation(cameraFiltered);

    cv::resize(video->getFrame(), videoRS, cv::Size(camera->width, camera->height));
    // result = cv::abs(videoRS - cameraFiltered);
    result = cameraFiltered;
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
void MergeFrameRender::applyMedianBlur(cv::Mat frame){

}

void MergeFrameRender::applyGaussianBlur(cv::Mat frame){

}

// Edge detector
void MergeFrameRender::applyCanny(cv::Mat frame){
    // Canny(frame, edgeFrame, thres, thres*ratio, kernelCanny);
}

void MergeFrameRender::applySobel(cv::Mat frame){

}
void MergeFrameRender::applyLaplacian(cv::Mat frame){

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

void MergeFrameRender::applyFilter(cv::Mat frame){
    switch (filter) {
    case FRAME_TO_MEDBLUR:
        applyMedianBlur(frame);
        break;
    case FRAME_TO_GAUBLUR:
        applyGaussianBlur(frame);
        break;
    default:
        break;
    }
}

void MergeFrameRender::applyEdgeDetector(cv::Mat frame){
    switch (edgeDetector) {
    case FRAME_TO_CANNY:
        applyCanny(frame);
        break;
    case FRAME_TO_SOBEL:
        applySobel(frame);
        break;
    case FRAME_TO_LAPLACIAN:
        applyLaplacian(frame);
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