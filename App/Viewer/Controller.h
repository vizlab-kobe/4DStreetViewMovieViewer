#pragma once
#include <QWidget>
#include <QPushButton>
#include <QSlider>
#include <QCheckBox>
#include "Model.h"
#include "View.h"
#include "Event.h"
#include "Timer.h"

namespace local
{
class Controller : public QWidget
{
    Q_OBJECT

private:
    local::Model* m_model;
    local::View* m_view;
    local::Event m_event;
    local::Timer m_timer;

    QPushButton *m_auto_button;
    QPushButton *m_flip_data_button;
    QSlider *m_slider;
    QCheckBox *m_loop_box;
    QCheckBox *m_reverse_box;
    QCheckBox *m_orientation_axis_box;
    QCheckBox *m_birds_eye_box;
    QCheckBox *m_focus_mode_box;

public:
    Controller( local::Model* model, local::View* view );
    QPushButton& autoButton() { return *m_auto_button; }
    QPushButton& flipDataButton() { return *m_flip_data_button; }
    QSlider& slider() { return *m_slider; }
    QCheckBox& loopBox() { return *m_loop_box; }
    QCheckBox& reverseBox() { return *m_reverse_box; }
    QCheckBox& orientationAxisBox() { return *m_orientation_axis_box; }
    QCheckBox& birdsEyeBox() { return *m_birds_eye_box; }
    QCheckBox& focusModeBox() { return *m_focus_mode_box; }

    void showWidget();
    void update();

public slots:
    void autoButtonPressed();
    void flipDataButtonPressed();
    void sliderValueChanged( int value );
    void loopBoxStateChanged();
    void reverseBoxStateChanged();
    void orientationAxisBoxStateChanged();
    void birdsEyeBoxStateChanged();
    void focusModeBoxStateChanged();

};

} // end of namespace local
