#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QTextCodec>
#include <QString>
#include <QDateTime>
#include <QFileInfo>
#include <QDebug>
#include <QFileDialog>
#include <QPixmap> // 在label上显示图片
#include <numeric> // 给存ID的容器排序

using std::vector;
using std::cout;
using std::endl;
using std::sort;

void MainWindow::on_enableDriverPushButton_toggled(bool checked)
{
    qDebug() << "on_enableDriverPushButton_toggled: " << checked << endl;
}

void MainWindow::on_enableDriverPushButton_clicked()
{
    // 先更新当前的状态
    can1->controller.SendMsg(jointBeingUsed->ID,CMDTYPE_RD,SYS_ENABLE_DRIVER,NULL,0x02);
    can1->controller.delayMs(5);
    uint16_t data_L = 0;
    can1->controller.GetValueInTable(jointBeingUsed->ID, SYS_ENABLE_DRIVER, data_L);
    // 准备反转当前的状态
    bool isEnable = false;
    if (data_L != 0) {
        isEnable = false;
    } else {
        isEnable = true;
    }
    // 向下位机请求数据
//    can1->setJointEnable(jointBeingUsed,isEnable);
    jointBeingUsed->setEnable(isEnable);
    cout << "MainWindow::on_ENonPPushButton_clicked(): isEnable = " << isEnable << endl;
    // 重新更新状态，确认反转状态
    updateEnableDriver();
}

void MainWindow::updateEnableDriver()
{
    enableDriverPushButtonOn = "background-color: rgb(";
    enableDriverPushButtonOn += QString::number(ENonP_RGB_R) + ',' + QString::number(ENonP_RGB_G) + ',' + QString::number(ENonP_RGB_B) + ");";
    enableDriverPushButtonOff = tgPOSPushButtonOn;
    can1->controller.SendMsg(jointBeingUsed->ID,CMDTYPE_RD,SYS_ENABLE_DRIVER,NULL,0x02);
    can1->controller.delayMs(2);
    uint16_t data_L = 0;
    can1->controller.GetValueInTable(jointBeingUsed->ID, SYS_ENABLE_DRIVER, data_L);
//    ui->enableDriverPushButton_2->setIconSize(QSize(ui->enableDriverPushButton_2->height(),
//                                                    ui->enableDriverPushButton_2->height()));
    if (data_L != 0) {
        ui->enableDriverPushButton_2->setStyleSheet(enableDriverPushButtonOn); // 按的是enableDriverPushButton但改变颜色的是enableDriverPushButton_2
//        ui->enableDriverPushButton_2->setIcon(QIcon(":/images/images/on.png"));
        ui->enableDriverPushButton->setText("Enabled"); // 文字相应改变
    } else {
        ui->enableDriverPushButton_2->setStyleSheet(enableDriverPushButtonOff);
//        ui->enableDriverPushButton_2->setIcon(QIcon(":/images/images/off.png"));
        ui->enableDriverPushButton->setText("Disabled"); // 文字相应改变
    }
}

void MainWindow::updateIfError() {
    ifErrorPushButton_NoError = "background-color: rgb(";
    ifErrorPushButton_Error = "background-color: rgb(";
    ifErrorPushButton_NoError += QString::number(ENonP_RGB_R) + ',' + QString::number(ENonP_RGB_G) + ',' + QString::number(ENonP_RGB_B) + ");";
    ifErrorPushButton_Error = QString::number(TGPOS_RGB_R) + ',' + QString::number(TGPOS_RGB_G) + ',' + QString::number(TGPOS_RGB_B) + ");";
    // 读错误码
    uint16_t data_L = 0;
    can1->controller.SendMsg(jointBeingUsed->ID,CMDTYPE_RD,SYS_ERROR,NULL,0x02);
    can1->controller.delayMs(2);
    can1->controller.GetValueInTable(jointBeingUsed->ID, SYS_ERROR, data_L);
    if (data_L != 0) {
        ui->ifErrorPushButton->setStyleSheet(ifErrorPushButton_Error);
    } else {
        ui->ifErrorPushButton->setStyleSheet(ifErrorPushButton_NoError);
    }
}

