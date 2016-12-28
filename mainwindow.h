#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDialog>
#include <QComboBox>
#include <QGridLayout>
#include <QLabel>
#include "joint_thread.h"

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

private:
    Ui::MainWindow *ui;
    JointThread* can1;
    int jointBeingUsed;
    int workMode;
    void workModeUpdatetxtBias();
    void txtBiasChangemanualSlider();

};

#endif // MAINWINDOW_H
