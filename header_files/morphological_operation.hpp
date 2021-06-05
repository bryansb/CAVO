#include "base.hpp"

class MorphologicalOperation {

    private:
        cv::Mat kernel;

    public:
        MorphologicalOperation(int = 3, int = 3);
        
        cv::Mat applyErosion(cv::Mat);
        cv::Mat applyDilatation(cv::Mat);
        cv::Mat applyOpen(cv::Mat);
        cv::Mat applyClose(cv::Mat);
        cv::Mat applyTopHat(cv::Mat);
        cv::Mat applyBlackHat(cv::Mat);
        cv::Mat applyEquation(cv::Mat);

        void createKernel(int);
};