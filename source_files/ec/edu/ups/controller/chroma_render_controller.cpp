#include "../../../../../header_files/chroma_render_controller.hpp"

ChromaRenderController::ChromaRenderController(string title, int w, QWidget *parent) 
    : MatRender(title, w, parent) {}

ChromaRenderController::ChromaRenderController(Frame *video, Camera *camera, string title, int w, QWidget *parent)
    : MatRender(title, w, parent) {
    this->video = video;
    this->camera = camera;
}

void ChromaRenderController::merge(){
    std::lock_guard<std::mutex> guard(frame_mutex);
    cameraOriginal = camera->getFrame().clone();
    cameraFiltered = cameraOriginal.clone();
    
    cameraOriginal = applyColorSpace(cameraOriginal, FRAME_TO_RGB);
    cameraFiltered = applyColorSpace(cameraFiltered, colorSpace);

    
    
    // vector<cv::Mat> canales;
    // cv::Mat cielab;
    // cv::split(cameraFiltered, canales);
    // cv::equalizeHist(canales[0], canales[0]);
    // cv::equalizeHist(canales[1], canales[1]);
    // cv::equalizeHist(canales[2], canales[2]);
    // cv::merge(canales, cameraFiltered);

    //----

    cameraThreshold = makeBinaryThresholding(cameraFiltered);
    cameraThreshold = applyFilter(cameraThreshold);
    Mat edge = cameraThreshold.clone();
    edge = applyEdgeDetector(edge);
    cameraThreshold += edge;
    cameraThreshold = applyMorphologicalOperation(cameraThreshold);


    //----

    // applyMorphologicalOperation(cameraThreshold);

    // Mat edge = cameraThreshold.clone();
    // applyEdgeDetector(edge);
    // cameraThreshold += edge;

    // applyFilter(cameraThreshold);
    
    
    // ----
    

    // applyFilter(cameraThreshold);

    // cameraThreshold = makeBinaryThresholding(cameraFiltered);

    // applyFilter(cameraThreshold);

    // applyMorphologicalOperation(cameraThreshold);

    // Mat edge = cameraThreshold.clone();
    // applyEdgeDetector(edge);
    // cameraThreshold += edge;
    
    

    cv::resize(video->getFrame(), videoRS, cv::Size(camera->width, camera->height));

    applyChromaEffect(cameraOriginal, videoRS, cameraThreshold, result);
}

// Color Space

cv::Mat ChromaRenderController::applyRGB(cv::Mat frame){
    cv::cvtColor(frame, frame, COLOR_BGR2RGB);
    return frame;
}

cv::Mat ChromaRenderController::applyHSV(cv::Mat frame){
    cv::cvtColor(frame, frame, COLOR_BGR2HSV);
    return frame;
}

cv::Mat ChromaRenderController::applyBGR(cv::Mat frame){
    return frame;
}

cv::Mat ChromaRenderController::applyYCbCr(cv::Mat frame){
    cv::cvtColor(frame, frame, COLOR_BGR2YCrCb);
    return frame;
}

cv::Mat ChromaRenderController::applyLab(cv::Mat frame){
    cv::cvtColor(frame, frame, COLOR_BGR2Lab);
    return frame;
}

// Filter
cv::Mat ChromaRenderController::applyMedianBlur(cv::Mat frame){
    medianBlur(frame, frame, filterKernelSize);
    return frame;
}

cv::Mat ChromaRenderController::applyGaussianBlur(cv::Mat frame){
    double sigma = 0.3 * (((double) filterKernelSize - 1) * 0.5 - 1) + 0.8;
    Size kernel = Size(filterKernelSize, filterKernelSize);

    GaussianBlur(frame, frame, kernel, sigma, sigma);
    return frame;
}

// Edge detector
cv::Mat ChromaRenderController::applyCanny(cv::Mat frame){
    cv::Mat cannyEdge;
    int ratio = 3;
    int kernelCanny = edgeKernelSize;

    if (kernelCanny < 3) {
        kernelCanny = 3;
    } else if (kernelCanny > 7){
        kernelCanny = 7;
    } else {
        kernelCanny = edgeKernelSize;
    }

    Canny(frame.clone(), cannyEdge, threshhold, threshhold*ratio, kernelCanny);
    return cannyEdge;
}

cv::Mat ChromaRenderController::applySobel(cv::Mat frame){
    cv::Mat sobelX;
    cv::Mat sobelY;
    cv::Mat sobelXAbs;
    cv::Mat sobelYAbs;

    Sobel(frame, sobelX, CV_16S, 1, 0, edgeKernelSize);
    Sobel(frame, sobelY, CV_16S, 0, 1, edgeKernelSize);
    convertScaleAbs(sobelX, sobelXAbs);
    convertScaleAbs(sobelY, sobelYAbs);
    addWeighted(sobelXAbs, 0.5, sobelYAbs, 0.5, 0, frame);

    return frame;
}

