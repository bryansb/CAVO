#include <chrono>

#include "slider_group.hpp"

#include "chroma_controller_render.hpp"

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

// #include "../model/camera.hpp"
// #include "../model/mat-render.hpp"

class MainCavoGUI : public QMainWindow {

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

        void handleChannel1Min(int);
        void handleChannel2Min(int);
        void handleChannel3Min(int);

        void handleChannel1Max(int);
        void handleChannel2Max(int);
        void handleChannel3Max(int);
    
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

        int channel1Min = 0;
        int channel2Min = 0;
        int channel3Min = 0;

        int channel1Max = 255;
        int channel2Max = 255;
        int channel3Max = 255;

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
        
        MainCavoGUI();
        int init();

        int w;

        void addMatToWidget(MatRender *render, cv::Mat, double percent=0.3);
        void displayImage(string, cv::Mat);
        void startProcess();

        void clearLayout(QLayout *layout);
};