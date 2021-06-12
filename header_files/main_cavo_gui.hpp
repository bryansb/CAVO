#include "slider_group.hpp"

#include "chroma_thread.hpp"

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

class MainCavoGUI : public QMainWindow {

    protected:
        void closeEvent (QCloseEvent *event) override;
        void resizeEvent(QResizeEvent *event) override;

    private slots:
        void handleKernelButton();
        void handleVideoChooserButton();
        void handleKernelSize(int);

        void handleSpaceColor(int);
        void handleFilter(int);
        void handleEdgeDetector(int);
        void handleMorphologicOperation(int);
        void handleShowResult(int);

        void handleChannel1Min(int);
        void handleChannel2Min(int);
        void handleChannel3Min(int);

        void handleChannel1Max(int);
        void handleChannel2Max(int);
        void handleChannel3Max(int);

        void handleFilterKernel(int);
        void handleEdgeKernel(int);
        void handleOMorphKernel(int);

        void handleCannyKernel(int);

    private:
        const string APP_NAME = "C.A.V.O.";

        int cameraNumber = 0; // 0: default laptop camera

        bool runningVideo = false;
        string pathToVideo;

        std::mutex frame_mutex;

        Camera *camera;
        MatRender *cameraRender;
        MatRender *videoRender;

        ChromaRenderController *chromaRenderController;

        std::vector<std::thread> thread_pool;
        ThreadRenderController *threadRC;
        

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

        QComboBox *resultsRenderCbox;

        QSpinBox *filterKernelSizeBox;
        QSpinBox *edgeKernelSizeBox;
        QSpinBox *mOpKernelSizeBox;

        int channel1Min = 0;
        int channel2Min = 0;
        int channel3Min = 0;

        int channel1Max = 255;
        int channel2Max = 255;
        int channel3Max = 255;

        int selectResultIndex = 0;

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

        void showCannySlider(int index);
        
        void stopProcess();

        void setRenderWidth();
    public:
        
        MainCavoGUI();
        int init();

        int w;

        void displayImage(string, cv::Mat);
        void startProcess();

        void clearLayout(QLayout *layout);
};