cv::Mat ChromaRenderController::applyLaplacian(cv::Mat frame){
    cv::Mat laplacian;

    Laplacian(frame, laplacian, CV_16S, edgeKernelSize);
    convertScaleAbs(laplacian, frame);

    return frame;
}

cv::Mat ChromaRenderController::makeBinaryThresholding(cv::Mat frame){
    cv::Mat thresholdized;
    cv::inRange(frame, Scalar(channel1Min, channel2Min, channel3Min), 
                Scalar(channel1Max, channel2Max, channel3Max), thresholdized);

    return thresholdized;
}

void ChromaRenderController::applyChromaEffect(cv::Mat camera, cv::Mat video, cv::Mat thresholdized, cv::Mat &output){
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
    cv::bitwise_not(thresholdized, cameraThresholdN);
    videoFusionBackground = video.clone();
    output = camera + video;
}

cv::Mat ChromaRenderController::applyColorSpace(cv::Mat frame, int colorSpace){
    switch (colorSpace) {
    case FRAME_TO_RGB:
        frame = applyRGB(frame);
        break;
    case FRAME_TO_HSV:
        frame = applyHSV(frame);
        break;
    case FRAME_TO_BGR:
        frame = applyBGR(frame);
        break;
    case FRAME_TO_YCbCr:
        frame = applyYCbCr(frame);
        break;
    case FRAME_TO_Lab:
        frame = applyLab(frame);
        break;
    default:
        break;
    }
    return frame;
}

cv::Mat ChromaRenderController::applyFilter(cv::Mat frame){
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
    return frame;
}

cv::Mat ChromaRenderController::applyEdgeDetector(cv::Mat frame){
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
    return frame;
}

cv::Mat ChromaRenderController::applyMorphologicalOperation(cv::Mat frame){
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
    return frame;
}

cv::Mat ChromaRenderController::getResult(){
    return result;
}

void ChromaRenderController::setVideo(Frame *video){
    this->video = video;
}

void ChromaRenderController::setCamera(Camera *camera){
    this->camera = camera;
}

void ChromaRenderController::setColorSpace(int colorSpace){
    this->colorSpace = colorSpace;
}

void ChromaRenderController::setFilter(int filter){
    this->filter = filter;
}

void ChromaRenderController::setEdgeDetector(int edgeDetector){
    this->edgeDetector = edgeDetector;
}

void ChromaRenderController::setMorphologicalOperation(int morphologicalOperation){
    this->morphologicalOperation = morphologicalOperation;
}

void ChromaRenderController::setFilterKernelSize(int filterKernelSize) {
    this->filterKernelSize = filterKernelSize;
}

void ChromaRenderController::setEdgeKernelSize(int edgeKernelSize) {
    this->edgeKernelSize = edgeKernelSize;
}

void ChromaRenderController::setMOpKernelSize(int mOpKernelSize) {
    mo.createKernel(mOpKernelSize);
}

void ChromaRenderController::setMinimunChannelValues(int channel1Min, int channel2Min, int channel3Min){
    this->channel1Min = channel1Min;
    this->channel2Min = channel2Min;
    this->channel3Min = channel3Min;
}


void ChromaRenderController::setMaximunChannelValues(int channel1Max, int channel2Max, int channel3Max){
    this->channel1Max = channel1Max;
    this->channel2Max = channel2Max;
    this->channel3Max = channel3Max;
}

void ChromaRenderController::setThreshhold(int threshhold){
    this->threshhold = threshhold;
}

void ChromaRenderController::setChannel1Min(int value) {
    this->channel1Min = value;
}

void ChromaRenderController::setChannel2Min(int value) {
    this->channel2Min = value;
}

void ChromaRenderController::setChannel3Min(int value) {
    this->channel3Min = value;
}

void ChromaRenderController::setChannel1Max(int value) {
    this->channel1Max = value;
}

void ChromaRenderController::setChannel2Max(int value) {
    this->channel2Max = value;
}

void ChromaRenderController::setChannel3Max(int value) {
    this->channel3Max = value;
}

cv::Mat ChromaRenderController::getCameraThreshold(){
    return cameraThreshold;
}

cv::Mat ChromaRenderController::getVideoFusionBackground(){
    return videoFusionBackground;
}

cv::Mat ChromaRenderController::getCameraThresholdN(){
    return cameraThresholdN;
}