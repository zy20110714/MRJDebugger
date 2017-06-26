#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "advancedcontrol.h"
#include <QMainWindow>
#include <QDialog>
#include <QComboBox>
#include <QGridLayout>
#include <QLabel>
#include <QTimer>
#include <QKeyEvent>
#include <QMouseEvent>
#include <iostream>
#include "usercontroloncan.h"
#include "paintarea.h"
#include "showqueue.h"
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_layout.h>
#include <qwt_plot_magnifier.h>
#include <qwt_plot_panner.h>
#include "oscilloscopethread.h"
#include "motioncontrol.h"

#define MOTION_CONTROL_INTEVAL 30   // 运动控制周期（ms）
#define OSCILLO_SCOPE_INTEVAL 10    // 示波器采样周期（ms）
#define MONITOR_INTEVAL 500         // 监视器更新周期（ms）
#define BOTTOM_UPDATE_INTEVAL 2000  // 底部更新周期（ms）
#define MODE_MANUAL     0           // 手动控制
#define MODE_SINE       1           // 自动控制之正弦波
#define MODE_SQUARE     2           // 自动控制之方波
#define MODE_TRIANGLE   3           // 自动控制之三角波

#define ENonP_RGB_R 131             // enable on power 按键的颜色定义
#define ENonP_RGB_G 177             //
#define ENonP_RGB_B 78              // Greenery 131, 177, 78

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    // CANAPI
    UserControlOnCan * can1;
    // Osilloscope
    MotionControl * MC;

private slots:
    // move
    void on_txtBias_editingFinished();
    void on_cmbWorkMode_currentIndexChanged(int index);
    void on_manualMax_editingFinished();
    void on_manualMin_editingFinished();
    void on_manualSlider_valueChanged(int value);
    void on_confirmButton_clicked();
    void slotTimeMoveDone();
    // Osilloscope
    void on_stopButton_clicked();
    void on_waveModeCombo_currentIndexChanged(int index);
    void on_frequencyLineEdit_editingFinished();
    void on_AmplitudeLineEdit_editingFinished();
    void on_tgPOSPushButton_clicked();
    void on_tgSPDPushButton_clicked();
    void on_tgCURPushButton_clicked();
    void on_rlPOSPushButton_clicked();
    void on_rlSPDPushButton_clicked();
    void on_rlCURPushButton_clicked();
    void on_scopeEnablePushButton_clicked();
    void on_offset_POSLineEdit_editingFinished();
    void on_offset_SPDLineEdit_editingFinished();
    void on_offset_CURLineEdit_editingFinished();
    void on_prComboBox_currentIndexChanged(int index);
    void on_srComboBox_currentIndexChanged(int index);
    void on_crComboBox_currentIndexChanged(int index);
    void on_ScanFrequencyComboBox_currentIndexChanged(int index);
    void on_adjustGroupComboBox_currentIndexChanged(int index);
    void updatePlot();
    // health
    void slotTimeMonitorDone();
    // PID
    void on_POS_PSpinBox_editingFinished();
    void on_POS_ISpinBox_editingFinished();
    void on_POS_DSpinBox_editingFinished();
    void on_POS_DSSpinBox_editingFinished();
    void on_SPD_PSpinBox_editingFinished();
    void on_SPD_ISpinBox_editingFinished();
    void on_SPD_DSpinBox_editingFinished();
    void on_SPD_DSSpinBox_editingFinished();
    void on_CUR_PSpinBox_editingFinished();
    void on_CUR_ISpinBox_editingFinished();
    void on_CUR_DSpinBox_editingFinished();
    void on_maxCurLineEdit_editingFinished();
    void on_maxSpdLineEdit_editingFinished();
    void on_maxAccLineEdit_editingFinished();
    void on_minPosLLineEdit_editingFinished();
    void on_maxPosLLineEdit_editingFinished();
    void on_minPosHLineEdit_editingFinished();
    void on_maxPosHLineEdit_editingFinished();
    // set
    void on_IDPushButton_clicked();
    void on_setZeroPushButton_clicked();
    void on_ENonPPushButton_clicked();
    void on_btnFlash_clicked();
    void on_btnConnect_clicked();
    void on_enableDriverPushButton_clicked();
    void on_clearErrorButton_clicked();
    void on_updateButton_clicked();
    // bottom
    void on_btnUpdateID_clicked();
    void slotTimeBottomDone();
    void on_btnSave_clicked();
    void on_btnLoad_clicked();
    void on_cmbID_currentIndexChanged(const QString &arg1);

public:
    Ui::MainWindow *ui;                 // 主要界面
    AdvancedControl *advControlForm;    // 另一界面
    Joint* jointBeingUsed;             // 标记当前选中的关节
    bool isCANInitialSucceed;
    // move
    QTimer* timerMove;
    bool EnableRun;
    double bias;
    double frequency;
    double amplitude;
    QString stopButtonOn;
    QString stopButtonOff;
    QString confirmButtonOn;
    QString confirmButtonOff;
    void workModeUpdatetxtBias();
    void txtBiasChangemanualSlider();
    void moveInitialize();
    // Osilloscope
    OscilloScopeThread *osthread;
    QTimer *timerOscilloScope;
    QString tgPOSPushButtonOn;
    QString tgPOSPushButtonOff;
    QString tgSPDPushButtonOn;
    QString tgSPDPushButtonOff;
    QString tgCURPushButtonOn;
    QString tgCURPushButtonOff;
    QString rlPOSPushButtonOn;
    QString rlPOSPushButtonOff;
    QString rlSPDPushButtonOn;
    QString rlSPDPushButtonOff;
    QString rlCURPushButtonOn;
    QString rlCURPushButtonOff;
    QString scopeEnablePushButtonOn;
    QString scopeEnablePushButtonOff;
    void SetValue(double value);
    void OscilloScope();
//    void setMask();
    QwtPlotCurve *curveTgPOS;
    QwtPlotCurve *curveTgSPD;
    QwtPlotCurve *curveTgCUR;
    QwtPlotCurve *curveRlPOS;
    QwtPlotCurve *curveRlSPD;
    QwtPlotCurve *curveRlCUR;
    QwtPlotGrid *grid;
    QwtPlotCurve *curveGrid;
    QwtPlotMagnifier * plotMag;
    QwtPlotMagnifier * plotSPDMag;
    QwtPlotMagnifier * plotPOSMag;
    // PID
    void PID();
    void showSEVPID();
    void showMPID();
    void showLPID();
    void setNewPID(int value,int index);
    // basic setting operation
    QString ENonPPushButtonOn;
    QString ENonPPushButtonOff;
    void Set();
    // health
    QTimer* timerMonitor;
    void health();
    // bottom
    QTimer* timerBottom;
    void updateEnableDriver();
    void updateIfError();
    void updateConnected();
    void updateWorkModePushButton();
    QString enableDriverPushButtonOn;
    QString enableDriverPushButtonOff;
    QString ifErrorPushButton_NoError;
    QString ifErrorPushButton_Error;
    void updatecmbID();
    uint16_t modelTypeBeingUsed;
    // keyPress
    void keyPressEvent(QKeyEvent *);
    // mouse move
    void mouseMoveEvent(QMouseEvent *);
};

#endif // MAINWINDOW_H
