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

using namespace std;
using namespace cv;

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