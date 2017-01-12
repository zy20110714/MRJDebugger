#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPen>
#include <math.h>
#include <QtGlobal>
#include <QString>

using std::cout;
using std::endl;

// QString tgPOSPushButtonOn = "background-color: rgb(";
// QString tgPOSPushButtonOff = "background-color: rgb(128, 128, 128);";
// QString tgSPDPushButtonOn = "background-color: rgb(";
// QString tgSPDPushButtonOff = tgPOSPushButtonOff;
// QString tgCURPushButtonOn = "background-color: rgb(";
// QString tgCURPushButtonOff = tgPOSPushButtonOff;
// QString rlPOSPushButtonOn = "background-color: rgb(";
// QString rlPOSPushButtonOff = tgPOSPushButtonOff;
// QString rlSPDPushButtonOn = "background-color: rgb(";
// QString rlSPDPushButtonOff = tgPOSPushButtonOff;
// QString rlCURPushButtonOn = "background-color: rgb(";
// QString rlCURPushButtonOff = tgPOSPushButtonOff;

// QString scopeEnablePushButtonOn = "background-color: rgb(";
// QString scopeEnablePushButtonOff = "background-color: rgb(";

void MainWindow::OscilloScope()
{
    timeOscilloScope = new QTimer(this);
    connect(timeOscilloScope,SIGNAL(timeout()),this,SLOT(slotTimeOscilloScopeDone()));
    timeOscilloScope->start(OSCILLO_SCOPE_INTEVAL);

    paintArea = new PaintArea(this);
    ui->gridLayout_OS->addWidget(paintArea);

    // 记录对象标志MASK的初始化
    can1->JointSendMsg(jointBeingUsed,CMDTYPE_RD,SCP_MASK,NULL,0x02);
    usleep(5000);
    uint16_t data_L = 0;
    can1->JointReadMsg(jointBeingUsed, SCP_MASK,data_L);
    paintArea->Mask = data_L;

    //参数表中的“记录时间间隔（对10kHZ的分频值）”显示到测量条件选项卡中的对应控件里
    can1->JointSendMsg(jointBeingUsed,CMDTYPE_RD,SCP_REC_TIM,NULL,0x02);
    usleep(5000);
//    uint16_t data_L = 0;
    can1->JointReadMsg(jointBeingUsed, SCP_REC_TIM,data_L);
    paintArea->ScanFrequency = data_L;
//    ui->ScanFrequencyComboBox->setCurrentIndex();
    //同样，用参数表中的“记录时间间隔（对10kHZ的分频值）”设置扫描频率，初始化Interval
    paintArea->Interval = paintArea->ScanFrequency / 10 / OSCILLO_SCOPE_INTEVAL; // think about frequency division factor of 10 kHz(ScanFrequency / 10 = Interval * OSCILLO_SCOPE_INTEVAL)

    // Initializes the color of the buttons
    if ((paintArea->Mask & MASK_TAGPOS) != 0x00) {
        ui->tgPOSPushButton->setStyleSheet(tgPOSPushButtonOn);
    } else {
        ui->tgPOSPushButton->setStyleSheet(tgPOSPushButtonOff);
    }
    if ((paintArea->Mask & MASK_TAGSPD) != 0x00) {
        ui->tgSPDPushButton->setStyleSheet(tgSPDPushButtonOn);
    } else {
        ui->tgSPDPushButton->setStyleSheet(tgSPDPushButtonOff);
    }
    if ((paintArea->Mask & MASK_TAGCUR) != 0x00) {
        ui->tgCURPushButton->setStyleSheet(tgCURPushButtonOn);
    } else {
        ui->tgCURPushButton->setStyleSheet(tgCURPushButtonOff);
    }
    if ((paintArea->Mask & MASK_MEACUR) != 0x00) {
        ui->rlCURPushButton->setStyleSheet(rlCURPushButtonOn);
    } else {
        ui->rlCURPushButton->setStyleSheet(rlCURPushButtonOff);
    }
    if ((paintArea->Mask & MASK_MEASPD) != 0x00) {
        ui->rlSPDPushButton->setStyleSheet(rlSPDPushButtonOn);
    } else {
        ui->rlSPDPushButton->setStyleSheet(rlSPDPushButtonOff);
    }
    if ((paintArea->Mask & MASK_MEAPOS) != 0x00) {
        ui->rlPOSPushButton->setStyleSheet(rlPOSPushButtonOn);
    } else {
        ui->rlPOSPushButton->setStyleSheet(rlPOSPushButtonOff);
    }

    // Initializes the color of the enable button
    if (paintArea->EnableScope) {
        ui->scopeEnablePushButton->setStyleSheet(scopeEnablePushButtonOn);
        ui->scopeEnablePushButton->setText("▹");
    } else {
        ui->scopeEnablePushButton->setStyleSheet(scopeEnablePushButtonOff);
        ui->scopeEnablePushButton->setText("II");
    }

    // Initializes the pr cr sr
    on_prComboBox_currentIndexChanged(ui->prComboBox->currentIndex());
//    on_crComboBox_currentIndexChanged(ui->crComboBox->currentIndex());
    ui->crComboBox->setCurrentIndex(1); // 50mA/div
    on_srComboBox_currentIndexChanged(ui->srComboBox->currentIndex());
}

