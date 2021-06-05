#include "../header_files/header.hpp"

MainImageGUI::MainImageGUI(){
    
}

int MainImageGUI::init(){
    this->resize(1500, 900);
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
    this->setWindowTitle(QString::fromStdString(APP_NAME));
    widget = new QWidget(this);
    layout = new QGridLayout(widget);

    this->setCentralWidget(widget);
    widget->setLayout(layout);

    // --- ROW 0, 0

    QGroupBox *boxImage = new QGroupBox("Video", widget);
    boxImage->setFixedHeight(100);
    layout->addWidget(boxImage, 0, 0, 1, 2);
    QVBoxLayout *boxLayoutImage = new QVBoxLayout(boxImage);
    QWidget* imageChooserWidget = new QWidget(boxImage);
    boxLayoutImage->addWidget(imageChooserWidget);

    QHBoxLayout *videoChooserLayout = new QHBoxLayout(imageChooserWidget);
    QLabel *dirLabel = new QLabel("Directorio:");
    videoChooserLayout->addWidget(dirLabel);
    videoPath = new QLineEdit();
    videoPath->setReadOnly(true);
    videoChooserLayout->addWidget(videoPath);
    
    imageChooserButton = new QPushButton("Seleccionar Video", imageChooserWidget);
    connect(imageChooserButton, &QPushButton::released,this, &MainImageGUI::handleVideoChooserButton);
    videoChooserLayout->addWidget(imageChooserButton);

    // --- ROW 1, 0
    QGroupBox *channelBox = new QGroupBox("Canales", widget);
    channelBox->setMaximumWidth(300);
    channelBox->setMaximumHeight(600);
    // channelBox->setSizePolicy
    layout->addWidget(channelBox, 1, 0, 2, 1);

    QVBoxLayout * colorSpaceLayout = new QVBoxLayout(channelBox);

    QWidget * kernelWidget = new QWidget(channelBox);
    colorSpaceLayout->addWidget(kernelWidget);
    QHBoxLayout * kernelLayout = new QHBoxLayout(kernelWidget);


    kernelSizeBox = new QSpinBox(kernelWidget);
    kernelSizeBox->setMinimum(3);
    kernelSizeBox->setSingleStep(2);
    kernelSizeBox->setPrefix("Kernel: ");
    kernelLayout->addWidget(kernelSizeBox);

    kernelButton = new QPushButton("Aplicar", kernelWidget);
    connect(kernelButton, &QPushButton::released,this, &MainImageGUI::handleKernelButton);
    kernelLayout->addWidget(kernelButton);

    colorSpaceCbox = new QComboBox(channelBox);
    colorSpaceCbox->addItems({"RGB", "HSV", "BGR", "YCbCr", "Lab"});
    colorSpaceLayout->addWidget(new QLabel("Espacios de Color:"));
    colorSpaceLayout->addWidget(colorSpaceCbox);
    
    connect(colorSpaceCbox, QOverload<int>::of(&QComboBox::activated),
            this, &MainImageGUI::handleSpaceColor);

    QWidget * allChannelWidget = new QWidget(channelBox);
    colorSpaceLayout->addWidget(allChannelWidget);

    QHBoxLayout *allChannelLayout = new QHBoxLayout(allChannelWidget);

    // ------------- 

    QGroupBox *box = new QGroupBox("Mínimos", channelBox);
    allChannelLayout->addWidget(box);
    QHBoxLayout *boxLayout = new QHBoxLayout(box);
    QWidget* channelWidget = new QWidget(box);
    boxLayout->addWidget(channelWidget);

    QVBoxLayout *channelLayout = new QVBoxLayout(channelWidget);

    channel1MinSlider = new SliderGroup(Qt::Horizontal, tr("R"), channelWidget);
    channel1MinSlider->setMaximum(255);
    QStackedWidget *stackedWidget = new QStackedWidget;
    stackedWidget->addWidget(channel1MinSlider);
    channelLayout->addWidget(stackedWidget);

    channel2MinSlider = new SliderGroup(Qt::Horizontal, tr("G"), channelWidget);
    channel2MinSlider->setMaximum(255);
    stackedWidget = new QStackedWidget;
    stackedWidget->addWidget(channel2MinSlider);
    channelLayout->addWidget(stackedWidget);

    channel3MinSlider = new SliderGroup(Qt::Horizontal, tr("B"), channelWidget);
    channel3MinSlider->setMaximum(255);
    stackedWidget = new QStackedWidget;
    stackedWidget->addWidget(channel3MinSlider);
    channelLayout->addWidget(stackedWidget);
    

    // ----------

    box = new QGroupBox("Máximos", channelBox);
    allChannelLayout->addWidget(box);
    boxLayout = new QHBoxLayout(box);
    channelWidget = new QWidget(box);
    boxLayout->addWidget(channelWidget);

    channelLayout = new QVBoxLayout(channelWidget);
    
    channel1MaxSlider = new SliderGroup(Qt::Horizontal, tr("R"), channelWidget);
    channel1MaxSlider->setMaximum(255);
    stackedWidget = new QStackedWidget;
    stackedWidget->addWidget(channel1MaxSlider);
    channelLayout->addWidget(stackedWidget);

    channel2MaxSlider = new SliderGroup(Qt::Horizontal, tr("G"), channelWidget);
    channel2MaxSlider->setMaximum(255);
    stackedWidget = new QStackedWidget;
    stackedWidget->addWidget(channel2MaxSlider);
    channelLayout->addWidget(stackedWidget);

    channel3MaxSlider = new SliderGroup(Qt::Horizontal, tr("B"), channelWidget);
    channel3MaxSlider->setMaximum(255);
    stackedWidget = new QStackedWidget;
    stackedWidget->addWidget(channel3MaxSlider);
    channelLayout->addWidget(stackedWidget);

    // --- ROW 1, 1

    QGroupBox *allFilterBox = new QGroupBox("Filtros", widget);
    allFilterBox->setMaximumHeight(100);
    layout->addWidget(allFilterBox, 1, 1, 1, 1);
    QHBoxLayout *allFilterLayout = new QHBoxLayout(allFilterBox);

    QWidget *filterWidget = new QWidget(allFilterBox);
    allFilterLayout->addWidget(filterWidget);
    QVBoxLayout *filterLayout = new QVBoxLayout(filterWidget);

    filterLayout->addWidget(new QLabel("Filtro:"));
    filterCbox = new QComboBox(filterWidget);
    filterCbox->addItems({"Ninguno", "Median Blur", "Gaussian Blur"});
    filterLayout->addWidget(filterCbox);
    connect(filterCbox, QOverload<int>::of(&QComboBox::activated),
            this, &MainImageGUI::handleFilter);

    filterWidget = new QWidget(allFilterBox);
    allFilterLayout->addWidget(filterWidget);
    filterLayout = new QVBoxLayout(filterWidget);

    filterLayout->addWidget(new QLabel("Detección de Bordes:"));
    edgeDetectorCbox = new QComboBox(filterWidget);
    edgeDetectorCbox->addItems({"Ninguno", "Canny", "Sobel", "Laplacian"});
    filterLayout->addWidget(edgeDetectorCbox);
    connect(edgeDetectorCbox, QOverload<int>::of(&QComboBox::activated),
            this, &MainImageGUI::handleEdgeDetector);

    filterWidget = new QWidget(allFilterBox);
    allFilterLayout->addWidget(filterWidget);
    filterLayout = new QVBoxLayout(filterWidget);

    filterLayout->addWidget(new QLabel("Operación Morfológica:"));
    morphologicalOperationCbox = new QComboBox(filterWidget);
    morphologicalOperationCbox->addItems({"Ninguno", "Dilatación", "Erosión", "Abierto", "Cerrado", "Black Hat", "Top Hat", "Ecuación"});
    filterLayout->addWidget(morphologicalOperationCbox);
    connect(morphologicalOperationCbox, QOverload<int>::of(&QComboBox::activated),
            this, &MainImageGUI::handleMorphologicOperation);

    // --- ROW 2, 1

    imageBox = new QGroupBox("Resultado", widget);
    QScrollArea* scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(imageBox);
    layout->addWidget(scrollArea, 2, 1, 1, 1);

    resultLayout = new QGridLayout(imageBox);
    resultWidget = new QWidget(imageBox);

    // AUX
    w = imageBox->width();
    // cout << "w:" << w << endl;
    resultLayout->addWidget(resultWidget);

    cameraRender = new MatRender("Cámara", w,  resultWidget);
    resultLayout->addWidget(cameraRender, 0, 0, 1, 1);

    videoRender = new MatRender("Video", w, resultWidget);
    resultLayout->addWidget(videoRender, 1, 0, 1, 1);

    mergeFrameRender = new MergeFrameRender("Seleccione un Video", w, resultWidget);
    resultLayout->addWidget(mergeFrameRender, 0, 1, 2, 1);

    layout->setRowStretch(0, 1);
    layout->setRowStretch(1, 5);
    layout->setColumnStretch(0, 1);
    layout->setColumnStretch(1, 2);

    this->show();
    w = imageBox->width();
    cameraRender->setW(w);
    videoRender->setW(w);
    mergeFrameRender->setW(w);
    
    video = NULL;
    startProcess();
    return 0;
}

