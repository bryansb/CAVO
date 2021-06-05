#include "../../../../../header_files/main-cavo-gui.hpp"

MainCavoGUI::MainCavoGUI(){}

int MainCavoGUI::init(){
    this->setFixedSize(1500, 900);
    // setWindowFlags(Qt::Widget | Qt::MSWindowsFixedSizeDialogHint);
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
    connect(imageChooserButton, &QPushButton::released,this, &MainCavoGUI::handleVideoChooserButton);
    videoChooserLayout->addWidget(imageChooserButton);

    // --- ROW 1, 0
    QGroupBox *channelBox = new QGroupBox("Canales", widget);
    channelBox->setMaximumWidth(300);
    channelBox->setMaximumHeight(800);
    // channelBox->setSizePolicy
    layout->addWidget(channelBox, 1, 0, 2, 1);

    QVBoxLayout * colorSpaceLayout = new QVBoxLayout(channelBox);

    QWidget * kernelWidget = new QWidget(channelBox);
    colorSpaceLayout->addWidget(kernelWidget);
    QHBoxLayout * kernelLayout = new QHBoxLayout(kernelWidget);


    // kernelSizeBox = new QSpinBox(kernelWidget);
    // kernelSizeBox->setMinimum(3);
    // kernelSizeBox->setSingleStep(2);
    // kernelSizeBox->setPrefix("Kernel: ");
    // kernelLayout->addWidget(kernelSizeBox);

    kernelButton = new QPushButton("Refrescar", kernelWidget);
    kernelButton->setStyleSheet("QPushButton {background-color: #3D72A4; color: white; font-size: 14px; font-weight: bold;}");
    kernelButton->setFixedHeight(35);
    kernelButton->setFixedWidth(150);
    kernelLayout->addWidget(kernelButton);

    colorSpaceCbox = new QComboBox(channelBox);
    colorSpaceCbox->addItems({"RGB", "HSV", "BGR", "YCbCr", "Lab"});
    colorSpaceLayout->addWidget(new QLabel("Espacios de Color:"));
    colorSpaceLayout->addWidget(colorSpaceCbox);
    connect(colorSpaceCbox, QOverload<int>::of(&QComboBox::activated),
            this, &MainCavoGUI::handleSpaceColor);
    
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
    channel1MaxSlider->setValue(255);
    stackedWidget = new QStackedWidget;
    stackedWidget->addWidget(channel1MaxSlider);
    channelLayout->addWidget(stackedWidget);

    channel2MaxSlider = new SliderGroup(Qt::Horizontal, tr("G"), channelWidget);
    channel2MaxSlider->setMaximum(255);
    channel2MaxSlider->setValue(255);
    stackedWidget = new QStackedWidget;
    stackedWidget->addWidget(channel2MaxSlider);
    channelLayout->addWidget(stackedWidget);

    channel3MaxSlider = new SliderGroup(Qt::Horizontal, tr("B"), channelWidget);
    channel3MaxSlider->setMaximum(255);
    channel3MaxSlider->setValue(255);
    stackedWidget = new QStackedWidget;
    stackedWidget->addWidget(channel3MaxSlider);
    channelLayout->addWidget(stackedWidget);

    // --- ROW 1, 1

    QGroupBox *allFilterBox = new QGroupBox("Filtros", widget);
    allFilterBox->setMaximumHeight(150);
    layout->addWidget(allFilterBox, 1, 1, 1, 1);
    QHBoxLayout *allFilterLayout = new QHBoxLayout(allFilterBox);
    allFilterLayout->setContentsMargins(2, 2, 2, 2);

    //----
    QWidget *filterWidget = new QWidget(allFilterBox);
    allFilterLayout->addWidget(filterWidget);
    QHBoxLayout *filterLayoutBox = new QHBoxLayout(filterWidget);
    QWidget *kernelFilterWidget = new QWidget(filterWidget);
    filterLayoutBox->addWidget(kernelFilterWidget);
    QVBoxLayout *filterLayout = new QVBoxLayout(kernelFilterWidget);

    QLabel *kernelLabel = new QLabel("Filtro:");
    kernelLabel->setFixedHeight(20);
    filterLayout->addWidget(kernelLabel);

    // SpinBox
    filterKernelSizeBox = new QSpinBox(filterWidget);
    filterKernelSizeBox->setMinimum(3);
    filterKernelSizeBox->setSingleStep(2);
    filterKernelSizeBox->setPrefix("Kernel: ");
    filterLayout->addWidget(filterKernelSizeBox);

    filterCbox = new QComboBox(filterWidget);
    filterCbox->addItems({"Ninguno", "Median Blur", "Gaussian Blur"});
    filterLayout->addWidget(filterCbox);
    connect(filterCbox, QOverload<int>::of(&QComboBox::activated),
            this, &MainCavoGUI::handleFilter);

    //-------

    filterWidget = new QWidget(allFilterBox);
    allFilterLayout->addWidget(filterWidget);
    filterLayoutBox = new QHBoxLayout(filterWidget);
    kernelFilterWidget = new QWidget(filterWidget);
    filterLayoutBox->addWidget(kernelFilterWidget);
    filterLayout = new QVBoxLayout(kernelFilterWidget);

    cannySlider = new SliderGroup(Qt::Horizontal, tr("Threshold"), filterWidget);
    cannySlider->setMaximum(255);
    cannySlider->setVisible(false);
    filterLayoutBox->addWidget(cannySlider);

    kernelLabel = new QLabel("Detección de Bordes:");
    kernelLabel->setFixedHeight(20);
    filterLayout->addWidget(kernelLabel);

    // SpinBox
    edgeKernelSizeBox = new QSpinBox(filterWidget);
    edgeKernelSizeBox->setMinimum(3);
    edgeKernelSizeBox->setSingleStep(2);
    edgeKernelSizeBox->setPrefix("Kernel: ");
    filterLayout->addWidget(edgeKernelSizeBox);

    edgeDetectorCbox = new QComboBox(filterWidget);
    edgeDetectorCbox->addItems({"Ninguno", "Canny", "Sobel", "Laplacian"});
    filterLayout->addWidget(edgeDetectorCbox);
    connect(edgeDetectorCbox, QOverload<int>::of(&QComboBox::activated),
            this, &MainCavoGUI::handleEdgeDetector);

    filterWidget = new QWidget(allFilterBox);
    allFilterLayout->addWidget(filterWidget);
    filterLayout = new QVBoxLayout(filterWidget);

    kernelLabel = new QLabel("Operaciones Morfológicas:");
    kernelLabel->setFixedHeight(20);
    filterLayout->addWidget(kernelLabel);

    // SpinBox
    mOpKernelSizeBox = new QSpinBox(filterWidget);
    mOpKernelSizeBox->setMinimum(3);
    mOpKernelSizeBox->setSingleStep(2);
    mOpKernelSizeBox->setPrefix("Kernel: ");
    filterLayout->addWidget(mOpKernelSizeBox);

    morphologicalOperationCbox = new QComboBox(filterWidget);
    morphologicalOperationCbox->addItems({"Ninguno", "Dilatación", "Erosión", "Abierto", "Cerrado", "Black Hat", "Top Hat", "Ecuación"});
    filterLayout->addWidget(morphologicalOperationCbox);
    connect(morphologicalOperationCbox, QOverload<int>::of(&QComboBox::activated),
            this, &MainCavoGUI::handleMorphologicOperation);

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

    chromaRenderController = new ChromaRenderController("Seleccione un Video", w, resultWidget);
    resultLayout->addWidget(chromaRenderController, 0, 1, 2, 1);

    cameraThresholdRender = new MatRender("Umbralizacón", w, resultWidget);
    resultLayout->addWidget(cameraThresholdRender, 2, 1, 1, 1);

    cameraThresholdNRender = new MatRender("Umbralizacón Inversa", w, resultWidget);
    resultLayout->addWidget(cameraThresholdNRender, 3, 1, 1, 1);

    videoFusionBackgroundRender= new MatRender("Fusión del Video", w, resultWidget);
    resultLayout->addWidget(videoFusionBackgroundRender, 4, 1, 1, 1);

    layout->setRowStretch(0, 1);
    layout->setRowStretch(1, 5);
    layout->setColumnStretch(0, 1);
    layout->setColumnStretch(1, 2);

    this->show();
    w = imageBox->width();
    cameraRender->setW(w);
    videoRender->setW(w);
    chromaRenderController->setW(w);
    cameraThresholdRender->setW(w);
    cameraThresholdNRender->setW(w);
    videoFusionBackgroundRender->setW(w);
    
    video = NULL;
    startProcess();
    return 0;
}