void MainWindow::slotTimeOscilloScopeDone()
{
    //示波器显示曲线用的计数量
    static int gatherCount = 0;

    //示波器绘制曲线使能开启
    if (paintArea->EnableScope) {
        //间隔计数会从1开始判断
        gatherCount++;
        //按照Interval限定的间隔去执行
        if (gatherCount >= paintArea->Interval) {
            //间隔计数清零
            gatherCount = 0;
            //分别处理每个显示项
            for (int i = 0; i < paintArea->showItems.size(); i++) {
                //显示项的显示队列不为0才向队列追加一个新值，该32位有符号值由2个16位有符号数组合而成
                if (paintArea->showItems[i].IsCheck) {
                    can1->JointSendMsg(jointBeingUsed,CMDTYPE_RD,paintArea->showItems[i].Item,NULL,0x04);
                }
            }
            usleep(5000);
            for (int i = 0; i < paintArea->showItems.size(); i++) {
                if (paintArea->showItems[i].sq->MaxLength != 0 && paintArea->showItems[i].IsCheck) {
                    uint16_t data_L = 0;
                    uint16_t data_H = 0;
                    can1->JointReadMsg(jointBeingUsed, paintArea->showItems[i].Item, data_L);
                    can1->JointReadMsg(jointBeingUsed, paintArea->showItems[i].Item + 1, data_H);
        //                    temp = (int)(data_L + data_H * 65536);
                    double temp = data_L + (data_H * 65536);
        //                    cout << temp << endl;
        //                    cout.flush();

        //                    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
        //                    temp = qrand()%10;
        //                    cout << temp << endl;
        //                    cout.flush();

                    switch (paintArea->showItems[i].Item) {
                    case TAG_CURRENT_L://(showItems[0].Item)://SCP_TAGCUR_L:
                    case SYS_CURRENT_L://showItems[3].Item://SCP_MEACUR_L://若曲线是电流
                        break;
                    case TAG_SPEED_L://showItems[1].Item://SCP_TAGSPD_L:
                    case SYS_SPEED_L://showItems[4].Item://SCP_MEASPD_L://若曲线是速度
                        temp *= 60; // rpm
                        temp /= 65536; // rpm
                        temp /= GEAR_RATIO_M14;
                        break;
                    case TAG_POSITION_L://showItems[2].Item://SCP_TAGPOS_L:
                    case SYS_POSITION_L://showItems[5].Item://SCP_MEAPOS_L://若曲线是位置
                        break;
                    }

                    //向队尾追加值
                    paintArea->showItems[i].sq->Append(temp);
        //                    cout << "paintArea->showItems[" << i << "].sq: " << paintArea->showItems[i].sq->GetValue(paintArea->showItems[i].sq->Count() - 1) << endl;
        //                    cout.flush();
                    paintArea->update();
                }
            }
        }
    }
}

