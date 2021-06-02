#include "../header_files/header.hpp"

MatRender::MatRender(string title, QWidget *parent)
    :QWidget(parent){
    layout = new QVBoxLayout(this);
    frameLabel = new QLabel(this);
    titleLabel = new QLabel(QString::fromStdString(title));
    titleLabel->setMaximumHeight(100);
    
    layout->addWidget(titleLabel);
    layout->addWidget(frameLabel);
}

void MatRender::render(cv::Mat img, int w, double percent){
    busy = true;
    image = img.clone();
    ws = ((double)w * percent);
    hs = (ws / image.cols) * image.rows;
    cv::resize(image, image, Size(abs(ws), abs(hs)));
    frameLabel->setPixmap(QPixmap::fromImage(QImage(image.data, image.cols, image.rows, image.step, QImage::Format_RGB888)));
    busy = false;
}

void MatRender::setTitle(string title){
    titleLabel->setText(QString::fromStdString(title));
}