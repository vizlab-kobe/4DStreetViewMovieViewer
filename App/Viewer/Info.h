/* ***************************************************************************/
/**
* @file Info.h
* @brief Infoクラスの定義
*/
/* ***************************************************************************/
#pragma once
#include "Model.h"
#include "Screen.h"
#include <QWidget>
#include <QTabWidget>
#include <QLayout>

namespace local
{

/*==========================================================================*/
/**
* @brief CameraInfoクラスの定義
* @details カメラや動画の情報を表示するためのクラス
*/
/*==========================================================================*/
class CameraInfo : public QWidget
{
private:
    local::Model* m_model; ///< pointer to the model
    local::Screen* m_screen; ///< pointer to the screen

    QLabel *m_title_filename; // "Filename : "
    QLabel *m_title_img_reso; // "Image resolution : "
    QLabel *m_title_cam_array; // "Camera array dimensions : "
    QLabel *m_title_num_frames; // "Number of frames : "

    QLabel *m_filename; ///< 表示中のファイル名を表示するQLabel
    QLabel *m_img_reso; ///< 表示中の画像の解像度を表示するQLabel
    QLabel *m_cam_array; ///< 表示中のカメラ位置の範囲を表示するQLabel
    QLabel *m_num_frames; ///< 表示中の動画の総フレーム数を表示するQLabel

public:
    CameraInfo( local::Model* model, local::Screen* screen, QWidget *parent = nullptr );
    /**
    * @brief m_modelにmodelをセットする関数
    * @param model Modelへのポインタ
    */
    void setup( local::Model* model ) { m_model = model; }
    void update();
};

/*==========================================================================*/
/**
* @brief DataInfoクラスの定義
* @details データに関する情報を表示するためのクラス
*/
/*==========================================================================*/
class DataInfo : public QWidget
{
private:
    local::Model* m_model; ///< pointer to the model

    QLabel *m_information; ///< データに関する情報を表示するQLabel

public:
    DataInfo( local::Model* model, QWidget *parent = nullptr );
    /**
    * @brief m_modelにmodelをセットする関数
    * @param model Modelへのポインタ
    */
    void setup( local::Model* model ){ m_model = model; }
    void update();
};

/*==========================================================================*/
/**
* @brief Infoクラスの定義
*/
/*==========================================================================*/
class Info : public QWidget
{
private:
    QTabWidget *tabWidget;
    CameraInfo *m_camera_info;
    DataInfo *m_data_info;

    local::Model* m_model; ///< pointer to the model

public:
    Info( local::Screen* screen, QWidget *parent = nullptr );
    void setup( local::Model* model );
    void update();
};

} //  end of namespace local
