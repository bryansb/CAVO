#include "base.hpp"

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