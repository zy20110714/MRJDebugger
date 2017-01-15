#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "connectform.h"
#include <QMessageBox>
#include <QMouseEvent>

using std::cout;
using std::endl;

void MainWindow::support()
{
    // 设置要显示tooltip的控件的鼠标事件
    // this->setMouseTracking(true);
    // ui->centralWidget->setMouseTracking(true);
    // ui->adjustGroupComboBox->setMouseTracking(true);
}

void MainWindow::mouseMoveEvent(QMouseEvent *e)
{
    QPoint centralWidget = ui->centralWidget->pos(); // (0,0)

    // TestRun中的控件
    QPoint TestRun = ui->TestRun->pos();
    QRect AMPLabel = QRect(ui->AMPLabel->pos() + centralWidget + TestRun, ui->AMPLabel->size());
    // QRect AmplitudeLineEdit = QRect(ui->AmplitudeLineEdit->pos() + centralWidget + TestRun, ui->AmplitudeLineEdit->size());
    QRect BIASLabel = QRect(ui->BIASLabel->pos() + centralWidget + TestRun, ui->BIASLabel->size());
    QRect FREQLabel = QRect(ui->FREQLabel->pos() + centralWidget + TestRun, ui->FREQLabel->size());
    QRect MODELabel = QRect(ui->MODELabel->pos() + centralWidget + TestRun, ui->MODELabel->size());
    QRect TARGETLabel = QRect(ui->TARGETLabel->pos() + centralWidget + TestRun, ui->TARGETLabel->size());
    QRect cmbWorkMode = QRect(ui->cmbWorkMode->pos() + centralWidget + TestRun, ui->cmbWorkMode->size());
    QRect confirmButton = QRect(ui->confirmButton->pos() + centralWidget + TestRun, ui->confirmButton->size());
    // QRect frequencyLineEdit = QRect(ui->frequencyLineEdit->pos() + centralWidget + TestRun, ui->frequencyLineEdit->size());
    // QRect manualMax = QRect(ui->manualMax->pos() + centralWidget + TestRun, ui->manualMax->size());
    // QRect manualMin = QRect(ui->manualMin->pos() + centralWidget + TestRun, ui->manualMin->size());
    QRect manualSlider = QRect(ui->manualSlider->pos() + centralWidget + TestRun, ui->manualSlider->size());
    QRect stopButton = QRect(ui->stopButton->pos() + centralWidget + TestRun, ui->stopButton->size());
    QRect txtBias = QRect(ui->txtBias->pos() + centralWidget + TestRun, ui->txtBias->size());
    QRect waveModeCombo = QRect(ui->waveModeCombo->pos() + centralWidget + TestRun, ui->waveModeCombo->size());
    if(AMPLabel.contains(e->pos())) {
        ui->helpTextEdit->setText(ui->AMPLabel->toolTip());
        return;
    }
    if(BIASLabel.contains(e->pos())) {
        ui->helpTextEdit->setText(ui->BIASLabel->toolTip());
        return;
    }
    if(FREQLabel.contains(e->pos())) {
        ui->helpTextEdit->setText(ui->FREQLabel->toolTip());
        return;
    }
    if(MODELabel.contains(e->pos())) {
        ui->helpTextEdit->setText(ui->MODELabel->toolTip());
        return;
    }
    if(TARGETLabel.contains(e->pos())) {
        ui->helpTextEdit->setText(ui->TARGETLabel->toolTip());
        return;
    }
    if(cmbWorkMode.contains(e->pos())) {
        ui->helpTextEdit->setText(ui->cmbWorkMode->toolTip());
        return;
    }
    if(confirmButton.contains(e->pos())) {
        ui->helpTextEdit->setText(ui->confirmButton->toolTip());
        return;
    }
    if(manualSlider.contains(e->pos())) {
        ui->helpTextEdit->setText(ui->manualSlider->toolTip());
        return;
    }
    if(stopButton.contains(e->pos())) {
        ui->helpTextEdit->setText(ui->stopButton->toolTip());
        return;
    }
    if(txtBias.contains(e->pos())) {
        ui->helpTextEdit->setText(ui->txtBias->toolTip());
        return;
    }
    if(waveModeCombo.contains(e->pos())) {
        ui->helpTextEdit->setText(ui->waveModeCombo->toolTip());
        return;
    }

    // 示波器中的控件
    QRect ScanFrequencyComboBox = QRect(ui->ScanFrequencyComboBox->pos() + centralWidget, ui->ScanFrequencyComboBox->size());
    if(ScanFrequencyComboBox.contains(e->pos())) {
        ui->helpTextEdit->setText(ui->ScanFrequencyComboBox->toolTip());
        return;
    }
    QRect crComboBox = QRect(ui->crComboBox->pos() + centralWidget, ui->crComboBox->size());
    if(crComboBox.contains(e->pos())) {
        ui->helpTextEdit->setText(ui->crComboBox->toolTip());
        return;
    }
    QRect prComboBox = QRect(ui->prComboBox->pos() + centralWidget, ui->prComboBox->size());
    if(prComboBox.contains(e->pos())) {
        ui->helpTextEdit->setText(ui->prComboBox->toolTip());
        return;
    }
    QRect rlCURPushButton = QRect(ui->rlCURPushButton->pos() + centralWidget, ui->rlCURPushButton->size());
    if(rlCURPushButton.contains(e->pos())) {
        ui->helpTextEdit->setText(ui->rlCURPushButton->toolTip());
        return;
    }
    QRect rlPOSPushButton = QRect(ui->rlPOSPushButton->pos() + centralWidget, ui->rlPOSPushButton->size());
    if(rlPOSPushButton.contains(e->pos())) {
        ui->helpTextEdit->setText(ui->rlPOSPushButton->toolTip());
        return;
    }
    QRect rlSPDPushButton = QRect(ui->rlSPDPushButton->pos() + centralWidget, ui->rlSPDPushButton->size());
    if(rlSPDPushButton.contains(e->pos())) {
        ui->helpTextEdit->setText(ui->rlSPDPushButton->toolTip());
        return;
    }
    QRect tgCURPushButton = QRect(ui->tgCURPushButton->pos() + centralWidget, ui->tgCURPushButton->size());
    if(tgCURPushButton.contains(e->pos())) {
        ui->helpTextEdit->setText(ui->tgCURPushButton->toolTip());
        return;
    }
    QRect tgPOSPushButton = QRect(ui->tgPOSPushButton->pos() + centralWidget, ui->tgPOSPushButton->size());
    if(tgPOSPushButton.contains(e->pos())) {
        ui->helpTextEdit->setText(ui->tgPOSPushButton->toolTip());
        return;
    }
    QRect tgSPDPushButton = QRect(ui->tgSPDPushButton->pos() + centralWidget, ui->tgSPDPushButton->size());
    if(tgSPDPushButton.contains(e->pos())) {
        ui->helpTextEdit->setText(ui->tgSPDPushButton->toolTip());
        return;
    }
    QRect scopeEnablePushButton = QRect(ui->scopeEnablePushButton->pos() + centralWidget, ui->scopeEnablePushButton->size());
    if(scopeEnablePushButton.contains(e->pos())) {
        ui->helpTextEdit->setText(ui->scopeEnablePushButton->toolTip());
        return;
    }
    QRect srComboBox = QRect(ui->srComboBox->pos() + centralWidget, ui->srComboBox->size());
    if(srComboBox.contains(e->pos())) {
        ui->helpTextEdit->setText(ui->srComboBox->toolTip());
        return;
    }

    // PID中的控件
    QRect adjustGroupComboBox = QRect(ui->adjustGroupComboBox->pos() + centralWidget, ui->adjustGroupComboBox->size());
    if(adjustGroupComboBox.contains(e->pos())) {
        ui->helpTextEdit->setText(ui->adjustGroupComboBox->toolTip());
        return;
    }
    QRect limitAccLabel = QRect(ui->limitAccLabel->pos() + centralWidget, ui->limitAccLabel->size());
    if(limitAccLabel.contains(e->pos())) {
        ui->helpTextEdit->setText(ui->limitAccLabel->toolTip());
        return;
    }
    QRect limitCurLabel = QRect(ui->limitCurLabel->pos() + centralWidget, ui->limitCurLabel->size());
    if(limitCurLabel.contains(e->pos())) {
        ui->helpTextEdit->setText(ui->limitCurLabel->toolTip());
        return;
    }
    QRect limitSpdLabel = QRect(ui->limitSpdLabel->pos() + centralWidget, ui->limitSpdLabel->size());
    if(limitSpdLabel.contains(e->pos())) {
        ui->helpTextEdit->setText(ui->limitSpdLabel->toolTip());
        return;
    }
    // 布局里的mouseMoveEvent就不能正常响应了。
    // QRect AMPLabel = QRect(ui->AMPLabel->pos() + centralWidget + TestRun, ui->AMPLabel->size());
    // if(AMPLabel.contains(e->pos())) {
    //     ui->helpTextEdit->setText(ui->AMPLabel->toolTip());
    //     return;
    // }

    // Monitor中的控件
    // QPoint Monitor = ui->Monitor->pos();
    // QPoint Set = ui->SET->pos();
    // QRect ENonPPushButton = QRect(ui->ENonPPushButton->pos() + centralWidget + Monitor + Set, ui->ENonPPushButton->size());
    // if(ENonPPushButton.contains(e->pos())) {
    //     ui->helpTextEdit->setText(ui->ENonPPushButton->toolTip());
    //     return;
    // }

    // Bar中的控件
    QPoint Bar = ui->Bar->pos();
    QRect btnFlash = QRect(ui->btnFlash->pos() + centralWidget + Bar, ui->btnFlash->size());
    if(btnFlash.contains(e->pos())) {
        ui->helpTextEdit->setText(ui->btnFlash->toolTip());
        return;
    }
    QRect cmbID = QRect(ui->cmbID->pos() + centralWidget + Bar, ui->cmbID->size());
    if(cmbID.contains(e->pos())) {
        ui->helpTextEdit->setText(ui->cmbID->toolTip());
        return;
    }
    QRect firmLabel = QRect(ui->firmLabel->pos() + centralWidget + Bar, ui->firmLabel->size());
    if(firmLabel.contains(e->pos())) {
        ui->helpTextEdit->setText(ui->firmLabel->toolTip());
        return;
    }
    QRect helpLabel = QRect(ui->helpLabel->pos() + centralWidget + Bar, ui->helpLabel->size());
    if(helpLabel.contains(e->pos())) {
        ui->helpTextEdit->setText(ui->helpLabel->toolTip());
        return;
    }
    QRect ratioLabel = QRect(ui->ratioLabel->pos() + centralWidget + Bar, ui->ratioLabel->size());
    if(ratioLabel.contains(e->pos())) {
        ui->helpTextEdit->setText(ui->ratioLabel->toolTip());
        return;
    }
    QRect typeLabel = QRect(ui->typeLabel->pos() + centralWidget + Bar, ui->typeLabel->size());
    if(typeLabel.contains(e->pos())) {
        ui->helpTextEdit->setText(ui->typeLabel->toolTip());
        return;
    }

    // 测试信息输出
    // cout << "e->pos(): (" << e->pos().x() << "," << e->pos().y() << ")" << endl;
    // cout << "TestRun->pos(): (" << TestRun.x() << "," << TestRun.y() << ")" << endl;
    // cout << "AmplitudeLineEdit: (" << AmplitudeLineEdit.x() << "," << AmplitudeLineEdit.y() << ")" << endl;
    // cout << "ENonPPushButton: (" << ENonPPushButton.x() << "," << ENonPPushButton.y() << ")" << endl;
    // cout << "ScanFrequencyCombBox: (" << ScanFrequencyComboBox.x() << "," << ScanFrequencyComboBox.y() << ")" << endl;
    // cout.flush();
}

