#include "../../../../../header_files/main_cavo_gui.hpp"

MainCavoGUI::MainCavoGUI(){}

int MainCavoGUI::init(){
    // setWindowFlags(Qt::Widget | Qt::MSWindowsFixedSizeDialogHint);
    // this->setFixedSize(1500, 900);
    // setSizeIncrement(1500, 900);
    // setMinimumSize(1500, 900);
    // setBaseSize(1500, 900);
    resize(1500, 900);
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
    layout->addWidget(channelBox, 1, 0, 2, 1);

    QVBoxLayout * colorSpaceLayout = new QVBoxLayout(channelBox);

    QWidget * kernelWidget = new QWidget(channelBox);
    colorSpaceLayout->addWidget(kernelWidget);
    QHBoxLayout * kernelLayout = new QHBoxLayout(kernelWidget);

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
    QStackedWidget *stackedWidget = new QStackedWidget;
    stackedWidget->addWidget(channel1MinSlider);
    channelLayout->addWidget(stackedWidget);
    connect(channel1MinSlider->getDial(), QOverload<int>::of(&QDial::valueChanged), this, &MainCavoGUI::handleChannel1Min);
    channel1MinSlider->setMaximum(255);

    channel2MinSlider = new SliderGroup(Qt::Horizontal, tr("G"), channelWidget);
    channel2MinSlider->setMaximum(255);
    stackedWidget = new QStackedWidget;
    stackedWidget->addWidget(channel2MinSlider);
    channelLayout->addWidget(stackedWidget);
    connect(channel2MinSlider->getDial(), QOverload<int>::of(&QDial::valueChanged), this, &MainCavoGUI::handleChannel2Min);

    channel3MinSlider = new SliderGroup(Qt::Horizontal, tr("B"), channelWidget);
    channel3MinSlider->setMaximum(255);
    stackedWidget = new QStackedWidget;
    stackedWidget->addWidget(channel3MinSlider);
    channelLayout->addWidget(stackedWidget);
    connect(channel3MinSlider->getDial(), QOverload<int>::of(&QDial::valueChanged), this, &MainCavoGUI::handleChannel3Min);
    

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
    connect(channel1MaxSlider->getDial(), QOverload<int>::of(&QDial::valueChanged), this, &MainCavoGUI::handleChannel1Max);

    channel2MaxSlider = new SliderGroup(Qt::Horizontal, tr("G"), channelWidget);
    channel2MaxSlider->setMaximum(255);
    channel2MaxSlider->setValue(255);
    stackedWidget = new QStackedWidget;
    stackedWidget->addWidget(channel2MaxSlider);
    channelLayout->addWidget(stackedWidget);
    connect(channel2MaxSlider->getDial(), QOverload<int>::of(&QDial::valueChanged), this, &MainCavoGUI::handleChannel2Max);

    channel3MaxSlider = new SliderGroup(Qt::Horizontal, tr("B"), channelWidget);
    channel3MaxSlider->setMaximum(255);
    channel3MaxSlider->setValue(255);
    stackedWidget = new QStackedWidget;
    stackedWidget->addWidget(channel3MaxSlider);
    channelLayout->addWidget(stackedWidget);
    connect(channel3MaxSlider->getDial(), QOverload<int>::of(&QDial::valueChanged), this, &MainCavoGUI::handleChannel3Max);

    // --- ROW 1, 1

    QGroupBox *allFilterBox = new QGroupBox("Filtros", widget);
    allFilterBox->setContentsMargins(0,0,0,0);
    allFilterBox->setMaximumHeight(150);
    layout->addWidget(allFilterBox, 1, 1, 1, 1);
    QHBoxLayout *allFilterLayout = new QHBoxLayout(allFilterBox);
    allFilterLayout->setContentsMargins(0,0,0,0);

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
    connect(filterKernelSizeBox, qOverload<int>(&QSpinBox::valueChanged), this, &MainCavoGUI::handleFilterKernel);
    
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
    connect(cannySlider->getDial(), QOverload<int>::of(&QDial::valueChanged), this, &MainCavoGUI::handleCannyKernel);
    filterLayoutBox->addWidget(cannySlider);

    kernelLabel = new QLabel("Detección de Bordes:");
    kernelLabel->setFixedHeight(20);
    filterLayout->addWidget(kernelLabel);

    // SpinBox
    edgeKernelSizeBox = new QSpinBox(filterWidget);
    edgeKernelSizeBox->setMinimum(3);
    edgeKernelSizeBox->setSingleStep(2);
    edgeKernelSizeBox->setPrefix("Kernel: ");
    connect(edgeKernelSizeBox, qOverload<int>(&QSpinBox::valueChanged), this, &MainCavoGUI::handleEdgeKernel);
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
    connect(mOpKernelSizeBox, qOverload<int>(&QSpinBox::valueChanged), this, &MainCavoGUI::handleOMorphKernel);
    filterLayout->addWidget(mOpKernelSizeBox);

    morphologicalOperationCbox = new QComboBox(filterWidget);
    morphologicalOperationCbox->addItems({"Ninguno", "Dilatación", "Erosión", "Abierto", "Cerrado", "Black Hat", "Top Hat", "Ecuación"});
    filterLayout->addWidget(morphologicalOperationCbox);
    connect(morphologicalOperationCbox, QOverload<int>::of(&QComboBox::activated),
            this, &MainCavoGUI::handleMorphologicOperation);

    // --- ROW 2, 1

    imageBox = new QGroupBox("Resultado", widget);
    // QScrollArea *scrollArea = new QScrollArea();
    // scrollArea->setWidgetResizable(true);
    // scrollArea->setWidget(imageBox);
    layout->addWidget(imageBox, 2, 1, 1, 1);

    resultLayout = new QGridLayout(imageBox);

    QWidget *selectionResultW = new QWidget(imageBox);
    selectionResultW->setContentsMargins(2, 2, 2, 2);
    selectionResultW->setMaximumHeight(50);
    resultLayout->addWidget(selectionResultW, 0, 0, 1, 2);
    QHBoxLayout *selectionResultL = new QHBoxLayout(selectionResultW);

    selectionResultL->addWidget(new QLabel("Seleccione un resultado:"));
    resultsRenderCbox = new QComboBox(imageBox);
    resultsRenderCbox->addItems({"Croma aplicado", "Umbralización", "Umbralización Inversa", "Fusión del Video"});
    selectionResultL->addWidget(resultsRenderCbox);
    selectionResultL->addStretch();
    connect(resultsRenderCbox, QOverload<int>::of(&QComboBox::activated),
            this, &MainCavoGUI::handleShowResult);

    // resultLayout->addWidget(resultWidget);

    cameraRender = new MatRender("Cámara", w,  imageBox);
    resultLayout->addWidget(cameraRender, 1, 0, 1, 1);

    videoRender = new MatRender("Video", w, imageBox);
    resultLayout->addWidget(videoRender, 2, 0, 1, 1);

    chromaRenderController = new ChromaRenderController("Seleccione un Video", w, imageBox);
    resultLayout->addWidget(chromaRenderController, 1, 1, 2, 1);

    layout->setRowStretch(0, 1);
    layout->setRowStretch(1, 5);
    layout->setColumnStretch(0, 1);
    layout->setColumnStretch(1, 2);

    this->show();
    w = imageBox->width();
    cameraRender->setWidth(w);
    videoRender->setWidth(w);
    chromaRenderController->setWidth(w);
    
    startProcess();
    return 0;
}