void MainWindow::setMask()
{
    // 向下位机请求数据
    uint8_t data[2] = {0,0};
    data[1] = (uint8_t)( (paintArea->Mask & 0xff00) >> 8 );
    data[0] = (uint8_t)( paintArea->Mask & 0xff );
    can1->JointSendMsg(jointBeingUsed, CMDTYPE_WR_NR, SCP_MASK, data, 2);
//    usleep(5000);
    cout << "MainWindow::setMask(): paintArea->Mask = " << paintArea->Mask << endl;
    cout.flush();
    can1->JointSendMsg(jointBeingUsed,CMDTYPE_RD,SCP_MASK,NULL,0x02);
    usleep(5000);
//    can1->JointSendMsg(jointBeingUsed,CMDTYPE_RD,SCP_TAGPOS_L,NULL,0x02);
//    usleep(5000); // test
//    uint16_t data_L = 0;
//    can1->JointReadMsg(jointBeingUsed, SCP_MASK,data_L);
//    cout << "data_L: " << data_L << endl;
//    cout.flush();
}

void MainWindow::on_tgPOSPushButton_clicked()
{
//    ui->tgPOSPushButton->setAutoFillBackground(true);
//    if (ui->tgPOSPushButton->palette().button().color() == Qt::red) {
//        QPalette bgpal = ui->tgPOSPushButton->palette();
//        bgpal.setColor (QPalette::Button, Qt::darkRed);
//        ui->tgPOSPushButton->setPalette(bgpal);
//    } else {
//        QPalette bgpal = ui->tgPOSPushButton->palette();
//        bgpal.setColor (QPalette::Button, Qt::red);
//        ui->tgPOSPushButton->setPalette(bgpal);
//    }
    if (paintArea->showItems[2].IsCheck == true) { // showItems[2] is tgPOS
        paintArea->showItems[2].IsCheck = false;
        paintArea->Mask &= ~MASK_TAGPOS;
        ui->tgPOSPushButton->setStyleSheet(tgPOSPushButtonOff);
    } else {
        paintArea->showItems[2].IsCheck = true;
        paintArea->Mask |= MASK_TAGPOS;
        ui->tgPOSPushButton->setStyleSheet(tgPOSPushButtonOn);
        paintArea->showItems[2].sq->FillZero();
    }
    setMask();
}

void MainWindow::on_tgSPDPushButton_clicked()
{
    if (paintArea->showItems[1].IsCheck == true) { // showItems[1] is tgSPD
        paintArea->showItems[1].IsCheck = false;
        paintArea->Mask &= ~MASK_TAGSPD;
        ui->tgSPDPushButton->setStyleSheet(tgSPDPushButtonOff);
    } else {
        paintArea->showItems[1].IsCheck = true;
        paintArea->Mask |= MASK_TAGSPD;
        ui->tgSPDPushButton->setStyleSheet(tgSPDPushButtonOn);
        paintArea->showItems[1].sq->FillZero();
    }
    setMask();
}

void MainWindow::on_tgCURPushButton_clicked()
{
    if (paintArea->showItems[0].IsCheck == true) { // showItems[0] is tgCUR
        paintArea->showItems[0].IsCheck = false;
        paintArea->Mask &= ~MASK_TAGCUR;
        ui->tgCURPushButton->setStyleSheet(tgCURPushButtonOff);
    } else {
        paintArea->showItems[0].IsCheck = true;
        paintArea->Mask |= MASK_TAGCUR;
        ui->tgCURPushButton->setStyleSheet(tgCURPushButtonOn);
        paintArea->showItems[0].sq->FillZero();
    }
    setMask();
}

void MainWindow::on_rlPOSPushButton_clicked()
{
    if (paintArea->showItems[5].IsCheck == true) { // showItems[5] is rlPOS
        paintArea->showItems[5].IsCheck = false;
        paintArea->Mask &= ~MASK_MEAPOS;
        ui->rlPOSPushButton->setStyleSheet(rlPOSPushButtonOff);
    } else {
        paintArea->showItems[5].IsCheck = true;
        paintArea->Mask |= MASK_MEAPOS;
        ui->rlPOSPushButton->setStyleSheet(rlPOSPushButtonOn);
        paintArea->showItems[5].sq->FillZero();
    }
    setMask();
}

void MainWindow::on_rlSPDPushButton_clicked()
{
    if (paintArea->showItems[4].IsCheck == true) { // showItems[4] is rlSPD
        paintArea->showItems[4].IsCheck = false;
        paintArea->Mask &= ~MASK_MEASPD;
        ui->rlSPDPushButton->setStyleSheet(rlSPDPushButtonOff);
    } else {
        paintArea->showItems[4].IsCheck = true;
        paintArea->Mask |= MASK_MEASPD;
        ui->rlSPDPushButton->setStyleSheet(rlSPDPushButtonOn);
        paintArea->showItems[4].sq->FillZero();
    }
    setMask();
}

