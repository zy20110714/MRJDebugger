#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDialog>
#include <QComboBox>
#include <QGridLayout>
#include <QLabel>
#include <QTimer>
#include <QKeyEvent>
#include <QMouseEvent>
#include <iostream>
#include "joint_thread.h"
#include "paintarea.h"
#include "showqueue.h"

#define MOTION_CONTROL_INTEVAL 10
#define OSCILLO_SCOPE_INTEVAL 1
#define MONITOR_INTEVAL 500
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

private slots:
    void on_cmbID_currentIndexChanged(const QString &arg1);
    void on_txtBias_editingFinished();
    void on_cmbWorkMode_currentIndexChanged(int index);
    void on_manualMax_editingFinished();
    void on_manualMin_editingFinished();
    void on_manualSlider_valueChanged(int value);
    void on_confirmButton_clicked();
    void slotTimeDone();
    void slotTimeOscilloScopeDone();
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
    void on_IDPushButton_clicked();
    void on_setZeroPushButton_clicked();
    void on_ENonPPushButton_clicked();
    void on_btnFlash_clicked();

    void on_btnConnect_clicked();
    void on_enableDriverPushButton_clicked();

    void on_clearErrorButton_clicked();

    void on_updateButton_clicked();

private:
    Ui::MainWindow *ui;
    JointThread* can1;
    PaintArea* paintArea;
    int jointBeingUsed;
    int modelTypeBeingUsed;
    void workModeUpdatetxtBias();
    void txtBiasChangemanualSlider();
    bool EnableRun;
    double bias;
    double frequency;
    double amplitude;
    QString stopButtonOn;
    QString stopButtonOff;
    QString confirmButtonOn;
    QString confirmButtonOff;

    // Osilloscope
    QTimer* time_clock;
    QTimer* timeOscilloScope;
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

    void support();
    // keyPress
    void keyPressEvent(QKeyEvent *);

    // mouse move
    void mouseMoveEvent(QMouseEvent *);

    // Bar
    QTimer* timeMonitor;
    void updateEnableDriver();
    QString enableDriverPushButtonOn;
    QString enableDriverPushButtonOff;

protected:
};

#endif // MAINWINDOW_H