void MainCavoGUI::resizeEvent(QResizeEvent* event){
    QMainWindow::resizeEvent(event);
    // w = imageBox->width();
    // cameraRender->setWidth(w);
    // videoRender->setWidth(w);
    // chromaRenderController->setWidth(w);
}


void MainCavoGUI::handleVideoChooserButton(){
    videoPath->setText(QFileDialog::getOpenFileName(this,
        tr("Seleccione un Video"), "../", tr("Video Files (*.mp4 *.mkv)")));
    loadVideo(videoPath->text().toStdString());
}

void MainCavoGUI::loadVideo(string path){
    if (!path.empty()) {
        pathToVideo = path; 
        threadRC->loadNewVideo(pathToVideo);

    }
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

void MainCavoGUI::setChannelValues(){

    chromaRenderController->setMinimunChannelValues(channel1Min, channel2Min, channel3Min);
    chromaRenderController->setMaximunChannelValues(channel1Max, channel2Max, channel3Max);

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

void MainCavoGUI::handleShowResult(int selectResultIndex){
    threadRC->setSelectResultIndex(selectResultIndex);
}

void MainCavoGUI::showCannySlider(int index){
    if (index == FRAME_TO_CANNY) {
        cannySlider->setVisible(true);
    } else {
        cannySlider->setVisible(false);
    }
}

void MainCavoGUI::stopProcess(){
    threadRC->stop();
}

void MainCavoGUI::startProcess(){
    threadRC = new ThreadRenderController();
    camera = new Camera(cameraNumber);
    chromaRenderController->setCamera(camera);
    
    threadRC->startCamera(camera, cameraRender, imageBox);

    setChannelValues();
    threadRC->startChromaRenderController(chromaRenderController, videoRender, imageBox);
    
}

void MainCavoGUI::handleChannel1Min(int value){
    chromaRenderController->setChannel1Min(value);
}

void MainCavoGUI::handleChannel2Min(int value){
    chromaRenderController->setChannel2Min(value);
}

void MainCavoGUI::handleChannel3Min(int value){
    chromaRenderController->setChannel3Min(value);
}

void MainCavoGUI::handleChannel1Max(int value){
    chromaRenderController->setChannel1Max(value);
}

void MainCavoGUI::handleChannel2Max(int value){
    chromaRenderController->setChannel2Max(value);
}

void MainCavoGUI::handleChannel3Max(int value){
    chromaRenderController->setChannel3Max(value);
}

void MainCavoGUI::handleFilterKernel(int v){
    chromaRenderController->setFilterKernelSize(v);
}

void MainCavoGUI::handleEdgeKernel(int v){
    chromaRenderController->setEdgeKernelSize(v);
}

void MainCavoGUI::handleOMorphKernel(int v){
    chromaRenderController->setMOpKernelSize(v);
}

void MainCavoGUI::handleCannyKernel(int v){
    chromaRenderController->setThreshhold(v);  
}