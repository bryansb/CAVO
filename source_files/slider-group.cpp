#include "../header_files/slider-group.hpp"
#include <iostream>
using namespace std;

SliderGroup::SliderGroup(Qt::Orientation orientation, QString title,
                           QWidget *parent) 
    : QGroupBox(title, parent) {

    // slider = new QSlider(orientation);
    // slider->setFocusPolicy(Qt::StrongFocus);
    // slider->setTickPosition(QSlider::TicksBothSides);
    // slider->setTickInterval(10);
    // slider->setSingleStep(1);

    // QFont font;
    // font.setBold(true);
    // font.setPixelSize(20);
    // setFont(font);

    setStyleSheet("QGroupBox { font-weight: bold; font-size: 15px; } ");

    dial = new QDial;
    dial->setFocusPolicy(Qt::StrongFocus);
    // dial->setMaximumSize(QSize(w, h));

    // connect(slider, SIGNAL(valueChanged(int)), dial, SLOT(setValue(int)));
    // connect(dial, SIGNAL(valueChanged(int)), slider, SLOT(setValue(int)));
    // connect(dial, SIGNAL(valueChanged()), this, SIGNAL(setValue(int)));
    // connect(dial, SIGNAL(valueChanged(int)), this, &SliderGroup::changeValue);
    connect(dial, &QDial::valueChanged, this, &SliderGroup::changeValue);

    QBoxLayout *direction;

    if (orientation == Qt::Horizontal)
        direction = new QVBoxLayout();
    else
        direction = new QHBoxLayout();

    // QBoxLayout *slidersLayout = new QBoxLayout(direction);
    value = new QLabel("0");
    // value->setMaximumHeight(10);
    value->setAlignment(Qt::AlignCenter);
    direction->addWidget(value);
    direction->addWidget(dial);
    setLayout(direction);
    
}

void SliderGroup::setValue(int v) {
    dial->setValue(v);
    value->setText(QString::fromStdString(to_string(v)));
}

void SliderGroup::setMinimum(int v) {
    // slider->setMinimum(value);
    dial->setMinimum(v);
}

void SliderGroup::setMaximum(int v) {
    // slider->setMaximum(value);
    dial->setMaximum(v);
}

void SliderGroup::invertAppearance(bool invert)
{
    // slider->setInvertedAppearance(invert);
    dial->setInvertedAppearance(invert);
}

void SliderGroup::changeValue(){
    setValue(dial->value());
}

void SliderGroup::setTitleToBox(string title) {
    this->setTitle(QString::fromStdString(title));
}

int SliderGroup::getValue() {
    return dial->value();
}