#include "../header_files/header.hpp"

Frame::Frame(string path) 
    : cv::VideoCapture(path){
}

Frame::Frame(int cam) 
    : cv::VideoCapture(cam){
}

bool Frame::nextFrame(){
    this->read(frame);
    return frame.empty() ? false:true;
}

cv::Mat Frame::getFrame(){
    return frame;
}