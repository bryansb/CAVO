#include "../header_files/header.hpp"

MergeFrameRender::MergeFrameRender(string title, QWidget *parent) 
    : MatRender(title, parent) {}

MergeFrameRender::MergeFrameRender(Frame *video, Camera *camera, string title, QWidget *parent)
    : MatRender(title, parent) {
    this->video = video;
    this->camera = camera;
}

void MergeFrameRender::merge(){
    
    cv::resize(video->getFrame().clone(), videoRS, cv::Size(camera->width, camera->height));
    result = cv::abs(videoRS - camera->getFrame().clone());
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