void MainWindow::on_rlCURPushButton_clicked()
{
    if (paintArea->showItems[3].IsCheck == true) { // showItems[3] is rlCUR
        paintArea->showItems[3].IsCheck = false;
        paintArea->Mask &= ~MASK_MEACUR;
        ui->rlCURPushButton->setStyleSheet(rlCURPushButtonOff);
    } else {
        paintArea->showItems[3].IsCheck = true;
        paintArea->Mask |= MASK_MEACUR;
        ui->rlCURPushButton->setStyleSheet(rlCURPushButtonOn);
        paintArea->showItems[3].sq->FillZero();
    }
    setMask();
}

void MainWindow::on_scopeEnablePushButton_clicked()
{
    if (paintArea->EnableScope) {
        paintArea->EnableScope = false;
        ui->scopeEnablePushButton->setStyleSheet(scopeEnablePushButtonOff);
        ui->scopeEnablePushButton->setText("II");
    } else {
        paintArea->EnableScope = true;
        ui->scopeEnablePushButton->setStyleSheet(scopeEnablePushButtonOn);
        ui->scopeEnablePushButton->setText("▹");
    }
}

void MainWindow::on_offset_POSLineEdit_editingFinished()
{
    paintArea->positionOffset = ui->offset_POSLineEdit->text().toDouble();
}

void MainWindow::on_offset_SPDLineEdit_editingFinished()
{
    paintArea->speedOffset = ui->offset_SPDLineEdit->text().toDouble();
}

void MainWindow::on_offset_CURLineEdit_editingFinished()
{
    paintArea->currentOffset = ui->offset_CURLineEdit->text().toDouble();
}

void MainWindow::on_prComboBox_currentIndexChanged(int index)
{
    switch(index) {
    case 0:
        paintArea->pr = 10;
        break;
    case 1:
        paintArea->pr = 50;
        break;
    case 2:
        paintArea->pr = 100;
        break;
    case 3:
        paintArea->pr = 1000;
        break;
    default:
        paintArea->pr = 10;
        break;
    }
}

void MainWindow::on_srComboBox_currentIndexChanged(int index)
{
    switch(index) {
    case 0:
        paintArea->sr = 1;
        break;
    case 1:
        paintArea->sr = 5;
        break;
    case 2:
        paintArea->sr = 10;
        break;
    default:
        paintArea->sr = 1;
        break;
    }
}

void MainWindow::on_crComboBox_currentIndexChanged(int index)
{
    switch(index) {
    case 0:
        paintArea->cr = 20;
        break;
    case 1:
        paintArea->cr = 50;
        break;
    case 2:
        paintArea->cr = 100;
        break;
    default:
        paintArea->cr = 20;
        break;
    }
}

void MainWindow::on_ScanFrequencyComboBox_currentIndexChanged(int index)
{
    const int MaxFrequency = 10; // kHz
    paintArea->Interval = paintArea->ScanFrequency / MaxFrequency / OSCILLO_SCOPE_INTEVAL; // think about frequency division factor of 10 kHz(ScanFrequency / 10 = Interval * OSCILLO_SCOPE_INTEVAL)
    switch(index) {
    case 0: {// 10(ms) = paintArea->Interval * OSCILLO_SCOPE_INTEVAL
        paintArea->Interval = 10 / OSCILLO_SCOPE_INTEVAL;
        paintArea->ScanFrequency = 10 * MaxFrequency;
        break;
    }
    case 1: // 50ms
        paintArea->Interval = 50 / OSCILLO_SCOPE_INTEVAL;
        paintArea->ScanFrequency = 50 * MaxFrequency;
        break;
    case 2: // 100ms
        paintArea->Interval = 100 / OSCILLO_SCOPE_INTEVAL;
        paintArea->ScanFrequency = 100 * MaxFrequency;
        break;
    }
    uint8_t data[2] = {0,0};
    data[1]= (uint8_t)( (paintArea->ScanFrequency & 0xff00) >> 8 );
    data[0]= (uint8_t)( paintArea->ScanFrequency & 0xff );
    can1->JointSendMsg(jointBeingUsed,CMDTYPE_WR_NR,SCP_REC_TIM,data,0x02);
    usleep(5000);
}
