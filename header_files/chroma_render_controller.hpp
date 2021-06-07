#include "camera.hpp"
#include "mat_render.hpp"

#include "morphological_operation.hpp"

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
        // in another class

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
        cv::Mat applyColorSpace(cv::Mat, int color);
        cv::Mat applyFilter(cv::Mat);
        cv::Mat applyEdgeDetector(cv::Mat);
        cv::Mat applyMorphologicalOperation(cv::Mat);

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

        void setChannel1Min(int);
        void setChannel2Min(int);
        void setChannel3Min(int);

        void setChannel1Max(int);
        void setChannel2Max(int);
        void setChannel3Max(int);

        cv::Mat getCameraThreshold();
        cv::Mat getVideoFusionBackground();
        cv::Mat getCameraThresholdN();
};