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


    QPoint Monitor = ui->Monitor->pos();
    QPoint Set = ui->SET->pos();

    QRect ENonPPushButton = QRect(ui->ENonPPushButton->pos() + centralWidget + Monitor + Set, ui->ENonPPushButton->size());
    QRect adjustGroupComboBox = QRect(ui->adjustGroupComboBox->pos() + centralWidget, ui->adjustGroupComboBox->size());
    QRect scopeEnablePushButton = QRect(ui->scopeEnablePushButton->pos(), ui->scopeEnablePushButton->size());
//    QRect ScanFrequencyComboBox = QRect(ui->ScanFrequencyComboBox->pos(), ui->ScanFrequencyComboBox->size());

    cout << "e->pos(): (" << e->pos().x() << "," << e->pos().y() << ")" << endl;
//    cout << "TestRun->pos(): (" << TestRun.x() << "," << TestRun.y() << ")" << endl;

//    cout << "AmplitudeLineEdit: (" << AmplitudeLineEdit.x() << "," << AmplitudeLineEdit.y() << ")" << endl;
//    cout << "ENonPPushButton: (" << ENonPPushButton.x() << "," << ENonPPushButton.y() << ")" << endl;
//    cout << "ScanFrequencyCombBox: (" << ScanFrequencyComboBox.x() << "," << ScanFrequencyComboBox.y() << ")" << endl;
    cout.flush();
    if(ENonPPushButton.contains(e->pos())) {
        ui->helpTextEdit->setText(ui->ENonPPushButton->toolTip());
        return;
    }
    if(adjustGroupComboBox.contains(e->pos())) {
        ui->helpTextEdit->setText(ui->adjustGroupComboBox->toolTip());
        return;
    }
    if(scopeEnablePushButton.contains(e->pos())) {
        ui->helpTextEdit->setText(ui->scopeEnablePushButton->toolTip());
        return;
    }
//    if(ScanFrequencyComboBox.contains(e->pos())) {
//        ui->helpTextEdit->setText(ui->ScanFrequencyComboBox->toolTip());
//        return;
//    } // 布局里的mouseMoveEvent就不能正常响应了。
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

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    if (e->modifiers() == (Qt::ShiftModifier | Qt::ControlModifier | Qt::AltModifier) && e->key() == Qt::Key_F7) {
        if (QApplication::mouseButtons() == Qt::LeftButton)
            QMessageBox::information(this,"提示","命令发送高级模式！");
    }
}