void MainImageGUI::addMatToWidget(MatRender *render, cv::Mat image, double percent){
    try {
        
        // std::lock_guard<std::mutex> guard(frame_mutex);
        // int w = this->w;
        // if (!render->busy) {
            
            // render->setW(w);
            render->render(image, percent);
            // render->busy = false;
        // }
    } catch(exception&){
        cout << "LOG: Render Busy on Rendering" << endl;
        render->busy = false;
    }
}

void MainImageGUI::handleKernelButton(){
    try {
        // MergeFrameRender->op
        int size = kernelSizeBox->value();
        mergeFrameRender->setKernelSize(size);
    }catch (int e) {
        if (e == 20) {
            QMessageBox messageBox;
            messageBox.critical(0,"Error", "Seleccione una imagen");
            messageBox.setFixedSize(500,200);
        } else {
            QMessageBox messageBox;
            messageBox.critical(0,"Error", "El valor del Kernel tiene que ser impar y mayor a cero");
            messageBox.setFixedSize(500,200);
        }
    }catch (exception&){
        QMessageBox messageBox;
        messageBox.critical(0,"Error", "Error al procesar la imagen");
        messageBox.setFixedSize(500,200);
    }
}

void MainImageGUI::handleVideoChooserButton(){
    videoPath->setText(QFileDialog::getOpenFileName(this,
        tr("Seleccione un Video"), "../", tr("Video Files (*.mp4 *.mkv)")));
    pathToVideo = videoPath->text().toStdString();
    loadVideo();
}

