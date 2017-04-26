#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

using std::cout;
using std::endl;

void MainWindow::Set()
{
    if (jointBeingUsed == NULL) {
        return;
    }
    // initialize ENABLE_ON_POWER - related content
    ENonPPushButtonOn = "color: rgb(";
    ENonPPushButtonOn += QString::number(ENonP_RGB_R) + ',' + QString::number(ENonP_RGB_G) + ',' + QString::number(ENonP_RGB_B) + ");";
    ENonPPushButtonOff = "color: rgb("; //background-color背景色
    ENonPPushButtonOff += QString::number(TGPOS_RGB_R) + ',' + QString::number(TGPOS_RGB_G) + ',' + QString::number(TGPOS_RGB_B) + ");";
    // 初始化API中的MCT里的ENABLE_ON_POWER
    can1->controller.SendMsg(jointBeingUsed->ID,CMDTYPE_RD,SYS_ENABLE_ON_POWER,NULL,0x02);
    can1->controller.delayMs(2); // 1 ms并不能成功更新MCT
    uint16_t data_L = 0;
    can1->controller.GetValueInTable(jointBeingUsed->ID, SYS_ENABLE_ON_POWER,data_L);
//    cout << "Set() - data_L: " << data_L << endl;
    if (data_L != 0) {
        ui->ENonPPushButton->setStyleSheet(ENonPPushButtonOn);
    } else {
        ui->ENonPPushButton->setStyleSheet(ENonPPushButtonOff);
    }

}

void MainWindow::on_IDPushButton_clicked()
{
    int newID = ui->IDLineEdit->text().toInt();
    jointBeingUsed->setID(newID);
}

void MainWindow::on_setZeroPushButton_clicked()
{
    //如果当前是位置控制模式，则向内存控制表SYS_SET_ZERO_POS写入1
    int workMode = jointBeingUsed->getWorkMode();
    if (3 == workMode) {
        // 停止运动控制
        on_stopButton_clicked();
        // 设置0位
        jointBeingUsed->setZeroPos();
        //给出下一步提示，可以直接点击烧写
        QMessageBox::information(this,"提示","设置成功，请烧写Flash！");
        // 停止运动控制，以更新bias显示
        on_stopButton_clicked();
    }
    else {
        QMessageBox::warning(this,"警告","当前非位置控制模式！");
    }
}

void MainWindow::on_ENonPPushButton_clicked()
{
    uint16_t data_L = 0;
    can1->controller.GetValueInTable(jointBeingUsed->ID, SYS_ENABLE_ON_POWER, data_L);
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
    can1->controller.SendMsg(jointBeingUsed->ID, CMDTYPE_WR_NR, SYS_ENABLE_ON_POWER, data, 2);
    can1->controller.delayMs(1);
    cout << "MainWindow::on_ENonPPushButton_clicked(): value = " << value << endl;
    Set();
}
