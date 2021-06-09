#include "base.hpp"
#include <mutex>

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QImage>
#include <QCoreApplication>
#include <QtTest/QtTest>

class MatRender : public QWidget{
    private:
        QVBoxLayout *layout;
        QLabel *titleLabel;
        QLabel *frameLabel;
        QImage qimage;
        QPixmap pixmap;

        cv::Mat image;

        double ws;
        double hs;
        int w;

        std::mutex frame_mutex;

        

    public:
        bool busy = false;
        
        MatRender(string title, int w,
                    QWidget *parent);
        void render(cv::Mat, double percent);
        void setTitle(string title);

        void setWidth(int w);
};