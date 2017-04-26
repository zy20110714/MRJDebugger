#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "connectform.h"

using std::cout;
using std::endl;

void MainWindow::health()
{
    slotTimeMonitorDone();// 运行一次更新数据
    if (timerMonitor == NULL) {
        timerMonitor = new QTimer(this);
        connect(timerMonitor,SIGNAL(timeout()),this,SLOT(slotTimeMonitorDone()));
        timerMonitor->start(MONITOR_INTEVAL);
    } // 暂时持续性地更新数据 // 暂时不需要持续性地更新数据
}

void MainWindow::on_clearErrorButton_clicked()
{
    jointBeingUsed->clearErrStatus();
    cout << "MainWindow::on_clearErrorButton_clicked(): done."<< endl;
}

void MainWindow::on_updateButton_clicked()
{
    slotTimeMonitorDone();
}

void MainWindow::slotTimeMonitorDone()
{
//    updateEnableDriver(); // 不属于health部分
    if (jointBeingUsed == NULL) {
        return;
    }

    can1->controller.SendMsg(jointBeingUsed->ID,CMDTYPE_RD,SYS_VOLTAGE,NULL,0x02);
    can1->controller.delayMs(1);
    can1->controller.SendMsg(jointBeingUsed->ID,CMDTYPE_RD,SYS_CURRENT_L,NULL,0x04);
    can1->controller.delayMs(1);
    can1->controller.SendMsg(jointBeingUsed->ID,CMDTYPE_RD,SYS_TEMP,NULL,0x02);
    can1->controller.delayMs(1);
//    can1->controller.SendMsg(jointBeingUsed->ID,CMDTYPE_RD,SYS_SPEED_L,NULL,0x04);
    jointBeingUsed->updateCurSpeed();
    can1->controller.delayMs(1);
    can1->controller.SendMsg(jointBeingUsed->ID,CMDTYPE_RD,SYS_POSITION_L,NULL,0x04);
    can1->controller.delayMs(1);
    can1->controller.SendMsg(jointBeingUsed->ID,CMDTYPE_RD,MOT_ST_DAT,NULL,0x04);
    can1->controller.delayMs(1);
    can1->controller.SendMsg(jointBeingUsed->ID,CMDTYPE_RD,SYS_ERROR,NULL,0x02);
    can1->controller.delayMs(1);
    can1->controller.SendMsg(jointBeingUsed->ID,CMDTYPE_RD,BAT_VOLT,NULL,0x02);
    can1->controller.delayMs(1);
    can1->controller.SendMsg(jointBeingUsed->ID,CMDTYPE_RD,ACC_X,NULL,0x06);
    can1->controller.delayMs(5);

    uint16_t data_L = 0;
    uint16_t data_H = 0;

    // 更新电压，并显示
    can1->controller.GetValueInTable(jointBeingUsed->ID, SYS_VOLTAGE, data_L);
    ui->VolLineEdit->setText(QString::number((double)data_L / 100, 'f' ,2) + "V");

    // 更新电流，并显示
    can1->controller.GetValueInTable(jointBeingUsed->ID, SYS_CURRENT_L, data_L);
    can1->controller.GetValueInTable(jointBeingUsed->ID, SYS_CURRENT_H, data_H);
    double temp = data_L + (data_H * 65536);
    ui->CurLineEdit->setText(QString::number(temp / 1000, 'f', 2) + "A");

    // 更新温度，并显示
    can1->controller.GetValueInTable(jointBeingUsed->ID, SYS_TEMP,data_L);
    ui->TempLineEdit->setText(QString::number((double)data_L / 10, 'f', 1) + "℃");

    // 更新速度，并显示
//    can1->controller.GetValueInTable(jointBeingUsed->ID, SYS_SPEED_L, data_L);
//    can1->controller.GetValueInTable(jointBeingUsed->ID, SYS_SPEED_H, data_H);
//    temp = data_L + (data_H * 65536);
//    temp *= 60; // rpm
//    temp /= 65536; // rpm
//    temp /= GEAR_RATIO_M14;
    temp = jointBeingUsed->getCurSpeed();
    ui->SpeedLineEdit->setText(QString::number(temp, 'f', 2) + "rpm");

    // 更新位置，并显示
    can1->controller.GetValueInTable(jointBeingUsed->ID, SYS_POSITION_L, data_L);
    can1->controller.GetValueInTable(jointBeingUsed->ID, SYS_POSITION_H, data_H);
    ui->POS_LLineEdit->setText(QString::number((double)data_L * 360.0 / 65536.0, 'f', 2) + "°");
    ui->POS_HLineEdit->setText(QString::number((short)data_H));

    // 更新编码器，并显示
    can1->controller.GetValueInTable(jointBeingUsed->ID, MOT_ST_DAT, data_L);
    can1->controller.GetValueInTable(jointBeingUsed->ID, MOT_MT_DAT, data_H);
    ui->STurnLineEdit->setText(QString::number((double)data_L * 360.0 / 65536.0, 'f', 2) + "°");
    ui->MTurnLineEdit->setText(QString::number((short)data_H));

    // 更新错误码，并显示
    can1->controller.GetValueInTable(jointBeingUsed->ID, SYS_ERROR, data_L);
    // 按错误码显示文字
//    ui->errorLineEdit->setVisible(false); // 临时性
//    data_L = ERROR_MASK_OVER_CURRENT; // 测试用
    ui->errorTextEdit->clear();
    if (data_L & ERROR_MASK_OVER_CURRENT) {
        ui->errorTextEdit->append("过流，错误码0x0001\n");
    }
    if (data_L & ERROR_MASK_OVER_VOLTAGE) {
        ui->errorTextEdit->append("过压，错误码0x0002\n");
    }
    if (data_L & ERROR_MASK_UNDER_VOLTAGE) {
        ui->errorTextEdit->append("欠压，错误码0x0004\n");
    }
    if (data_L & ERROR_MASK_OVER_TEMP) {
        ui->errorTextEdit->append("过温，错误码0x0008\n");
    }
    if (data_L & ERROR_MASK_BATTERY) {
        ui->errorTextEdit->append("编码器电池错误，错误码0x0010\n");
    }
    if (data_L & ERROR_MASK_ENCODER) {
        ui->errorTextEdit->append("码盘错误，错误码0x0020\n");
    }
    if (data_L & ERROR_MASK_POTEN) {
        ui->errorTextEdit->append("电位器错误，错误码0x0040\n");
    }
    if (data_L & ERROR_MASK_CURRENT_INIT) {
        ui->errorTextEdit->append("电流检测错误，错误码0x0080\n");
    }
    if (data_L & ERROR_MASK_FUSE) {
        ui->errorTextEdit->append("保险丝断开错误，错误码0x0100\n");
    }
    if (data_L == 0) {
        ui->errorTextEdit->append("No Error\n");
    }

    // 更新电池电压，并显示
    can1->controller.GetValueInTable(jointBeingUsed->ID, BAT_VOLT, data_L);
    if (data_L == 0) {
        ui->BATVoltLineEdit->setText("-");
    } else {
        ui->BATVoltLineEdit->setText(QString::number((double)data_L / 100, 'f' ,2) + "V");
    }

    // 更新三轴加速度，并显示
    can1->controller.GetValueInTable(jointBeingUsed->ID, ACC_X, data_L);
    ui->AccXLineEdit->setText(QString::number((short)data_L) + "mg");
    can1->controller.GetValueInTable(jointBeingUsed->ID, ACC_Y, data_L);
    ui->AccYLineEdit->setText(QString::number((short)data_L) + "mg");
    can1->controller.GetValueInTable(jointBeingUsed->ID, ACC_Z, data_L);
    ui->AccZLineEdit->setText(QString::number((short)data_L) + "mg");

}
