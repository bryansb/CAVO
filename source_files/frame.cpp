#include "../header_files/header.hpp"

Frame::Frame(string path) 
    : cv::VideoCapture(path){
}

Frame::Frame(int cam) 
    : cv::VideoCapture(cam){
}

bool Frame::nextFrame(bool flip, int orientation){
    this->read(frame);
    if (flip)
        flipFrame(orientation);
    return frame.empty() ? false:true;
}

void Frame::flipFrame(int orientation){
    cv::flip(frame, frame, orientation);
}

cv::Mat Frame::getFrame(){
    return frame;
}

void Frame::setFrame(cv::Mat f){
    cout << frame.empty() << endl;
    this->frame = f;
}