void MainWindow::updateConnected() {
    QString connected = "background-color: rgb(";
    connected += QString::number(ENonP_RGB_R) + ',' + QString::number(ENonP_RGB_G) + ',' + QString::number(ENonP_RGB_B) + ");";
    ui->connectedPushButton->setStyleSheet(connected);
}

void MainWindow::on_btnFlash_clicked()
{
    bool isSuccess = false;
//    isSuccess = can1->JointSaveToFlash(jointBeingUsed);
    jointBeingUsed->setSaveToFlash();
    if (isSuccess) {
        QMessageBox::information(this,"提示","烧写Flash成功！");
    } else {
        QMessageBox::warning(this,"警告","烧写失败！");
    }
}

void MainWindow::on_btnUpdateID_clicked()
{
    can1->controller.updateAllID();
    updatecmbID();
}

void MainWindow::updatecmbID()
{
    // Fill the items of combo box
    ui->cmbID->clear(); // 清空之后才能正确添加
    qDebug() << "clear done";
    // 先把ID存出来，排序
    vector<uint32_t> vectID;
    for (vector<Joint>::iterator iter = can1->controller.allJoint.begin(); iter != can1->controller.allJoint.end(); ++iter) {
        vectID.push_back(iter.base()->ID);
    }
    sort(vectID.begin(), vectID.end());
    // 再把排序好的ID添加到combox
    for (vector<uint32_t>::iterator iter = vectID.begin(); iter != vectID.end(); ++iter) {
        ui->cmbID->addItem(QString::number(*iter, 10));
    }
    // combox添加好后
    if (can1->controller.allJoint.size() == 0) {
//        qDebug("The joint is not connected.\n");
        QMessageBox::warning(this,"警告","未检测到模块！");
//        exit(0); //临时的处理方式
    } else {
        // 触发cmbID的index改变的槽函数
        ui->cmbID->setCurrentIndex(can1->controller.allJoint.size() - 1); // 获取末尾ID，方便调试，index从0开始，size从1开始
    }
}

void MainWindow::on_cmbID_currentIndexChanged(const QString &arg1)
{
    jointBeingUsed = can1->findJointID(arg1.toInt()); // jointBeingUsed = ui->cmbID->currentText().toInt(); // 修改当前控制的模块ID
//    cout << jointBeingUsed << endl; // 若ID为空，jointBeingUsed也是空
    if (jointBeingUsed == NULL) {
        return;
    }
    qDebug() << "in on_cmbID_currentIndexChanged(), ID: " << jointBeingUsed->ID;

    moveInitialize();

    // 初始化示波器
    OscilloScope();
    // 初始化PID面板
    PID();
    // 初始化Set面板
    Set();
    // 初始化health面板
    health();
    // bottom的1个部分
    if (timerBottom == NULL) { // 若是首次运行
        timerBottom = new QTimer(this);
        connect(timerBottom,SIGNAL(timeout()),this,SLOT(slotTimeBottomDone()));
        timerBottom->start(BOTTOM_UPDATE_INTEVAL);
    }
    // bottom的另1个部分，4条状态的显示
    can1->controller.SendMsg(jointBeingUsed->ID,CMDTYPE_RD,SYS_MODEL_TYPE,NULL,0x02);
    can1->controller.SendMsg(jointBeingUsed->ID,CMDTYPE_RD,SYS_FW_VERSION,NULL,0x02);
    can1->controller.SendMsg(jointBeingUsed->ID,CMDTYPE_RD,SYS_REDU_RATIO,NULL,0x02);
    can1->controller.delayMs(5);
    uint16_t data_L = 0;
    can1->controller.GetValueInTable(jointBeingUsed->ID, SYS_MODEL_TYPE,data_L);
    modelTypeBeingUsed = data_L;
//    cout << "modelTypeBeingUsed: " << modelTypeBeingUsed << endl;
    QString tmp;
    switch (modelTypeBeingUsed)
    {
        case 16: tmp = "M14"; break;
        case 17: tmp = "M14E"; break;
        case 2:
        case 32: tmp = "M17"; modelTypeBeingUsed = 2; break; // CANAPI .h里只定义了2的情况，没有定义32的情况
        case 33: tmp = "M17E"; break;
        case 48: tmp = "M20"; break;
        case 64: tmp = "LIFT"; break;
    }
    ui->typeLabel->setText("TYPE: " + tmp);
    can1->controller.GetValueInTable(jointBeingUsed->ID, SYS_FW_VERSION,data_L);
    ui->firmLabel->setText("FIRM: " + QString::number(data_L,10));
    can1->controller.GetValueInTable(jointBeingUsed->ID, SYS_REDU_RATIO,data_L);
    ui->ratioLabel->setText("RATIO:" + QString::number(data_L,10));
    // bottom中的connect按钮
    ui->btnConnect->setVisible(false);

}

