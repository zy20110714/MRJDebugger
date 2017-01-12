#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

using std::cout;
using std::endl;

void MainWindow::Set()
{
    // 初始化API中的MCT里的限制值
    can1->JointSendMsg(jointBeingUsed,CMDTYPE_RD,SYS_ENABLE_ON_POWER,NULL,0x02);
    usleep(5000);
    uint16_t data_L = 0;
    can1->JointReadMsg(jointBeingUsed, SYS_ENABLE_ON_POWER,data_L);
    if (data_L != 0) {
        ui->ENonPPushButton->setStyleSheet(ENonPPushButtonOn);
    } else {
        ui->ENonPPushButton->setStyleSheet(ENonPPushButtonOff);
    }

}

void MainWindow::on_IDPushButton_clicked()
{
    int newID = ui->IDLineEdit->text().toInt();
    can1->setJointID(jointBeingUsed,newID);
}

void MainWindow::on_setZeroPushButton_clicked()
{
    //如果当前是位置控制模式，则向内存控制表SYS_SET_ZERO_POS写入1
    int workMode = can1->getJointWorkMode(jointBeingUsed);
    if (3 == workMode)
    {
        can1->setJointZeroPos(jointBeingUsed);
        // //更新分类1
        // pc.ReadWords(16, 16, PCan.currentID);
        // Thread.Sleep(10);
        //更新分类3，不更新模块会立刻运动
        ui->cmbWorkMode->setCurrentIndex(workMode); // 由当前工作模式更新控制板块中的ComboBox
        workModeUpdatetxtBias();
        txtBiasChangemanualSlider();
        //给出下一步提示，可以直接点击烧写
        QMessageBox::information(this,"提示","设置成功，请烧写Flash！");
    }
    else
    {
        QMessageBox::warning(this,"警告","当前非位置控制模式！");
    }
}

void MainWindow::on_ENonPPushButton_clicked()
{
    uint16_t data_L = 0;
    can1->JointReadMsg(jointBeingUsed, SYS_ENABLE_ON_POWER, data_L);
    int value = 0;
    if (data_L != 0) {
        value = 0;
    } else {
        value = 1;
    }
    // 向下位机请求数据
    uint8_t data[2] = {0,0};
    data[1] = (uint8_t)( (value & 0xff00) >> 8 );
    data[0] = (uint8_t)( value & 0xff );
    can1->JointSendMsg(jointBeingUsed, CMDTYPE_WR_NR, SYS_ENABLE_ON_POWER, data, 2);
    usleep(1000);
    cout << "MainWindow::on_ENonPPushButton_clicked(): value = " << value << endl;
    cout.flush();
    Set();
}
