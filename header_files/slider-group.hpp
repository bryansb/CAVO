#include <QSlider>
#include <QWidget>
#include <QGroupBox>
#include <QScrollBar>
#include <QDial>
#include <QBoxLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>

class SliderGroup : public QGroupBox{
    
    private slots:
        void changeValue();

    public:
        QSlider *slider;
        QDial *dial;
        QLabel *value;

        SliderGroup(Qt::Orientation orientation, const QString &title,
                    QWidget *parent);
        void setValue(int value);
        void setMinimum(int value);
        void setMaximum(int value);
        void invertAppearance(bool invert);
};