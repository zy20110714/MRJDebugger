#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <vector>
#include <iostream>

using std::vector;
using std::cout;
using std::endl;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    // use shellscript to initialize the CAN path
    FILE *fp = popen("./../findPCAN.sh", "r"); // ./ represents the relative path to the build folder
    if(fp == NULL)
    {
       cout << "The shellscript could not be found to initialize the CAN path.\n";
       exit(0);
    }
    // Save the path to a variable
    char* canPath = new char[1024];
    fgets(canPath, 1024, fp);
    // Delete the '\n' at the end of the variable
    int n = strlen(canPath);
    canPath[n-1]='\0';
    pclose(fp);

    can1 = new JointThread();
    cout << canPath << endl;
    can1->JointCanInit(canPath);
    can1->JointThreadStart(3000);

    jointBeingUsed = 0;

    ui->setupUi(this);
    // Fill the items fo combo box
    vector<int> allID = can1->getJointallID();//getallID();
    for (vector<int>::iterator iter = allID.begin(); iter != allID.end(); ++iter) {
        ui->cmbID->addItem(QString::number(*iter, 10));
    }
    ui->cmbID->setCurrentIndex(allID.size() - 1); // 获取末尾ID，方便调试，index从0开始，size从1开始

    ui->manualMin->setText("-30");
    ui->manualMax->setText("30");
    txtBiasChangemanualSlider();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_cmbID_currentIndexChanged(const QString &arg1)
{
    jointBeingUsed = arg1.toInt(); // jointBeingUsed = ui->cmbID->currentText().toInt(); // 修改当前控制的模块ID
    workMode = can1->getJointWorkMode(jointBeingUsed);
    ui->cmbWorkMode->setCurrentIndex(workMode); // 由当前工作模式更新控制板块中的ComboBox
    workModeUpdatetxtBias();
    txtBiasChangemanualSlider();
}

void MainWindow::on_txtBias_editingFinished()
{
    float angle;
    int pwm;
    float speed;
    int curI;

    switch(workMode) // 不同控制模式，控制指令不同
    {
        case MODE_OPEN:
            pwm = ui->txtBias->text().toInt();
            can1->setTagPWM(jointBeingUsed, pwm);
            break;
        case MODE_CURRENT:
            curI = ui->txtBias->text().toInt();
            can1->setTagI(jointBeingUsed, curI);
            break;
        case MODE_SPEED:
            speed = ui->txtBias->text().toFloat();
            can1->setTagSpeed(jointBeingUsed, speed, MODEL_TYPE_M20, CMDTYPE_WR_NR);
            break;
        case MODE_POSITION:
            angle = ui->txtBias->text().toFloat();
            can1->setTagPos(jointBeingUsed, angle, JOINT_ANGLE, MODEL_TYPE_M20, CMDTYPE_WR_NR);
            break;
        default: break;
    }
    txtBiasChangemanualSlider();
}

void MainWindow::on_cmbWorkMode_currentIndexChanged(int index)
{
    workMode = index;
    can1->setJointWorkMode(jointBeingUsed,workMode);
    workModeUpdatetxtBias();
    txtBiasChangemanualSlider();
}

void MainWindow::workModeUpdatetxtBias()
{
    switch(workMode) // 工作模式修改后，修改txtBias
    {
        case MODE_OPEN:
            can1->readTagPWM(jointBeingUsed);
            usleep(1000);
            ui->txtBias->setValue(can1->getTagPWM(jointBeingUsed)); // 由当前目标PWM更新手动控制中的偏移量
            break;
        case MODE_CURRENT:
            can1->readCurI(jointBeingUsed);
            usleep(1000);
            ui->txtBias->setValue(can1->getCurI(jointBeingUsed)); // 由当前实际电流更新手动控制中的偏移量
            break;
        case MODE_SPEED:
            can1->readCurSpeed(jointBeingUsed);
            usleep(1000);
            ui->txtBias->setValue(can1->getCurSpeed(jointBeingUsed, MODEL_TYPE_M20)); // 由当前实际电流更新手动控制中的偏移量
            break;
        case MODE_POSITION:
            can1->readCurPos(jointBeingUsed);
            usleep(1000);
            ui->txtBias->setValue(can1->getCurPos(jointBeingUsed, JOINT_ANGLE, MODEL_TYPE_M20)); // 由当前实际位置更新手动控制中的偏移量
            break;
        default: break;
    }
}

void MainWindow::on_manualMax_editingFinished()
{
    float max = ui->manualMax->text().toFloat();
    QString s = QString("%1").arg(max * -1);
    ui->manualMin->setText(s);
    txtBiasChangemanualSlider();
}

void MainWindow::on_manualMin_editingFinished()
{
    txtBiasChangemanualSlider();
}

void MainWindow::txtBiasChangemanualSlider()
{
    float bias = ui->txtBias->text().toFloat();
    float min = ui->manualMin->text().toFloat();
    float max = ui->manualMax->text().toFloat();
    float f = 100 * (bias - min) / (max - min);
    int v = (int)f;
    ui->manualSlider->setValue(v);
}

void MainWindow::on_manualSlider_valueChanged(int value)
{
    float min = ui->manualMin->text().toFloat();
    float max = ui->manualMax->text().toFloat();
    if (ui->manualSlider->hasFocus()) {
        float bias = min + (max - min) * value / 100;
        ui->txtBias->setValue(bias);
        on_txtBias_editingFinished();
    }
}
