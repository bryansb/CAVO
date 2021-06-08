
#include "../../../../../header_files/chroma_thread.hpp"
#include <QDebug>

CameraThread::CameraThread() {
}

void CameraThread::run(){
    bool firstFrame = true;
    Mat cameraShow;
    while(running){
        try {
            if(camera->nextFrame(true)){
                cameraShow = camera->getFrame().clone();
                cv::cvtColor(cameraShow, cameraShow, cv::COLOR_BGR2RGB);
                if (firstFrame) {
                    camera->width = cameraShow.cols;
                    camera->height = cameraShow.rows;
                    firstFrame = false;
                }

                // cameraRender->setWidth(imageBox->width());
                cameraRender->render(cameraShow, 0.3);
            }
        
        } catch (exception&){
            cout << "LOG: Camera Render is busy" << endl;
        }
    }
}

void CameraThread::setCamera(Camera *camera){
    this->camera = camera;
}

void CameraThread::setCameraRender(MatRender *cameraRender){    
    this->cameraRender = cameraRender;
}

// **********************************************

ChromaThread::ChromaThread(){
}

long ChromaThread::nanoTime(){
    std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch(); 
    return std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();

}

Frame * ChromaThread::loadVideo(){
    this->video = new Frame(path);
    chromaRC->setVideo(video);
    return video;
}

void ChromaThread::run(){
    int aps = 0;
    int fps = 0;
    int ups = 0;

    long updateReference = nanoTime();
    long countReference = nanoTime();

    double timeElapsed;
    double delta = 0;

    long loopStart;

    while(running){
        loopStart = nanoTime();
        timeElapsed = loopStart - updateReference;
        updateReference = loopStart;

        delta += timeElapsed / NS_PER_UPDATES;

        while (delta >= 1) {
            try {
                if (video != NULL) {
                    if (video->nextFrame()) {
                            cv::cvtColor(video->getFrame(), video->getFrame(), cv::COLOR_BGR2RGB);
                            // cout << chromaRC != NULL << endl;
                            // chromaRC->applyMorphologicalOperation(chromaRC->getCamera());
                            chromaRC->merge();

                            // showRender();
                            
                            // videoRender->setWidth(imageBox->width());
                            // chromaRenderController->setWidth(imageBox->width());
                            // addMatToWidget(videoRender, video->getFrame());
                            videoRender->render(video->getFrame(), 0.3);
                            showRender();
                            // chromaRenderController->render(chromaRenderController->get, 0.6);
                    } else {
                        // emit videoFinished(true);
                        loadVideo();
                    }
                
                }
            }catch (exception&){
                cout << "LOG: Processing video was busy" << endl;
            }
            ups++;
            delta--;
            fps++;
        }

        if (nanoTime() - countReference > NS_PER_SECOND) {
            // cout << "FPS:" << fps << endl;
            ups = 0;
            fps = 0;
            countReference = nanoTime();
        }
    }
}

void ChromaThread::showRender(){
    switch (selectResultIndex)
    {
    case 0:
        chromaRC->setTitle(RENDER_RESULTS_NAMES[selectResultIndex]);
        chromaRC->render(chromaRC->getResult(), 0.6);
        break;
    case 1:
        chromaRC->setTitle(RENDER_RESULTS_NAMES[selectResultIndex]);
        chromaRC->render(chromaRC->getCameraThreshold(), 0.6);
        // addMatToWidget(chromaRenderController, chromaRenderController->getCameraThreshold(), 0.6, RENDER_RESULTS_NAMES[selectResultIndex]);
        break;
    case 2:
        chromaRC->setTitle(RENDER_RESULTS_NAMES[selectResultIndex]);
        chromaRC->render(chromaRC->getCameraThresholdN(), 0.6);
        // addMatToWidget(chromaRenderController, chromaRenderController->getCameraThresholdN(), 0.6, RENDER_RESULTS_NAMES[selectResultIndex]);
        break;
    case 3:
        chromaRC->setTitle(RENDER_RESULTS_NAMES[selectResultIndex]);
        chromaRC->render(chromaRC->getVideoFusionBackground(), 0.6);
        // addMatToWidget(chromaRenderController, chromaRenderController->getVideoFusionBackground(), 0.6, RENDER_RESULTS_NAMES[selectResultIndex]);
        break;
    default:
        break;
    }
}

void ChromaThread::setChromaRenderController(ChromaRenderController *chromaRC){
    this->chromaRC = chromaRC;
}

void ChromaThread::setVideoRender(MatRender *videoRender){
    this->videoRender = videoRender;
}

void ChromaThread::setVideo(Frame *video){
    this->video = video;
}

void ChromaThread::setCamera(Camera *camera){
    this->camera = camera;
}


// **********************************************

ThreadRenderController::ThreadRenderController(bool){

}

ThreadRenderController::ThreadRenderController(){
    this->cameraThread = new CameraThread();
    this->chromaThread = new ChromaThread();
}

void ThreadRenderController::startCamera(Camera *camera, MatRender *cameraRender){
    this->cameraThread->setCamera(camera);
    this->cameraThread->setCameraRender(cameraRender);
    cameraThread->running = true;
    cameraThread->start();
}

void ThreadRenderController::startChromaRenderController(ChromaRenderController *chromaRC, MatRender *videoRender){
    this->chromaThread->setChromaRenderController(chromaRC);
    this->chromaThread->setVideoRender(videoRender);
    // this->chromaThread->setCamera(camera);
    // chromaThread->running = true;
    // chromaThread->start();
}

void ThreadRenderController::loadNewVideo(Frame *video){
    chromaThread->running = false;
    chromaThread->wait();
    chromaThread->setVideo(video);
    chromaThread->running = true;
    chromaThread->start();
}

Frame * ThreadRenderController::loadNewVideo(string path){
    chromaThread->running = false;
    chromaThread->wait();
    chromaThread->path = path;
    Frame *video = chromaThread->loadVideo();
    chromaThread->running = true;
    chromaThread->start();
    return video;
}

void ThreadRenderController::stop(){
    cameraThread->running  = false;
    chromaThread->running  = false;
    cameraThread->wait();
    chromaThread->wait();
}

void ThreadRenderController::setSelectResultIndex(int index) {
    chromaThread->selectResultIndex = index;
}