void MainImageGUI::loadVideo(){
    runningVideo = false;
    if (!pathToVideo.empty()) {
        delete video;
        video = NULL;    
        video = new Frame(pathToVideo);
        mergeVideoCamera();
    }
    runningVideo = true;
}

void MainImageGUI::clearLayout(QLayout *layout) {
    QLayoutItem *child;
    while (layout->count() != 0) {
        child = layout->takeAt ( 0 );
        if (child->layout() != 0) {
            clearLayout ( child->layout() );
        } else if ( child->widget() != 0 ) {
            delete child->widget();
        }
        delete child;
    }
}

void MainImageGUI::closeEvent (QCloseEvent *event){
    QMessageBox::StandardButton resBtn = QMessageBox::question( this, QString::fromStdString(APP_NAME),
                                                                tr("Are you sure?\n"),
                                                                QMessageBox::No | QMessageBox::Yes,
                                                                QMessageBox::Yes);
    if (resBtn != QMessageBox::Yes) {
        event->ignore();
    } else {
        stopProcess();
        event->accept();
    }
}

void MainImageGUI::readCameraAndRender(){
    camera = new Camera(1);
    mergeFrameRender->setCamera(camera);

    bool firstFrame = true;
    Mat cameraShow;
    while(running){
        try {
            if(camera->nextFrame(true)){
                cameraShow = camera->getFrame().clone();
                mergeFrameRender->applyColorSpace(cameraShow, FRAME_TO_RGB);
                if (firstFrame) {
                    camera->width = cameraShow.cols;
                    camera->height = cameraShow.rows;
                    firstFrame = false;
                }
                //
                // w = imageBox->width();
                //
                // std::lock_guard<std::mutex> guard(frame_mutex);
                addMatToWidget(cameraRender, cameraShow);
            }
        
        } catch (exception&){
            cout << "LOG: Camera Render is busy" << endl;
        }
    }
}