void MainWindow::updateWorkModePushButton() {
    QString background = "background-color: rgb(";
    background += QString::number(ENonP_RGB_R) + ',' + QString::number(ENonP_RGB_G) + ',' + QString::number(ENonP_RGB_B) + ");";
    ui->workModePushButton->setStyleSheet(background);

    int workMode = ui->cmbWorkMode->currentIndex();
    switch(workMode) {// 工作模式修改后，修改txtBias
        case MODE_OPEN:
            ui->workModePushButton->setText("OPEN");
            break;
        case MODE_CURRENT:
            ui->workModePushButton->setText("CURRENT");
            break;
        case MODE_SPEED:
            ui->workModePushButton->setText("SPEED");
            break;
        case MODE_POSITION:
            ui->workModePushButton->setText("POSITION");
            break;
        default: break;
    }
}

void MainWindow::slotTimeBottomDone()
{
    if (jointBeingUsed == NULL) {
        return;
    }

    updateEnableDriver();
    updateIfError(); // 检查是否发生了错误
    updateConnected(); // 把按钮变成绿色
    updateWorkModePushButton(); // 把按钮变成绿色，并且显示工作模式
}

void MainWindow::on_btnSave_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("保存PID数据"), "./PID.dat", tr("PID数据文件(*.dat)"));
//    qDebug() << fileName << endl;
    if (fileName == "")
        return;
    QFile file(fileName);
    if (! file.open(QIODevice::WriteOnly | QIODevice::Text))
        qDebug() << file.errorString();
    int oldIndex = ui->adjustGroupComboBox->currentIndex();
    ui->adjustGroupComboBox->setCurrentIndex(0);
//    on_adjustGroupComboBox_currentIndexChanged(0);
    QString data = ui->POS_PSpinBox->text() + tr("\n")
                 + ui->POS_ISpinBox->text() + tr("\n")
                 + ui->POS_DSpinBox->text() + tr("\n")
                 + ui->POS_DSSpinBox->text() + tr("\n")
                 + ui->SPD_PSpinBox->text() + tr("\n")
                 + ui->SPD_ISpinBox->text() + tr("\n")
                 + ui->SPD_DSpinBox->text() + tr("\n")
                 + ui->SPD_DSSpinBox->text() + tr("\n")
                 + ui->CUR_PSpinBox->text() + tr("\n")
                 + ui->CUR_ISpinBox->text() + tr("\n")
                 + ui->CUR_DSpinBox->text() + tr("\n"); // 第1组完成，需要切换组别
    ui->adjustGroupComboBox->setCurrentIndex(1);
//    on_adjustGroupComboBox_currentIndexChanged(1);
    data += ui->POS_PSpinBox->text() + tr("\n")
                 + ui->POS_ISpinBox->text() + tr("\n")
                 + ui->POS_DSpinBox->text() + tr("\n")
                 + ui->POS_DSSpinBox->text() + tr("\n")
                 + ui->SPD_PSpinBox->text() + tr("\n")
                 + ui->SPD_ISpinBox->text() + tr("\n")
                 + ui->SPD_DSpinBox->text() + tr("\n")
                 + ui->SPD_DSSpinBox->text() + tr("\n")
                 + ui->CUR_PSpinBox->text() + tr("\n")
                 + ui->CUR_ISpinBox->text() + tr("\n")
                 + ui->CUR_DSpinBox->text() + tr("\n"); // 第2组完成，需要切换组别
    ui->adjustGroupComboBox->setCurrentIndex(2);
