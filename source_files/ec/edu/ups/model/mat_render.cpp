#include "../../../../../header_files/mat_render.hpp"

MatRender::MatRender(string title, int w, QWidget *parent)
    :QWidget(parent){
    this->w = w;
    layout = new QVBoxLayout(this);
    frameLabel = new QLabel(this);
    titleLabel = new QLabel(QString::fromStdString(title));
    titleLabel->setMaximumHeight(100);
    
    layout->addWidget(titleLabel);
    layout->addWidget(frameLabel);
}

void MatRender::render(cv::Mat img, double percent){
    Mat image = img.clone();
    if (image.cols != 0 && image.rows != 0) {
        resizeEvent(0);
        if (image.channels() == 1) {
            cv::cvtColor(image, image, cv::COLOR_GRAY2RGB);
        }
        ws = ((double)w * percent);
        hs = (ws / image.cols) * image.rows;
        cv::resize(image, image, Size(abs(ws), abs(hs)));
        qimage = QImage((uchar*)image.data, image.cols, image.rows, image.step, QImage::Format_RGB888);
        pixmap = QPixmap::fromImage(qimage.copy());
        
        frameLabel->setUpdatesEnabled(false);
        frameLabel->setPixmap(pixmap);
        frameLabel->setUpdatesEnabled(true);
        update();
    }
}

void MatRender::setTitle(string title){
    titleLabel->setText(QString::fromStdString(title));
}

void MatRender::setWidth(int w) {
    this->w = w;
}