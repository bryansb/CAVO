#include <iostream>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <cmath>
#include <string>
#include <thread>
#include <chrono>
#include <mutex>

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
#include <QComboBox>
#include <QSpinBox>

#include "../header_files/slider-group.hpp"


using namespace std;
using namespace cv;
using namespace std::chrono;

// Color space
#define FRAME_TO_RGB 0
#define FRAME_TO_HSV 1
#define FRAME_TO_BGR 2
#define FRAME_TO_YCbCr 3
#define FRAME_TO_Lab 4

// Filter
#define FRAME_TO_MEDBLUR 0
#define FRAME_TO_GAUBLUR 1

// Edge detector
#define FRAME_TO_CANNY 0
#define FRAME_TO_SOBEL 1
#define FRAME_TO_LAPLACIAN 2

// Morphological Operations
#define FRAME_TO_DILATATION 0
#define FRAME_TO_EROSION 1
#define FRAME_TO_OPEN 2
#define FRAME_TO_CLOSE 3
#define FRAME_TO_BLACKHAT 4
#define FRAME_TO_TOPHAT 5
#define FRAME_TO_EQUATION 6

class MorphologicalOperation {

    private:
        cv::Mat kernel;

    public:
        MorphologicalOperation(int = 3, int = 3);
        
        cv::Mat applyErosion(cv::Mat &);
        cv::Mat applyDilatation(cv::Mat);
        cv::Mat applyOpen(cv::Mat);
        cv::Mat applyClose(cv::Mat);
        cv::Mat applyTopHat(cv::Mat);
        cv::Mat applyBlackHat(cv::Mat);
        cv::Mat applyEquation(cv::Mat);

        void createKernel(int);
};

class Frame : public VideoCapture {
    private:
        cv::Mat frame;

        std::mutex frame_mutex;

    public:
        bool busy = false;
        Frame(string path);
        Frame(int cam);
        bool nextFrame(bool = false, int = 1);
        void flipFrame(cv::Mat frame, int orientation = 1);

        cv::Mat getFrame();
        void setFrame(cv::Mat frame);

};

class Camera : public Frame {

    public:
        int width;
        int height;

        Camera(int);
        
};

class MatRender : public QWidget{
    private:
        QVBoxLayout *layout;
        QLabel *titleLabel;
        QLabel *frameLabel;
        QImage qimage;

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

        void setW(int w);
};

class ChromaRenderController : public MatRender {

    private:
        Frame *video;
        Camera *camera;
        
        cv::Mat cameraOriginal;
        cv::Mat cameraFiltered;
        cv::Mat videoFusionBackground;
        cv::Mat cameraThreshold;
        cv::Mat cameraThresholdN;
        cv::Mat result;
        cv::Mat videoRS;

        int filterKernelSize = 3;
        int edgeKernelSize = 3;
        int mOpKernelSize = 3;

        int threshhold = 70;

        int channel1Min;
        int channel2Min;
        int channel3Min;

        int channel1Max;
        int channel2Max;
        int channel3Max;

        int colorSpace = 0;
        int filter = -1;
        int edgeDetector = -1;
        int morphologicalOperation = -1;
        
        MorphologicalOperation mo;

        std::mutex frame_mutex;
        
        // Color space
        cv::Mat applyRGB(cv::Mat frame);
        cv::Mat applyHSV(cv::Mat frame);
        cv::Mat applyBGR(cv::Mat frame);
        cv::Mat applyYCbCr(cv::Mat frame);
        cv::Mat applyLab(cv::Mat frame);

        // Filter
        cv::Mat applyMedianBlur(cv::Mat frame);
        cv::Mat applyGaussianBlur(cv::Mat frame);
        
        // Edge detector
        cv::Mat applyCanny(cv::Mat frame);
        cv::Mat applySobel(cv::Mat frame);
        cv::Mat applyLaplacian(cv::Mat frame);

        // Morphological Operation
        // in other class

        // Threshold
        cv::Mat makeBinaryThresholding(cv::Mat frame);

        // Pixel Detection
        void applyChromaEffect(cv::Mat camera, cv::Mat video, cv::Mat thresholdized, cv::Mat &output);

    public:
        ChromaRenderController(string title, int w, QWidget *parent);
        ChromaRenderController(Frame *video, Camera *camera, string title,
                    int w, QWidget *parent);
        void merge();

        cv::Mat getResult();
        void setVideo(Frame *video);
        void setCamera(Camera *camera);

        // Apply transform
        void applyColorSpace(cv::Mat, int color);
        void applyFilter(cv::Mat);
        void applyEdgeDetector(cv::Mat);
        void applyMorphologicalOperation(cv::Mat);

        void setColorSpace(int);
        void setFilter(int);
        void setEdgeDetector(int);
        void setMorphologicalOperation(int);
        void setMinimunChannelValues(int, int, int);
        void setMaximunChannelValues(int, int, int);

        void setFilterKernelSize(int filterKernelSize);
        void setEdgeKernelSize(int edgeKernelSize);
        void setMOpKernelSize(int mOpKernelSize);

        void setThreshhold(int threshhold);

        cv::Mat getCameraThreshold();
        cv::Mat getVideoFusionBackground();
        cv::Mat getCameraThresholdN();
};

class MainImageGUI : public QMainWindow {

    protected:
        void closeEvent (QCloseEvent *event) override;

    private slots:
        void handleKernelButton();
        void handleVideoChooserButton();
        void handleKernelSize(int);

        void handleSpaceColor(int);
        void handleFilter(int);
        void handleEdgeDetector(int);
        void handleMorphologicOperation(int);
    
    private:
        const int NS_PER_SECOND = 1000000000;
        const int UPS_OBJECT = 22;
        const double NS_PER_UPDATES = NS_PER_SECOND / UPS_OBJECT;

        bool runningVideo = false;
        string pathToVideo;

        std::mutex frame_mutex;

        Camera *camera;
        Frame *video;
        MatRender *cameraRender;
        MatRender *videoRender;

        MatRender *videoFusionBackgroundRender;
        MatRender *cameraThresholdRender;
        MatRender *cameraThresholdNRender;

        ChromaRenderController *chromaRenderController;

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

        SliderGroup *cannySlider;

        QComboBox *colorSpaceCbox;
        QComboBox *filterCbox;
        QComboBox *morphologicalOperationCbox;
        QComboBox *edgeDetectorCbox;

        QSpinBox *filterKernelSizeBox;
        QSpinBox *edgeKernelSizeBox;
        QSpinBox *mOpKernelSizeBox;

        const string APP_NAME = "C.A.V.O.";
        bool running = false;
        void startProcessConverter();
        double getTimeInMilliseconds();
        void loadVideo(string);
        void readCameraAndRender();
        long nanoTime();

        void mergeVideoCamera();

        void setChannelValues();

        void changeChannelValues(int min, int max, string title, SliderGroup * slider, bool isMax = false);

        void changeColorSpace();
        void changeFilter();
        void changeEdgeDetector();
        void changeMorpOperation();

        void setKernelValues();
        void showCannySlider(int index);
        
        void stopProcess();
    public:
        
        MainImageGUI();
        int init();

        int w;

        void addMatToWidget(MatRender *render, cv::Mat, double percent=0.3);
        void displayImage(string, cv::Mat);
        void startProcess();

        void clearLayout(QLayout *layout);
};