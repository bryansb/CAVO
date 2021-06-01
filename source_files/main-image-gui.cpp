#include "../header_files/header.hpp"

MainImageGUI::MainImageGUI(){
    
}

int MainImageGUI::init(){
    this->resize(1000, 800);
    this->setWindowTitle(QString::fromStdString(APP_NAME));
    widget = new QWidget(this);
    layout = new QGridLayout(widget);

    this->setCentralWidget(widget);
    widget->setLayout(layout);

    // --- ROW 0

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
    videoChooserLayout->addWidget(videoPath);
    
    imageChooserButton = new QPushButton("Seleccionar Video", imageChooserWidget);
    connect(imageChooserButton, &QPushButton::released,this, &MainImageGUI::handleVideoChooserButton);
    videoChooserLayout->addWidget(imageChooserButton);

    // --- ROW 1
    QGroupBox *channelBox = new QGroupBox("Canales", widget);
    channelBox->setMaximumWidth(300);
    channelBox->setMaximumHeight(600);
    // channelBox->setSizePolicy
    layout->addWidget(channelBox, 1, 0, 1, 1);
    QHBoxLayout *allChannelLayout = new QHBoxLayout(channelBox);

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

    // --- ROW 2

    imageBox = new QGroupBox("Resultado", widget);
    QScrollArea* scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(imageBox);
    layout->addWidget(scrollArea, 1, 1, 1, 1);

    resultLayout = new QGridLayout(imageBox);
    resultWidget = new QWidget(imageBox);

    // AUX

    resultLayout->addWidget(resultWidget);

    cameraRender = new MatRender("Cámara", resultWidget);
    resultLayout->addWidget(cameraRender, 0, 0);

    videoRender = new MatRender("Video", resultWidget);
    resultLayout->addWidget(videoRender, 1, 0);

    layout->setRowStretch(0, 1);
    layout->setRowStretch(1, 5);
    layout->setColumnStretch(0, 1);
    layout->setColumnStretch(1, 2);

    this->show();
    video = NULL;
    startProcess();
    return 0;
}

void MainImageGUI::addMatToWidget(MatRender *render, cv::Mat image, double percent){
    if (w != 0)
        render->render(image, w, percent);
}

void MainImageGUI::handleButton(){
    cout << "ENTRA" << endl;
    try {
        if (kernelSize->text().toStdString().empty()) {
            throw(20);
        }
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
    loadVideo();
}

void MainImageGUI::loadVideo(){
    video = NULL;
    string path = videoPath->text().toStdString();
    if (!path.empty()) {
        video = new Frame(videoPath->text().toStdString());
    }
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

void MainImageGUI::resizeEvent(QResizeEvent* event) {
    QMainWindow::resizeEvent(event);
    w = resultLayout->geometry().width();
}
void MainImageGUI::closeEvent (QCloseEvent *event){
    QMessageBox::StandardButton resBtn = QMessageBox::question( this, QString::fromStdString(APP_NAME),
                                                                tr("Are you sure?\n"),
                                                                QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                                                QMessageBox::Yes);
    if (resBtn != QMessageBox::Yes) {
        event->ignore();
    } else {
        RUNNING = false;
        for (int i = 0; i < thread_pool.size(); i++){
            thread_pool[i].join();
        }
        event->accept();
    }
}

void MainImageGUI::readCameraAndRender(){
    camera = new Camera();
    w = resultLayout->geometry().width();
    while(RUNNING){
        // cout << "ENTRA" << endl;
        if(camera->nextFrame()){
            addMatToWidget(cameraRender, camera->getFrame());
        }
    }
}

long MainImageGUI::nanoTime(){
    std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch(); 
    return std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();

}

void MainImageGUI::startProcessConverter(){

    w = resultLayout->geometry().width();

    int aps = 0;
    int fps = 0;
    int ups = 0;

    const int NS_PER_SECOND = 1000000000;
    const int UPS_OBJECT = 30;
    const double NS_PER_UPDATES = NS_PER_SECOND / UPS_OBJECT;

    long updateReference = nanoTime();
    long countReference = nanoTime();

    double timeElapsed;
    double delta = 0;

    long loopStart;

    while(RUNNING){
        loopStart = nanoTime();
        timeElapsed = loopStart - updateReference;
        updateReference = loopStart;

        delta += timeElapsed / NS_PER_UPDATES;

        while (delta >= 1) {
            ups++;
            delta--;
            // cout << video->getFrame() << endl;
            if (video != NULL) {
                if (video->nextFrame()) {
                    addMatToWidget(videoRender, video->getFrame());
                } else {
                    loadVideo();
                }
            }
            fps++;

        }

        if (nanoTime() - countReference > NS_PER_SECOND) {
            ups = 0;
            fps = 0;
            countReference = nanoTime();
        }
    }

}

void MainImageGUI::startProcess(){
    thread_pool.push_back(std::thread(&MainImageGUI::readCameraAndRender, this));
    thread_pool.push_back(std::thread(&MainImageGUI::startProcessConverter, this));
    
}