void MainWindow::on_btnFlash_clicked()
{
    bool isSuccess = false;
    isSuccess = can1->JointSaveToFlash(jointBeingUsed);
    if (isSuccess) {
        QMessageBox::information(this,"提示","烧写Flash成功！");
    } else {
        QMessageBox::warning(this,"警告","烧写失败！");
    }
}

void MainWindow::on_clearErrorButton_clicked()
{
    can1->JointClearErrStatus(jointBeingUsed);
    cout << "MainWindow::on_clearErrorButton_clicked(): done."<< endl;
    cout.flush();
}

void MainWindow::on_enableDriverPushButton_clicked()
{
    // 先更新当前的状态
    can1->JointSendMsg(jointBeingUsed,CMDTYPE_RD,SYS_ENABLE_DRIVER,NULL,0x02);
    usleep(5000);
    uint16_t data_L = 0;
    can1->JointReadMsg(jointBeingUsed, SYS_ENABLE_DRIVER, data_L);
    // 准备反转当前的状态
    // int value = 0;
    bool isEnable = false;
    if (data_L != 0) {
        // value = 0;
        isEnable = false;
    } else {
        // value = 1;
        isEnable = true;
    }
    // 向下位机请求数据
    // uint8_t data[2] = {0,0};
    // data[1] = (uint8_t)( (value & 0xff00) >> 8 );
    // data[0] = (uint8_t)( value & 0xff );
    // can1->JointSendMsg(jointBeingUsed, CMDTYPE_WR_NR, SYS_ENABLE_DRIVER, data, 2);
    // usleep(1000);
    can1->setJointEnable(jointBeingUsed,isEnable);
//    cout << "MainWindow::on_ENonPPushButton_clicked(): value = " << value << endl;
    cout << "MainWindow::on_ENonPPushButton_clicked(): isEnable = " << isEnable << endl;
    cout.flush();
    // 重新更新状态，确认反转状态
    updateEnableDriver();
}