long MainImageGUI::nanoTime(){
    std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch(); 
    return std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();

}

void MainImageGUI::startProcessConverter(){

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
                if (video != NULL && runningVideo) {
                    if (video->nextFrame()) {
                            setChannelValues();
                            mergeFrameRender->applyColorSpace(video->getFrame(), FRAME_TO_RGB);

                            mergeFrameRender->applyMorphologicalOperation(camera->getFrame());
                            mergeFrameRender->merge();
                            addMatToWidget(mergeFrameRender, mergeFrameRender->getResult(), 0.6);
                            addMatToWidget(videoRender, video->getFrame());
                    } else {
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

void MainImageGUI::mergeVideoCamera(){
    mergeFrameRender->setTitle("Croma Aplicado");
    mergeFrameRender->setVideo(video);
}

void MainImageGUI::setChannelValues(){
    int c1Min = channel1MinSlider->getValue();
    int c2Min = channel2MinSlider->getValue();
    int c3Min = channel3MinSlider->getValue();

    int c1Max = channel1MaxSlider->getValue();
    int c2Max = channel2MaxSlider->getValue();
    int c3Max = channel3MaxSlider->getValue();

    mergeFrameRender->setMinimunChannelValues(c1Min, c2Min, c3Min);
    mergeFrameRender->setMaximunChannelValues(c1Max, c2Max, c3Max);    
}

// HANDLE
void MainImageGUI::handleSpaceColor(int i){
    // "RGB", "HSV", "BGR", "YCbCr", "Lab"
    switch (i) {
    case 0:
        changeChannelValues(0, 255, "R", channel1MinSlider);
        changeChannelValues(0, 255, "G", channel2MinSlider);
        changeChannelValues(0, 255, "B", channel3MinSlider);

        changeChannelValues(0, 255, "R", channel1MaxSlider);
        changeChannelValues(0, 255, "G", channel2MaxSlider);
        changeChannelValues(0, 255, "B", channel3MaxSlider);
        break;
    case 1:
        changeChannelValues(0, 180, "H", channel1MinSlider);
        changeChannelValues(0, 255, "S", channel2MinSlider);
        changeChannelValues(0, 255, "V", channel3MinSlider);

        changeChannelValues(0, 180, "H", channel1MaxSlider);
        changeChannelValues(0, 255, "S", channel2MaxSlider);
        changeChannelValues(0, 255, "V", channel3MaxSlider);
        break;
    case 2:
        changeChannelValues(0, 255, "B", channel1MinSlider);
        changeChannelValues(0, 255, "G", channel2MinSlider);
        changeChannelValues(0, 255, "R", channel3MinSlider);

        changeChannelValues(0, 255, "B", channel1MaxSlider);
        changeChannelValues(0, 255, "G", channel2MaxSlider);
        changeChannelValues(0, 255, "R", channel3MaxSlider);
        break;
    case 3:
        changeChannelValues(0, 255, "Y", channel1MinSlider);
        changeChannelValues(0, 255, "Cb", channel2MinSlider);
        changeChannelValues(0, 255, "Cr", channel3MinSlider);

        changeChannelValues(0, 255, "Y", channel1MaxSlider);
        changeChannelValues(0, 255, "Cb", channel2MaxSlider);
        changeChannelValues(0, 255, "Cr", channel3MaxSlider);
        break;

    case 4:
        changeChannelValues(0, 100, "L", channel1MinSlider);
        changeChannelValues(-127, 127, "a", channel2MinSlider);
        changeChannelValues(-127, 127, "b", channel3MinSlider);

        changeChannelValues(0, 100, "L", channel1MaxSlider);
        changeChannelValues(-127, 127, "a", channel2MaxSlider);
        changeChannelValues(-127, 127, "b", channel3MaxSlider);
        break;
    
    default:
        break;
    }
    changeColorSpace();
}

void MainImageGUI::changeChannelValues(int min, int max, string title, SliderGroup * slider){
    slider->setValue(0);
    slider->setMinimum(min);
    slider->setMaximum(max);
    slider->setTitleToBox(title);
}

void MainImageGUI::changeColorSpace(){
    // "RGB", "HSV", "BGR", "YCbCr", "Lab"
    int i = colorSpaceCbox->currentIndex();
    switch (i) {
    case 0:
        mergeFrameRender->setColorSpace(FRAME_TO_RGB);
        break;
    case 1:
        mergeFrameRender->setColorSpace(FRAME_TO_HSV);
        break;
    case 2:
        mergeFrameRender->setColorSpace(FRAME_TO_BGR);
        break;
    case 3:
        mergeFrameRender->setColorSpace(FRAME_TO_YCbCr);
        break;

    case 4:
        mergeFrameRender->setColorSpace(FRAME_TO_Lab);
        break;
    
    default:
        break;
    }
}

void MainImageGUI::handleFilter(int i){
    // "Median Blur", "Gaussian Blur"
    // int i = colorSpaceCbox->currentIndex();
    switch (i) {
    case 0:
        mergeFrameRender->setFilter(-1);
        break;
    case 1:
        mergeFrameRender->setFilter(FRAME_TO_MEDBLUR);
        break;
    case 2:
        mergeFrameRender->setFilter(FRAME_TO_GAUBLUR);
        break;

    default:
        break;
    }
}

void MainImageGUI::handleEdgeDetector(int i){
    // "Canny", "Sobel", "Laplacian"
    // int i = colorSpaceCbox->currentIndex();
    switch (i) {
    case 0:
        mergeFrameRender->setEdgeDetector(-1);
        break;
    case 1:
        mergeFrameRender->setEdgeDetector(FRAME_TO_CANNY);
        break;
    case 2:
        mergeFrameRender->setEdgeDetector(FRAME_TO_SOBEL);
        break;
    case 3:
        mergeFrameRender->setEdgeDetector(FRAME_TO_LAPLACIAN);
        break;

    default:
        break;
    }
}

void MainImageGUI::handleMorphologicOperation(int i){
    // "Dilatación", "Erosión", "Abierto", "Cerrado", "Black Hat", "Top Hat

    switch (i) {
    case 0:
        mergeFrameRender->setMorphologicalOperation(-1);
        break;
    case 1:
        mergeFrameRender->setMorphologicalOperation(FRAME_TO_DILATATION);
        break;
    case 2:
        mergeFrameRender->setMorphologicalOperation(FRAME_TO_EROSION);
        break;
    case 3:
        mergeFrameRender->setMorphologicalOperation(FRAME_TO_OPEN);
        break;
    case 4:
        mergeFrameRender->setMorphologicalOperation(FRAME_TO_CLOSE);
        break;

    case 5:
        mergeFrameRender->setMorphologicalOperation(FRAME_TO_BLACKHAT);
        break;
    case 6:
        mergeFrameRender->setMorphologicalOperation(FRAME_TO_TOPHAT);
        break;
    case 7:
        mergeFrameRender->setMorphologicalOperation(FRAME_TO_EQUATION);
        break;

    default:
        break;
    }
}

void MainImageGUI::stopProcess(){
    running = false;
    for (int i = 0; i < thread_pool.size(); i++){
        thread_pool[i].join();
    }
}

void MainImageGUI::startProcess(){
    thread_pool.clear();
    running = true;
    thread_pool.push_back(std::thread(&MainImageGUI::readCameraAndRender, this));
    thread_pool.push_back(std::thread(&MainImageGUI::startProcessConverter, this));
    
}