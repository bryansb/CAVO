#ifndef ChromaThread_HEADER
#define ChromaThread_HEADER

// #include "camera.hpp"
#include "chroma_render_controller.hpp"
#include "base.hpp"

#include <QThread>
#include <QGroupBox>

class ChromaThread : public QThread {
    
    // Q_OBJECT

    private:

        const string RENDER_RESULTS_NAMES[4] = {"Croma aplicado", "Umbralización", "Umbralización Inversa", "Fusión del Video"};

        const int NS_PER_SECOND = 1000000000;
        const int UPS_OBJECT = 22;
        const double NS_PER_UPDATES = NS_PER_SECOND / UPS_OBJECT;

        Frame *video;
        Camera *camera;

        ChromaRenderController *chromaRC;
        MatRender *videoRender;

        QGroupBox *box;

        long nanoTime();

        void showRender();

    public:
        bool running = false;

        int selectResultIndex = 0;
        
        ChromaThread();
        void run();

        void setChromaRenderController(ChromaRenderController *chromaRC);
        void setVideoRender(MatRender *videoRender);

        void setVideo(Frame *video);
        void setCamera(Camera *camera);

        void setBox(QGroupBox *);

        string path;

        Frame * loadVideo();

    
    // signals:
    //     void videoFinished(bool finished);

        
    
};

class CameraThread : public QThread {
    private:
        Camera *camera;
        MatRender *cameraRender;

        QGroupBox *box;

    public:
        bool running = false;
        
        CameraThread();
        void run();

        void setCamera(Camera *Camera);
        void setCameraRender(MatRender *cameraRender);

        void setBox(QGroupBox *);
    
};

class ThreadRenderController {
    // Q_OBJECT
    private:
        ChromaThread *chromaThread;
        CameraThread *cameraThread;

        vector<QThread> qthread_pool;

    public:
        ThreadRenderController(bool twoThread);

        ThreadRenderController();

        // void start(Camera *, ChromaRenderController *);
        void startCamera(Camera *camera, MatRender *cameraRender, QGroupBox * box);
        void startChromaRenderController(ChromaRenderController *, MatRender *videoRender, QGroupBox * box);
        void stop();

        void loadNewVideo(Frame *video);

        Frame * loadNewVideo(string path);

        void setSelectResultIndex(int);
};

#endif