void MainWindow::updateEnableDriver()
{
    can1->JointSendMsg(jointBeingUsed,CMDTYPE_RD,SYS_ENABLE_DRIVER,NULL,0x02);
    usleep(5000);
    uint16_t data_L = 0;
    can1->JointReadMsg(jointBeingUsed, SYS_ENABLE_DRIVER, data_L);
    if (data_L != 0) {
        ui->enableDriverPushButton->setStyleSheet(enableDriverPushButtonOn);
    } else {
        ui->enableDriverPushButton->setStyleSheet(enableDriverPushButtonOff);
    }
}

void MainWindow::on_updateButton_clicked()
{
    slotTimeMonitorDone();
}

void MainWindow::slotTimeMonitorDone()
{
    updateEnableDriver();

    can1->JointSendMsg(jointBeingUsed,CMDTYPE_RD,SYS_VOLTAGE,NULL,0x02);
    usleep(1000);
    can1->JointSendMsg(jointBeingUsed,CMDTYPE_RD,SYS_CURRENT_L,NULL,0x04);
    usleep(1000);
    can1->JointSendMsg(jointBeingUsed,CMDTYPE_RD,SYS_TEMP,NULL,0x02);
    usleep(1000);
    can1->JointSendMsg(jointBeingUsed,CMDTYPE_RD,SYS_SPEED_L,NULL,0x04);
    usleep(5000);

    uint16_t data_L = 0;
    uint16_t data_H = 0;

    can1->JointReadMsg(jointBeingUsed, SYS_VOLTAGE,data_L);
    ui->VolLineEdit->setText(QString::number((double)data_L / 100,'g',4) + "V");

    can1->JointReadMsg(jointBeingUsed, SYS_CURRENT_L, data_L);
    can1->JointReadMsg(jointBeingUsed, SYS_CURRENT_H, data_H);
    double temp = data_L + (data_H * 65536);
    ui->CurLineEdit->setText(QString::number(temp / 1000, 'g',4) + "A");

    can1->JointReadMsg(jointBeingUsed, SYS_TEMP,data_L);
    ui->TempLineEdit->setText(QString::number((double)data_L / 10, 'g',4) + "℃");

    can1->JointReadMsg(jointBeingUsed, SYS_SPEED_L, data_L);
    can1->JointReadMsg(jointBeingUsed, SYS_SPEED_H, data_H);
    temp = data_L + (data_H * 65536);
    temp *= 60; // rpm
    temp /= 65536; // rpm
    temp /= GEAR_RATIO_M14;
    ui->SpeedLineEdit->setText(QString::number(temp, 'g',4) + "rpm");

}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    if (e->modifiers() == (Qt::ShiftModifier | Qt::ControlModifier | Qt::AltModifier) && e->key() == Qt::Key_F7) {
        if (QApplication::mouseButtons() == Qt::LeftButton)
            QMessageBox::information(this,"提示","命令发送高级模式！");
    }
}