void MainCavoGUI::addMatToWidget(MatRender *render, cv::Mat image, double percent){
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

void MainCavoGUI::handleKernelButton(){
    try {
        // chromaRenderController->op
        // int size = kernelSizeBox->value();
        // chromaRenderController->setKernelSize(size);
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

void MainCavoGUI::handleVideoChooserButton(){
    videoPath->setText(QFileDialog::getOpenFileName(this,
        tr("Seleccione un Video"), "../", tr("Video Files (*.mp4 *.mkv)")));
    loadVideo(videoPath->text().toStdString());
}

void MainCavoGUI::loadVideo(string path){
    runningVideo = false;
    if (!path.empty()) {
        delete video;
        video = NULL;
        pathToVideo = path; 
        video = new Frame(pathToVideo);
        mergeVideoCamera();
    }
    runningVideo = true;
}

void MainCavoGUI::clearLayout(QLayout *layout) {
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

void MainCavoGUI::closeEvent (QCloseEvent *event){
    QMessageBox::StandardButton resBtn = QMessageBox::question( this, QString::fromStdString(APP_NAME),
                                                                tr("¿Está seguro?\n"),
                                                                QMessageBox::No | QMessageBox::Yes,
                                                                QMessageBox::Yes);
    if (resBtn != QMessageBox::Yes) {
        event->ignore();
    } else {
        stopProcess();
        event->accept();
    }
}

void MainCavoGUI::readCameraAndRender(){
    camera = new Camera(0);
    chromaRenderController->setCamera(camera);

    bool firstFrame = true;
    Mat cameraShow;

    changeColorSpace();

    while(running){
        try {
            if(camera->nextFrame(true)){
                cameraShow = camera->getFrame().clone();
                chromaRenderController->applyColorSpace(cameraShow, FRAME_TO_RGB);
                if (firstFrame) {
                    camera->width = cameraShow.cols;
                    camera->height = cameraShow.rows;
                    firstFrame = false;
                }
                // if (video != NULL && runningVideo) {
                //     if (video->nextFrame()) {
                //         setChannelValues();
                //         setKernelValues();
                //         chromaRenderController->applyColorSpace(video->getFrame(), FRAME_TO_RGB);

                //         chromaRenderController->applyMorphologicalOperation(camera->getFrame());
                //         chromaRenderController->merge();
                //         addMatToWidget(chromaRenderController, chromaRenderController->getResult(), 0.6);
                //         addMatToWidget(cameraThresholdRender, chromaRenderController->getCameraThreshold(), 0.6);
                //         addMatToWidget(cameraThresholdNRender, chromaRenderController->getCameraThresholdN(), 0.6);
                //         addMatToWidget(videoFusionBackgroundRender, chromaRenderController->getVideoFusionBackground(), 0.6);
                //         addMatToWidget(videoRender, video->getFrame());
                //         this->update();
                //     } else {
                //         loadVideo(pathToVideo);
                //     }
                // }

                // std::lock_guard<std::mutex> guard(frame_mutex);
                addMatToWidget(cameraRender, cameraShow);
            }
        
        } catch (exception&){
            cout << "LOG: Camera Render is busy" << endl;
        }
    }
}

long MainCavoGUI::nanoTime(){
    std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch(); 
    return std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();

}

void MainCavoGUI::startProcessConverter(){

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
                            setKernelValues();
                            chromaRenderController->applyColorSpace(video->getFrame(), FRAME_TO_RGB);

                            chromaRenderController->applyMorphologicalOperation(camera->getFrame());
                            chromaRenderController->merge();
                            addMatToWidget(chromaRenderController, chromaRenderController->getResult(), 0.6);
                            addMatToWidget(cameraThresholdRender, chromaRenderController->getCameraThreshold(), 0.6);
                            addMatToWidget(cameraThresholdNRender, chromaRenderController->getCameraThresholdN(), 0.6);
                            addMatToWidget(videoFusionBackgroundRender, chromaRenderController->getVideoFusionBackground(), 0.6);
                            addMatToWidget(videoRender, video->getFrame());
                            this->update();
                    } else {
                        loadVideo(pathToVideo);
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

void MainCavoGUI::mergeVideoCamera(){
    chromaRenderController->setTitle("Croma Aplicado");
    chromaRenderController->setVideo(video);
}

void MainCavoGUI::setChannelValues(){
    int c1Min = channel1MinSlider->getValue();
    int c2Min = channel2MinSlider->getValue();
    int c3Min = channel3MinSlider->getValue();

    int c1Max = channel1MaxSlider->getValue();
    int c2Max = channel2MaxSlider->getValue();
    int c3Max = channel3MaxSlider->getValue();

    chromaRenderController->setMinimunChannelValues(c1Min, c2Min, c3Min);
    chromaRenderController->setMaximunChannelValues(c1Max, c2Max, c3Max);

    chromaRenderController->setThreshhold(cannySlider->getValue());   
}

// HANDLE
void MainCavoGUI::handleSpaceColor(int i){
    // "RGB", "HSV", "BGR", "YCbCr", "Lab"
    switch (i) {
    case 0:
        changeChannelValues(0, 255, "R", channel1MinSlider);
        changeChannelValues(0, 255, "G", channel2MinSlider);
        changeChannelValues(0, 255, "B", channel3MinSlider);

        changeChannelValues(0, 255, "R", channel1MaxSlider, true);
        changeChannelValues(0, 255, "G", channel2MaxSlider, true);
        changeChannelValues(0, 255, "B", channel3MaxSlider, true);
        break;
    case 1:
        changeChannelValues(0, 180, "H", channel1MinSlider);
        changeChannelValues(0, 255, "S", channel2MinSlider);
        changeChannelValues(0, 255, "V", channel3MinSlider);

        changeChannelValues(0, 180, "H", channel1MaxSlider, true);
        changeChannelValues(0, 255, "S", channel2MaxSlider, true);
        changeChannelValues(0, 255, "V", channel3MaxSlider, true);
        break;
    case 2:
        changeChannelValues(0, 255, "B", channel1MinSlider);
        changeChannelValues(0, 255, "G", channel2MinSlider);
        changeChannelValues(0, 255, "R", channel3MinSlider);

        changeChannelValues(0, 255, "B", channel1MaxSlider, true);
        changeChannelValues(0, 255, "G", channel2MaxSlider, true);
        changeChannelValues(0, 255, "R", channel3MaxSlider, true);
        break;
    case 3:
        changeChannelValues(0, 255, "Y", channel1MinSlider);
        changeChannelValues(0, 255, "Cb", channel2MinSlider);
        changeChannelValues(0, 255, "Cr", channel3MinSlider);

        changeChannelValues(0, 255, "Y", channel1MaxSlider, true);
        changeChannelValues(0, 255, "Cb", channel2MaxSlider, true);
        changeChannelValues(0, 255, "Cr", channel3MaxSlider, true);
        break;

    case 4:
        changeChannelValues(0, 255, "L", channel1MinSlider);
        changeChannelValues(0, 255, "a", channel2MinSlider);
        changeChannelValues(0, 255, "b", channel3MinSlider);

        changeChannelValues(0, 255, "L", channel1MaxSlider, true);
        changeChannelValues(0, 255, "a", channel2MaxSlider, true);
        changeChannelValues(0, 255, "b", channel3MaxSlider, true);
        break;
    
    default:
        break;
    }
    changeColorSpace();
}

void MainCavoGUI::changeChannelValues(int min, int max, string title, SliderGroup * slider, bool isMax){
    slider->setValue(isMax ? max : min);
    slider->setMinimum(min);
    slider->setMaximum(max);
    slider->setTitleToBox(title);
}

void MainCavoGUI::changeColorSpace(){
    // "RGB", "HSV", "BGR", "YCbCr", "Lab"
    int i = colorSpaceCbox->currentIndex();

    switch (i) {
    case 0:
        chromaRenderController->setColorSpace(FRAME_TO_RGB);
        break;
    case 1:
        chromaRenderController->setColorSpace(FRAME_TO_HSV);
        break;
    case 2:
        chromaRenderController->setColorSpace(FRAME_TO_BGR);
        break;
    case 3:
        chromaRenderController->setColorSpace(FRAME_TO_YCbCr);
        break;

    case 4:
        chromaRenderController->setColorSpace(FRAME_TO_Lab);
        break;
    
    default:
        break;
    }
}

void MainCavoGUI::handleFilter(int i){
    // "Median Blur", "Gaussian Blur"
    // int i = colorSpaceCbox->currentIndex();
    switch (i) {
    case 0:
        chromaRenderController->setFilter(-1);
        break;
    case 1:
        chromaRenderController->setFilter(FRAME_TO_MEDBLUR);
        break;
    case 2:
        chromaRenderController->setFilter(FRAME_TO_GAUBLUR);
        break;

    default:
        break;
    }
}

void MainCavoGUI::handleEdgeDetector(int i){
    // "Canny", "Sobel", "Laplacian"
    // int i = colorSpaceCbox->currentIndex();
    showCannySlider(i - 1);
    switch (i) {
    case 0:
        chromaRenderController->setEdgeDetector(-1);
        break;
    case 1:
        chromaRenderController->setEdgeDetector(FRAME_TO_CANNY);
        break;
    case 2:
        chromaRenderController->setEdgeDetector(FRAME_TO_SOBEL);
        break;
    case 3:
        chromaRenderController->setEdgeDetector(FRAME_TO_LAPLACIAN);
        break;

    default:
        break;
    }
}

void MainCavoGUI::handleMorphologicOperation(int i){
    // "Dilatación", "Erosión", "Abierto", "Cerrado", "Black Hat", "Top Hat

    switch (i) {
    case 0:
        chromaRenderController->setMorphologicalOperation(-1);
        break;
    case 1:
        chromaRenderController->setMorphologicalOperation(FRAME_TO_DILATATION);
        break;
    case 2:
        chromaRenderController->setMorphologicalOperation(FRAME_TO_EROSION);
        break;
    case 3:
        chromaRenderController->setMorphologicalOperation(FRAME_TO_OPEN);
        break;
    case 4:
        chromaRenderController->setMorphologicalOperation(FRAME_TO_CLOSE);
        break;

    case 5:
        chromaRenderController->setMorphologicalOperation(FRAME_TO_BLACKHAT);
        break;
    case 6:
        chromaRenderController->setMorphologicalOperation(FRAME_TO_TOPHAT);
        break;
    case 7:
        chromaRenderController->setMorphologicalOperation(FRAME_TO_EQUATION);
        break;

    default:
        break;
    }
}

void MainCavoGUI::setKernelValues() {
    chromaRenderController->setFilterKernelSize(filterKernelSizeBox->value());
    chromaRenderController->setEdgeKernelSize(edgeKernelSizeBox->value());
    chromaRenderController->setMOpKernelSize(mOpKernelSizeBox->value());
}

void MainCavoGUI::showCannySlider(int index) {
    if (index == FRAME_TO_CANNY) {
        cannySlider->setVisible(true);
    } else {
        cannySlider->setVisible(false);
    }
}

void MainCavoGUI::stopProcess(){
    running = false;
    for (int i = 0; i < thread_pool.size(); i++){
        thread_pool[i].join();
    }
}

void MainCavoGUI::startProcess(){
    thread_pool.clear();
    running = true;
    thread_pool.push_back(std::thread(&MainCavoGUI::readCameraAndRender, this));
    thread_pool.push_back(std::thread(&MainCavoGUI::startProcessConverter, this));
    
}