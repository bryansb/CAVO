
#include "../../../../../header_files/slider-group.hpp"

SliderGroup::SliderGroup(Qt::Orientation orientation, QString title,
                           QWidget *parent) 
    : QGroupBox(title, parent) {

    setStyleSheet("QGroupBox { font-weight: bold; font-size: 14px; } ");

    dial = new QDial;
    dial->setFocusPolicy(Qt::StrongFocus);
    connect(dial, &QDial::valueChanged, this, &SliderGroup::changeValue);

    QBoxLayout *direction;

    if (orientation == Qt::Horizontal)
        direction = new QVBoxLayout();
    else
        direction = new QHBoxLayout();
    value = new QLabel("0");
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
    dial->setMinimum(v);
}

void SliderGroup::setMaximum(int v) {
    dial->setMaximum(v);
}

void SliderGroup::invertAppearance(bool invert)
{
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