//    on_adjustGroupComboBox_currentIndexChanged(2);
    data += ui->POS_PSpinBox->text() + tr("\n")
                 + ui->POS_ISpinBox->text() + tr("\n")
                 + ui->POS_DSpinBox->text() + tr("\n")
                 + ui->POS_DSSpinBox->text() + tr("\n")
                 + ui->SPD_PSpinBox->text() + tr("\n")
                 + ui->SPD_ISpinBox->text() + tr("\n")
                 + ui->SPD_DSpinBox->text() + tr("\n")
                 + ui->SPD_DSSpinBox->text() + tr("\n")
                 + ui->CUR_PSpinBox->text() + tr("\n")
                 + ui->CUR_ISpinBox->text() + tr("\n")
                 + ui->CUR_DSpinBox->text(); // 第3组完成，需要切换回组别
    ui->adjustGroupComboBox->setCurrentIndex(oldIndex);
//    on_adjustGroupComboBox_currentIndexChanged(oldIndex);
    QByteArray ba = data.toLatin1();
    file.write(ba);
    file.close();
    QMessageBox::information(this,"提示","PID数值已保存。");

}

void MainWindow::on_btnLoad_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("打开PID数据文件"),".",tr("PID数据文件(*.dat)"));
    if (fileName == "")
        return;
    QFile file(fileName);
    if (! file.open(QIODevice::ReadOnly | QIODevice::Text))
        qDebug() << file.errorString();
    file.seek(0);
    QTextStream in(&file);
    int count = 0;
    int oldIndex = ui->adjustGroupComboBox->currentIndex(); // 保存当前的组别
    cout << oldIndex << endl;
    while (! in.atEnd()) {
        QString line = in.readLine();
        switch (count) {
            case 0: { // 不是on_adjustGroupComboBox_currentIndexChanged(0);
                ui->adjustGroupComboBox->setCurrentIndex(0);
                cout << "on 0" << endl;
                ui->POS_PSpinBox->setValue(line.toInt());
                on_POS_PSpinBox_editingFinished();
                break;
            }
            case 1: {
                ui->POS_ISpinBox->setValue(line.toInt());
                on_POS_ISpinBox_editingFinished();
                break;
            }
            case 2: {
                ui->POS_DSpinBox->setValue(line.toInt());
                on_POS_DSpinBox_editingFinished();
                break;
            }
            case 3: {
                ui->POS_DSSpinBox->setValue(line.toInt());
                on_POS_DSSpinBox_editingFinished();
                break;
            }
            case 4: {
                ui->SPD_PSpinBox->setValue(line.toInt());
                on_SPD_PSpinBox_editingFinished();
                break;
            }
            case 5: {
                ui->SPD_ISpinBox->setValue(line.toInt());
                on_SPD_ISpinBox_editingFinished();
                break;
            }
            case 6: {
                ui->SPD_DSpinBox->setValue(line.toInt());
                on_SPD_DSpinBox_editingFinished();
                break;
            }
            case 7: {
                ui->SPD_DSSpinBox->setValue(line.toInt());
                on_SPD_DSSpinBox_editingFinished();
                break;
            }
            case 8: {
                ui->CUR_PSpinBox->setValue(line.toInt());
                on_CUR_PSpinBox_editingFinished();
                break;
            }
            case 9: {
                ui->CUR_ISpinBox->setValue(line.toInt());
                on_CUR_ISpinBox_editingFinished();
                break;
            }
            case 10: {
                ui->CUR_DSpinBox->setValue(line.toInt());
                on_CUR_DSpinBox_editingFinished();
                break;
            }
            case 11: { // 不是on_adjustGroupComboBox_currentIndexChanged(1);
                ui->adjustGroupComboBox->setCurrentIndex(1);
                cout << "on 1" << endl;
                ui->POS_PSpinBox->setValue(line.toInt());
                on_POS_PSpinBox_editingFinished();
                break;
            }
            case 12: {
                ui->POS_ISpinBox->setValue(line.toInt());
                on_POS_ISpinBox_editingFinished();
                break;
            }
            case 13: {
                ui->POS_DSpinBox->setValue(line.toInt());
                on_POS_DSpinBox_editingFinished();
                break;
            }
            case 14: {
                ui->POS_DSSpinBox->setValue(line.toInt());
                on_POS_DSSpinBox_editingFinished();
                break;
            }
            case 15: {
                ui->SPD_PSpinBox->setValue(line.toInt());
                on_SPD_PSpinBox_editingFinished();
                break;
            }
            case 16: {
                ui->SPD_ISpinBox->setValue(line.toInt());
                on_SPD_ISpinBox_editingFinished();
                break;
            }
            case 17: {
                ui->SPD_DSpinBox->setValue(line.toInt());
                on_SPD_DSpinBox_editingFinished();
                break;
            }
            case 18: {
                ui->SPD_DSSpinBox->setValue(line.toInt());
                on_SPD_DSSpinBox_editingFinished();
                break;
            }
            case 19: {
                ui->CUR_PSpinBox->setValue(line.toInt());
                on_CUR_PSpinBox_editingFinished();
                break;
            }
            case 20: {
                ui->CUR_ISpinBox->setValue(line.toInt());
                on_CUR_ISpinBox_editingFinished();
                break;
            }
            case 21: {
                ui->CUR_DSpinBox->setValue(line.toInt());
                on_CUR_DSpinBox_editingFinished();
                break;
            }
            case 22: { // 不是on_adjustGroupComboBox_currentIndexChanged(2);
                ui->adjustGroupComboBox->setCurrentIndex(2);
                cout << "on 2" << endl;
                ui->POS_PSpinBox->setValue(line.toInt());
                on_POS_PSpinBox_editingFinished();
                break;
            }
            case 23: {
                ui->POS_ISpinBox->setValue(line.toInt());
                on_POS_ISpinBox_editingFinished();
                break;
            }
            case 24: {
                ui->POS_DSpinBox->setValue(line.toInt());
                on_POS_DSpinBox_editingFinished();
                break;
            }
            case 25: {
                ui->POS_DSSpinBox->setValue(line.toInt());
                on_POS_DSSpinBox_editingFinished();
                break;
            }
            case 26: {
                ui->SPD_PSpinBox->setValue(line.toInt());
                on_SPD_PSpinBox_editingFinished();
                break;
            }
            case 27: {
                ui->SPD_ISpinBox->setValue(line.toInt());
                on_SPD_ISpinBox_editingFinished();
                break;
            }
            case 28: {
                ui->SPD_DSpinBox->setValue(line.toInt());
                on_SPD_DSpinBox_editingFinished();
                break;
            }
            case 29: {
                ui->SPD_DSSpinBox->setValue(line.toInt());
                on_SPD_DSSpinBox_editingFinished();
                break;
            }
            case 30: {
                ui->CUR_PSpinBox->setValue(line.toInt());
                on_CUR_PSpinBox_editingFinished();
                break;
            }
            case 31: {
                ui->CUR_ISpinBox->setValue(line.toInt());
                on_CUR_ISpinBox_editingFinished();
                break;
            }
            case 32: {
                ui->CUR_DSpinBox->setValue(line.toInt());
                on_CUR_DSpinBox_editingFinished();
                break;
            }
        }

//        qDebug() << line;
        ++count;
    }
//    qDebug() << QObject::tr("文件内容：") << endl << file.readAll();
    file.close();

    ui->adjustGroupComboBox->setCurrentIndex(oldIndex); // 切换回组别
    QMessageBox::information(this,"提示","PID数值已载入。");
}

// 未完成
void MainWindow::on_btnConnect_clicked()
{
//    connectForm* c = new connectForm(this);
//    c->show();
}
