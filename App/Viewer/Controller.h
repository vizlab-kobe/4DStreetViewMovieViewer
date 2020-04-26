/* ***************************************************************************/
/**
* @file  Controller.h
* @brief Definition of Controller class
*/
/* ***************************************************************************/
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
/*==========================================================================*/
/**
* @brief Controller class for operating the displaying image
*/
/*==========================================================================*/
class Controller : public QWidget
{
    Q_OBJECT

private:
    local::Model* m_model; ///< pointer to the model
    local::View* m_view; ///< pointer to the view
    local::Event m_event; ///< Event
    local::Timer m_timer; ///< Timer

    QPushButton *m_auto_button; ///< AutoPlay Button
    QPushButton *m_flip_data_button; ///< Button for switching the displaying data
    QSlider *m_slider; ///< Slider of frame index
    QSpinBox *m_current_index_spbox; ///< SpinBox of frame index
    QLabel *m_x_label; ///< Label of camera position X
    QLabel *m_y_label; ///< Label of camera position Y
    QLabel *m_z_label; ///< Label of camera position Z
    QSlider *m_x_position; ///< Slider for changing camera direction X
    QSlider *m_y_position; ///< Slider for changing camera direction Y
    QSlider *m_z_position; ///< Slider for changing camera direction Z
    QSpinBox *m_x_pos_spbox; ///< SpinBox for camera direction X
    QSpinBox *m_y_pos_spbox; ///< SpinBox for camera direction Y
    QSpinBox *m_z_pos_spbox; ///< SpinBox for camera direction Z
    QCheckBox *m_loop_box; ///< LoopPlay CheckBox
    QCheckBox *m_reverse_box; ///< ReversePlay CheckBox
    QCheckBox *m_orientation_axis_box; ///< OrientationAxis CheckBox
    QCheckBox *m_birds_eye_box; ///< Bird'sEyeViewMode CheckBox
    QCheckBox *m_focus_mode_box; ///< FocusMode CheckBox

public:
    Controller( local::Model* model, local::View* view );
    QPushButton& autoButton() { return *m_auto_button; } ///< m_auto_buttonのgetter
    QPushButton& flipDataButton() { return *m_flip_data_button; } ///< m_flip_data_buttonのgetter
    QSlider& slider() { return *m_slider; } ///< m_sliderのgetter
    QSlider& xPosition() { return *m_x_position; } ///< m_x_positionのgetter
    QSlider& yPosition() { return *m_y_position; } ///< m_y_positionのgetter
    QSlider& zPosition() { return *m_z_position; } ///< m_z_positionのgetter
    QSpinBox& xPosSpbox() { return *m_x_pos_spbox; } ///< m_x_pos_spboxのgetter
    QSpinBox& yPosSpbox() { return *m_y_pos_spbox; } ///< m_y_pos_spboxのgetter
    QSpinBox& zPosSpbox() { return *m_z_pos_spbox; } ///< m_z_pos_spboxのgetter
    QSpinBox& currentIndexSpbox() { return *m_current_index_spbox; } ///< m_current_index_spboxのgetter
    QCheckBox& loopBox() { return *m_loop_box; } ///< m_loop_boxのgetter
    QCheckBox& reverseBox() { return *m_reverse_box; } ///< m_reverse_boxのgetter
    QCheckBox& orientationAxisBox() { return *m_orientation_axis_box; } ///< m_orientation_axis_boxのgetter
    QCheckBox& birdsEyeBox() { return *m_birds_eye_box; } ///< m_birds_eye_boxのgetter
    QCheckBox& focusModeBox() { return *m_focus_mode_box; } ///< m_focus_mode_boxのgetter

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
