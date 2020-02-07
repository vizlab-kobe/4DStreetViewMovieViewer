#pragma once
#include "Model.h"
#include "Screen.h"
#include <QWidget>
#include <QTabWidget>
#include <QLayout>

namespace local
{
class CameraInfo : public QWidget
{
private:
    local::Model* m_model;
    local::Screen* m_screen;

    QLabel *m_title_filename;
    QLabel *m_title_img_reso;
    QLabel *m_title_cam_array;
    QLabel *m_title_num_frames;

    QLabel *m_filename;
    QLabel *m_img_reso;
    QLabel *m_cam_array;
    QLabel *m_num_frames;

public:
    CameraInfo( local::Model* model, local::Screen* screen, QWidget *parent = nullptr );
    void setup( local::Model* model ) { m_model = model; }
    void update();
};

class DataInfo : public QWidget
{
private:
    local::Model* m_model;

    QLabel *m_information; 

public:
    DataInfo( local::Model* model, QWidget *parent = nullptr );
    void setup( local::Model* model ){ m_model = model; }
    void update();
};

class Info : public QWidget
{
private:
    QTabWidget *tabWidget;
    CameraInfo *m_camera_info;
    DataInfo *m_data_info;

    local::Model* m_model;

public:
    Info( local::Screen* screen, QWidget *parent = nullptr );
    void setup( local::Model* model );
    void update();
};

} //  end of namespace local
