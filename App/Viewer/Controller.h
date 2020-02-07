#pragma once
#include <QWidget>
#include <QPushButton>
#include <QSlider>
#include <QCheckBox>
#include <QLabel>
#include <QSpinBox>
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
    QSlider *m_x_position;
    QSlider *m_y_position;
    QSlider *m_z_position;
    QSpinBox *m_x_pos_spbox;
    QSpinBox *m_y_pos_spbox;
    QSpinBox *m_z_pos_spbox;
    QSpinBox *m_current_index_spbox;
    QCheckBox *m_loop_box;
    QCheckBox *m_reverse_box;
    QCheckBox *m_orientation_axis_box;
    QCheckBox *m_birds_eye_box;
    QCheckBox *m_focus_mode_box;
    QLabel *m_x_label;
    QLabel *m_y_label;
    QLabel *m_z_label;

public:
    Controller( local::Model* model, local::View* view );
    QPushButton& autoButton() { return *m_auto_button; }
    QPushButton& flipDataButton() { return *m_flip_data_button; }
    QSlider& slider() { return *m_slider; }
    QSlider& xPosition() { return *m_x_position; }
    QSlider& yPosition() { return *m_y_position; }
    QSlider& zPosition() { return *m_z_position; }
    QSpinBox& xPosSpbox() { return *m_x_pos_spbox; }
    QSpinBox& yPosSpbox() { return *m_y_pos_spbox; }
    QSpinBox& zPosSpbox() { return *m_z_pos_spbox; }
    QSpinBox& currentIndexSpbox() { return *m_current_index_spbox; }
    QCheckBox& loopBox() { return *m_loop_box; }
    QCheckBox& reverseBox() { return *m_reverse_box; }
    QCheckBox& orientationAxisBox() { return *m_orientation_axis_box; }
    QCheckBox& birdsEyeBox() { return *m_birds_eye_box; }
    QCheckBox& focusModeBox() { return *m_focus_mode_box; }

    void createAutoPlayButton();
    void createFlipDataButton();
    void createFrameIndex();
    void createCameraPosition();
    void createModeBoxes();

    void showWidget();
    void show();
    void cameraPositionChanged();
    void update();

public slots:
    void autoButtonPressed();
    void flipDataButtonPressed();
    void currentIndexChanged( int value );
    void sliderCameraPositionChanged();
    void spboxCameraPositionChanged();
    void loopBoxStateChanged();
    void reverseBoxStateChanged();
    void orientationAxisBoxStateChanged();
    void birdsEyeBoxStateChanged();
    void focusModeBoxStateChanged();

};

} // end of namespace local
