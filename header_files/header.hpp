#include <iostream>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <cmath>
#include <string>
#include <thread>
#include <chrono>

#include <opencv2/core/core.hpp> 
#include <opencv2/highgui/highgui.hpp> 
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp> 
#include <opencv2/video/video.hpp> 
#include <opencv2/videoio/videoio.hpp>

#include <QApplication>
#include <QLabel>
#include <QWidget>
#include <QMainWindow>
#include <QGridLayout>
#include <QGroupBox>
#include <QLineEdit>
#include <QPushButton>
#include <QGraphicsScene>
#include <QFileDialog>
#include <QScrollArea>
#include <QLayout>
#include <QMessageBox>
#include <QSlider>
#include <QStackedWidget>
#include <QCloseEvent>

#include "../header_files/slider-group.hpp"


using namespace std;
using namespace cv;
using namespace std::chrono;

// Color space
#define FRAME_TO_RGB = 0;
#define FRAME_TO_HSV = 1;
#define FRAME_TO_BGR = 2;
#define FRAME_TO_YCbCr = 3;
#define FRAME_TO_Lab = 4;

// Filter
#define FRAME_TO_MEDBLUR = 0;
#define FRAME_TO_GAUBLUR = 1;

// Edge detector
#define FRAME_TO_CANNY = 0;
#define FRAME_TO_SOBEL = 1;
#define FRAME_TO_LAPLACIAN = 2;

// Morphological Operations
#define FRAME_TO_DILATATION = 0;
#define FRAME_TO_EROSION = 1;
#define FRAME_TO_OPEN = 2;
#define FRAME_TO_CLOSE = 3;
#define FRAME_TO_BLACKHAT = 4;
#define FRAME_TO_TOPHAT = 5;

class MorphologicalOperation {

    private:
        cv::Mat kernel;

    public:
        MorphologicalOperation(int = 37, int = 37);
        
        cv::Mat applyErosion(cv::Mat);
        cv::Mat applyDilatation(cv::Mat);
        cv::Mat applyTopHat(cv::Mat);
        cv::Mat applyBlackHat(cv::Mat);
        cv::Mat applyEquation(cv::Mat);

        void createKernel(int);
};

class Frame : public VideoCapture {
    private:
        cv::Mat frame;

    public:
        Frame(string path);
        Frame(int cam);
        bool nextFrame(bool = false, int = 1);
        void flipFrame(int = 1);

        cv::Mat getFrame();
        void setFrame(cv::Mat frame);

};

class Camera : public Frame {
    private:
        int kernelSize = 3;
        int channel1;
        int channel2;
        int channel3;
        
        MorphologicalOperation mo;
        
        // Color space
        void applyRGB();
        void applyHSV();
        void applyBGR();
        void applyYCbCr();
        void applyLab();

        // Filter
        void applyMedianBlur();
        void applyGaussianBlur();
        
        // Edge detector
        void applyCanny();
        void applySobel();
        void applyLaplacian();

    public:
        int width;
        int height;

        Camera();
        void applyColorSpace(int color);
        void applyFilter(int filter);
        void applyEdgeDetector(int detector);
        void applyMorphologicalOperation(int operation);
};

class MatRender : public QWidget{
    private:
        // QGroupBox *boxImage;
        QVBoxLayout *layout;
        QLabel *titleLabel;
        QLabel *frameLabel;

        cv::Mat image;

        double ws;
        double hs;

    public:
        bool busy = false;
        
        MatRender(string title,
                    QWidget *parent);
        void render(cv::Mat, int w, double percent);
        void setTitle(string title);
};

class MergeFrameRender : public MatRender {

    private:
        Frame *video;
        Camera *camera;
        cv::Mat result;
        cv::Mat videoRS;

    public:
        MergeFrameRender(string title, QWidget *parent);
        MergeFrameRender(Frame *video, Camera *camera, string title,
                    QWidget *parent);
        void merge();

        cv::Mat getResult();
        void setVideo(Frame *video);
        void setCamera(Camera *camera);
        // void se


};

class MainImageGUI : public QMainWindow {

    protected:
        void resizeEvent(QResizeEvent *event) override;
        void closeEvent (QCloseEvent *event) override;

    private slots:
        void handleButton();
        void handleVideoChooserButton();
    
    private:
        const int NS_PER_SECOND = 1000000000;
        const int UPS_OBJECT = 20;
        const double NS_PER_UPDATES = NS_PER_SECOND / UPS_OBJECT;

        bool runningVideo = false;
        string pathToVideo;

        // MergeCameraVideo result;
        Camera *camera;
        Frame *video;
        MatRender *cameraRender;
        MatRender *videoRender;
        MergeFrameRender *mergeFrameRender;

        std::vector<std::thread > thread_pool;

        QGridLayout *layout;
        QGridLayout *resultLayout;
        QWidget *widget;
        QWidget *resultWidget;
        QLineEdit *kernelSize;
        QLineEdit *videoPath;
        QGroupBox *imageBox;

        QPushButton *kernelButton;
        QPushButton *imageChooserButton;

        SliderGroup *channel1MinSlider;
        SliderGroup *channel2MinSlider;
        SliderGroup *channel3MinSlider;

        SliderGroup *channel1MaxSlider;
        SliderGroup *channel2MaxSlider;
        SliderGroup *channel3MaxSlider;

        const string APP_NAME = "CAVO";
        bool running = true;

        void startProcessConverter();
        double getTimeInMilliseconds();
        void loadVideo();
        void readCameraAndRender();
        long nanoTime();

        void mergeVideoCamera();

    public:
        int w;
        MainImageGUI();
        int init();

        void addMatToWidget(MatRender *render, cv::Mat, double percent=0.3);
        void displayImage(string, cv::Mat);
        void startProcess();

        void clearLayout(QLayout *layout);
};