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

#define MOTION_CONTROL_INTEVAL 30
#define OSCILLO_SCOPE_INTEVAL 10
#define MONITOR_INTEVAL 500
#define BOTTOM_UPDATE_INTEVAL 2000
#define MODE_MANUAL     0
#define MODE_SINE       1
#define MODE_SQUARE     2
#define MODE_TRIANGLE   3

#define ENonP_RGB_R 131 // Greenery 131, 177, 78
#define ENonP_RGB_G 177
#define ENonP_RGB_B 78

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    UserControlOnCan* can1;
    // Osilloscope
    void slotTimeOscilloScopeDone();

private slots:
    void on_cmbID_currentIndexChanged(const QString &arg1);
    // move
    void on_txtBias_editingFinished();
    void on_cmbWorkMode_currentIndexChanged(int index);
    void on_manualMax_editingFinished();
    void on_manualMin_editingFinished();
    void on_manualSlider_valueChanged(int value);
    void on_confirmButton_clicked();
    void slotTimeMoveDone();
    // Osilloscope
    void slotTimeMonitorDone();
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
    // set
    void on_IDPushButton_clicked();
    void on_setZeroPushButton_clicked();
    void on_ENonPPushButton_clicked();
    void on_btnFlash_clicked();
    void on_btnConnect_clicked();
    void on_enableDriverPushButton_clicked();
    void on_clearErrorButton_clicked();
    void on_updateButton_clicked();

    void on_btnUpdateID_clicked();
    // bottom
    void slotTimeBottomDone();
    void on_btnSave_clicked();
    void on_btnLoad_clicked();

    void on_minPosLLineEdit_editingFinished();

    void on_maxPosLLineEdit_editingFinished();

    void on_minPosHLineEdit_editingFinished();

    void on_maxPosHLineEdit_editingFinished();

    void on_enableDriverPushButton_toggled(bool checked);

private:
    Ui::MainWindow *ui;
    AdvancedControl *advControlForm;
    PaintArea* paintArea;
    Joint * jointBeingUsed; // 标记当前选中的关节
    void updatecmbID();

    // move
    QTimer* timerMove;
    uint16_t modelTypeBeingUsed;
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
    QTimer* timerOscilloScope;
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
    void setMask();
    static unsigned __stdcall oscilloScopeThreadStaticEntryPoint(void* arg);
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

    // adjust PID
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

    // keyPress
    void keyPressEvent(QKeyEvent *);

    // mouse move
    void mouseMoveEvent(QMouseEvent *);

protected:
};

#endif // MAINWINDOW_H
