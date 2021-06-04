#include "../header_files/header.hpp"

Frame::Frame(string path) 
    : cv::VideoCapture(path){
    set(CAP_PROP_BUFFERSIZE, 1);
}

Frame::Frame(int cam) 
    : cv::VideoCapture(cam){
    set(CAP_PROP_BUFFERSIZE, 1);
}

bool Frame::nextFrame(bool flip, int orientation){
    Mat frame;    
    this->read(frame);
    if (flip)
        flipFrame(frame, orientation);
    std::lock_guard<std::mutex> guard(frame_mutex);
    setFrame(frame);
    return frame.empty() ? false : true;
}

void Frame::flipFrame(cv::Mat frame, int orientation){
    cv::flip(frame, frame, orientation);
}

cv::Mat Frame::getFrame(){
    std::lock_guard<std::mutex> guard(frame_mutex);
    return frame;
}

void Frame::setFrame(cv::Mat f){
    this->frame = f;
}
