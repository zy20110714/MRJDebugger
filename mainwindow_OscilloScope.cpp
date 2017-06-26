#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPen>
#include <math.h>
#include <QtGlobal>
#include <QString>
#include <QTime>
#include <vector>
#include "paintArea.h"
#include <qwt_symbol.h>

using std::cout;
using std::endl;
using std::vector;

void MainWindow::OscilloScope()
{
    // initialize Oscilloscope - related content
    tgPOSPushButtonOn = "background-color: rgb(";
    tgPOSPushButtonOn += QString::number(TGPOS_RGB_R) + ',' + QString::number(TGPOS_RGB_G) + ',' + QString::number(TGPOS_RGB_B) + ");";
    tgPOSPushButtonOff = "background-color: rgb(128, 128, 128);";
    tgSPDPushButtonOn = "background-color: rgb(";
    tgSPDPushButtonOn += QString::number(TGSPD_RGB_R) + ',' + QString::number(TGSPD_RGB_G) + ',' + QString::number(TGSPD_RGB_B) + ");";
    tgSPDPushButtonOff = tgPOSPushButtonOff;
    tgCURPushButtonOn = "background-color: rgb(";
    tgCURPushButtonOn += QString::number(TGCUR_RGB_R) + ',' + QString::number(TGCUR_RGB_G) + ',' + QString::number(TGCUR_RGB_B) + ");";
    tgCURPushButtonOff = tgPOSPushButtonOff;
    rlPOSPushButtonOn = "background-color: rgb(";
    rlPOSPushButtonOn += QString::number(RLPOS_RGB_R) + ',' + QString::number(RLPOS_RGB_G) + ',' + QString::number(RLPOS_RGB_B) + ");";
    rlPOSPushButtonOff = tgPOSPushButtonOff;
    rlSPDPushButtonOn = "background-color: rgb(";
    rlSPDPushButtonOn += QString::number(RLSPD_RGB_R) + ',' + QString::number(RLSPD_RGB_G) + ',' + QString::number(RLSPD_RGB_B) + ");";
    rlSPDPushButtonOff = tgPOSPushButtonOff;
    rlCURPushButtonOn = "background-color: rgb(";
    rlCURPushButtonOn += QString::number(RLCUR_RGB_R) + ',' + QString::number(RLCUR_RGB_G) + ',' + QString::number(RLCUR_RGB_B) + ");";
    rlCURPushButtonOff = tgPOSPushButtonOff;
    scopeEnablePushButtonOn = "color: rgb(";
    scopeEnablePushButtonOn += QString::number(SCOPEENABLE_RGB_R) + ',' + QString::number(SCOPEENABLE_RGB_G) + ',' + QString::number(SCOPEENABLE_RGB_B) + ");";
    scopeEnablePushButtonOff = "color: rgb(";
    scopeEnablePushButtonOff += QString::number(SCOPEDISABLE_RGB_R) + ',' + QString::number(SCOPEDISABLE_RGB_G) + ',' + QString::number(SCOPEDISABLE_RGB_B) + ");";

    // 启动刷新控件的定时器
    if (timerOscilloScope == NULL) {
        timerOscilloScope = new QTimer(this);
        connect( timerOscilloScope, SIGNAL(timeout()), this, SLOT(updatePlot()) );
        timerOscilloScope->start(10);
    }

    // 初始化qwt plot
    ui->plot->enableAxis(QwtPlot::xBottom , false);
    ui->plot->setCanvasBackground(Qt::gray);
    ui->plot->setTitle("电流曲线");
    ui->plotSPD->enableAxis(QwtPlot::xBottom , false);
    ui->plotSPD->setCanvasBackground(Qt::gray);
    ui->plotSPD->setTitle("速度曲线");
    ui->plotPOS->enableAxis(QwtPlot::xBottom , false);
    ui->plotPOS->setCanvasBackground(Qt::gray);
    ui->plotPOS->setTitle("位置曲线");

    if (grid == NULL) {
        grid = new QwtPlotGrid ();
        grid->setPen(Qt::black, 0, Qt::DotLine);
        grid->attach(ui->plot);
        grid = new QwtPlotGrid ();
        grid->setPen(Qt::black, 0, Qt::DotLine);
        grid->attach(ui->plotSPD);
        grid = new QwtPlotGrid ();
        grid->setPen(Qt::black, 0, Qt::DotLine);
        grid->attach(ui->plotPOS);
    }
    if (plotMag == NULL) { // 暂时没用
        plotMag = new QwtPlotMagnifier(ui->plot->canvas());
        plotMag->setAxisEnabled(QwtPlot::xBottom , false);
        plotMag->setAxisEnabled(QwtPlot::yLeft , false);
        plotSPDMag = new QwtPlotMagnifier(ui->plotSPD->canvas());
        plotSPDMag->setAxisEnabled(QwtPlot::xBottom , false);
        plotSPDMag->setAxisEnabled(QwtPlot::yLeft , false);
        plotPOSMag = new QwtPlotMagnifier(ui->plotPOS->canvas());
        plotPOSMag->setAxisEnabled(QwtPlot::xBottom , false);
        plotPOSMag->setAxisEnabled(QwtPlot::yLeft , false);
    }

    if (curveRlCUR == NULL) {
        curveRlCUR = new QwtPlotCurve();
        curveRlCUR->setPen(QColor(RLCUR_RGB_R,RLCUR_RGB_G,RLCUR_RGB_B), 3 );
        curveRlCUR->setRenderHint( QwtPlotItem::RenderAntialiased, true );
        curveRlCUR->attach(ui->plot);
    }
    if (curveRlPOS == NULL) {
        curveRlPOS = new QwtPlotCurve();
        curveRlPOS->setPen(QColor(RLPOS_RGB_R,RLPOS_RGB_G,RLPOS_RGB_B), 3 );
        curveRlPOS->setRenderHint( QwtPlotItem::RenderAntialiased, true );
        curveRlPOS->attach(ui->plotPOS);
    }
    if (curveRlSPD == NULL) {
        curveRlSPD = new QwtPlotCurve();
        curveRlSPD->setPen(QColor(RLSPD_RGB_R,RLSPD_RGB_G,RLSPD_RGB_B), 3 );
        curveRlSPD->setRenderHint( QwtPlotItem::RenderAntialiased, true );
        curveRlSPD->attach(ui->plotSPD);
    }
    if (curveTgCUR == NULL) {
        curveTgCUR = new QwtPlotCurve();
        curveTgCUR->setPen(QColor(TGCUR_RGB_R,TGCUR_RGB_G,TGCUR_RGB_B), 3 );
        curveTgCUR->setRenderHint( QwtPlotItem::RenderAntialiased, true );
        curveTgCUR->attach(ui->plot);
    }
    if (curveTgPOS == NULL) {
        curveTgPOS = new QwtPlotCurve();
        curveTgPOS->setPen(QColor(TGPOS_RGB_R,TGPOS_RGB_G,TGPOS_RGB_B), 3 );
        curveTgPOS->setRenderHint( QwtPlotItem::RenderAntialiased, true );
        curveTgPOS->attach(ui->plotPOS);
    }
    if (curveTgSPD == NULL) {
        curveTgSPD = new QwtPlotCurve();
        curveTgSPD->setPen(QColor(TGSPD_RGB_R,TGSPD_RGB_G,TGSPD_RGB_B), 3 );
        curveTgSPD->setRenderHint( QwtPlotItem::RenderAntialiased, true );
        curveTgSPD->attach(ui->plotSPD);
    }

    if (jointBeingUsed == NULL) {
        return;
    }
    // 开启新线程准备显示示波器曲线
    if (osthread == NULL) {
        osthread = new OscilloScopeThread(this, this);
    }

    uint16_t data_L = 0;
    // 记录对象标志MASK的初始化
    can1->controller.SendMsg(jointBeingUsed->ID,CMDTYPE_RD,SCP_MASK,NULL,0x02);
    can1->controller.delayMs(5);
    can1->controller.GetValueInTable(jointBeingUsed->ID, SCP_MASK,data_L);
    osthread->paintArea->Mask = data_L;
    //参数表中的“记录时间间隔（对10kHZ的分频值）”显示到测量条件选项卡中的对应控件里
    can1->controller.SendMsg(jointBeingUsed->ID,CMDTYPE_RD,SCP_REC_TIM,NULL,0x02);
    can1->controller.delayMs(5);
    can1->controller.GetValueInTable(jointBeingUsed->ID, SCP_REC_TIM,data_L);
    osthread->paintArea->ScanFrequency = data_L;
    ui->ScanFrequencyComboBox->setVisible(false); // 扫描频率设置改为不可见
    //同样，用参数表中的“记录时间间隔（对10kHZ的分频值）”设置扫描频率，初始化Interval
    osthread->paintArea->Interval = osthread->paintArea->ScanFrequency / 10 / OSCILLO_SCOPE_INTEVAL; // think about frequency division factor of 10 kHz(ScanFrequency / 10 = Interval * OSCILLO_SCOPE_INTEVAL)
    // EnableScope在paintArea的构造函数初始化了
    // Initializes the color of the buttons
    if ((osthread->paintArea->Mask & MASK_TAGPOS) != 0x00) {
        ui->tgPOSPushButton->setStyleSheet(tgPOSPushButtonOn);
    } else {
        ui->tgPOSPushButton->setStyleSheet(tgPOSPushButtonOff);
    }
    if ((osthread->paintArea->Mask & MASK_TAGSPD) != 0x00) {
        ui->tgSPDPushButton->setStyleSheet(tgSPDPushButtonOn);
    } else {
        ui->tgSPDPushButton->setStyleSheet(tgSPDPushButtonOff);
    }
    if ((osthread->paintArea->Mask & MASK_TAGCUR) != 0x00) {
        ui->tgCURPushButton->setStyleSheet(tgCURPushButtonOn);
    } else {
        ui->tgCURPushButton->setStyleSheet(tgCURPushButtonOff);
    }
    if ((osthread->paintArea->Mask & MASK_MEACUR) != 0x00) {
        ui->rlCURPushButton->setStyleSheet(rlCURPushButtonOn);
    } else {
        ui->rlCURPushButton->setStyleSheet(rlCURPushButtonOff);
    }
    if ((osthread->paintArea->Mask & MASK_MEASPD) != 0x00) {
        ui->rlSPDPushButton->setStyleSheet(rlSPDPushButtonOn);
    } else {
        ui->rlSPDPushButton->setStyleSheet(rlSPDPushButtonOff);
    }
    if ((osthread->paintArea->Mask & MASK_MEAPOS) != 0x00) {
        ui->rlPOSPushButton->setStyleSheet(rlPOSPushButtonOn);
    } else {
        ui->rlPOSPushButton->setStyleSheet(rlPOSPushButtonOff);
    }
    //由记录对象标志初始化要显示的测量曲线
    if (osthread->paintArea->showItems.empty()) {
        cout << "osthread->paintArea->showItems is empty" << endl;
    } else if (osthread->paintArea->showItems.size() == 6){
        if ((osthread->paintArea->Mask & MASK_TAGCUR) != 0x00) {
            osthread->paintArea->showItems[0].IsCheck = true;
        } else {
            osthread->paintArea->showItems[0].IsCheck = false;
        }
        if ((osthread->paintArea->Mask & MASK_TAGPOS) != 0x00) {
            osthread->paintArea->showItems[2].IsCheck = true;
        } else {
            osthread->paintArea->showItems[2].IsCheck = false;
        }
        if ((osthread->paintArea->Mask & MASK_TAGSPD) != 0x00) {
            osthread->paintArea->showItems[1].IsCheck = true;
        } else {
            osthread->paintArea->showItems[1].IsCheck = false;
        }
        if ((osthread->paintArea->Mask & MASK_MEACUR) != 0x00) {
            osthread->paintArea->showItems[3].IsCheck = true;
        } else {
            osthread->paintArea->showItems[3].IsCheck = false;
        }
        if ((osthread->paintArea->Mask & MASK_MEASPD) != 0x00) {
            osthread->paintArea->showItems[4].IsCheck = true;
        } else {
            osthread->paintArea->showItems[4].IsCheck = false;
        }
        if ((osthread->paintArea->Mask & MASK_MEAPOS) != 0x00) {
            osthread->paintArea->showItems[5].IsCheck = true;
        } else {
            osthread->paintArea->showItems[5].IsCheck = false;
        }
    }
    // Initializes the color of the enable button
    if (osthread->paintArea->EnableScope) {
        ui->scopeEnablePushButton->setText("▹");
    } else {
        ui->scopeEnablePushButton->setText("II");
    }
    // Initializes the pr cr sr
    ui->prComboBox->setCurrentIndex(1);
    ui->crComboBox->setCurrentIndex(2);
    ui->srComboBox->setCurrentIndex(2);
}

// 更新示波器显示
void MainWindow::updatePlot() {
    ui->plot->replot();
    ui->plotSPD->replot();
    ui->plotPOS->replot();
}

void MainWindow::on_tgPOSPushButton_clicked()
{
    if (osthread == NULL) {
        return;
    }
    if (osthread->paintArea->showItems[2].IsCheck == true) { // showItems[2] is tgPOS
        ui->tgPOSPushButton->setStyleSheet(tgPOSPushButtonOff);
    } else {
        ui->tgPOSPushButton->setStyleSheet(tgPOSPushButtonOn);
    }
    osthread->changeTgPOS();
}

void MainWindow::on_tgSPDPushButton_clicked()
{
    if (osthread == NULL) {
        return;
    }
    if (osthread->paintArea->showItems[1].IsCheck == true) { // showItems[1] is tgSPD
        ui->tgSPDPushButton->setStyleSheet(tgSPDPushButtonOff);
    } else {
        ui->tgSPDPushButton->setStyleSheet(tgSPDPushButtonOn);
    }
    osthread->changeTgSPD();
}

void MainWindow::on_tgCURPushButton_clicked()
{
    if (osthread == NULL) {
        return;
    }
    if (osthread->paintArea->showItems[0].IsCheck == true) { // showItems[0] is tgCUR
        ui->tgCURPushButton->setStyleSheet(tgCURPushButtonOff);
    } else {
        ui->tgCURPushButton->setStyleSheet(tgCURPushButtonOn);
    }
    osthread->changeTgCUR();
}

void MainWindow::on_rlPOSPushButton_clicked()
{
    if (osthread == NULL) {
        return;
    }
    if (osthread->paintArea->showItems[5].IsCheck == true) { // showItems[5] is rlPOS
        ui->rlPOSPushButton->setStyleSheet(rlPOSPushButtonOff);
    } else {
        ui->rlPOSPushButton->setStyleSheet(rlPOSPushButtonOn);
    }
    osthread->changeRlPOS();
}

void MainWindow::on_rlSPDPushButton_clicked()
{
    if (osthread == NULL) {
        return;
    }
    if (osthread->paintArea->showItems[4].IsCheck == true) { // showItems[4] is rlSPD
        ui->rlSPDPushButton->setStyleSheet(rlSPDPushButtonOff);
    } else {
        ui->rlSPDPushButton->setStyleSheet(rlSPDPushButtonOn);
    }
    osthread->changeRlSPD();
}

void MainWindow::on_rlCURPushButton_clicked()
{
    if (osthread == NULL) {
        return;
    }
    if (osthread->paintArea->showItems[3].IsCheck == true) { // showItems[3] is rlCUR
        ui->rlCURPushButton->setStyleSheet(rlCURPushButtonOff);
    } else {
        ui->rlCURPushButton->setStyleSheet(rlCURPushButtonOn);
    }
    osthread->changeRlCUR();
}

void MainWindow::on_scopeEnablePushButton_clicked()
{
    if (osthread == NULL) {
        return;
    }
    if (osthread->paintArea->EnableScope) {
        osthread->paintArea->EnableScope = false;
        ui->scopeEnablePushButton->setText("II");
    } else {
        osthread->paintArea->EnableScope = true;
        ui->scopeEnablePushButton->setText("▹");
    }
}

void MainWindow::on_offset_POSLineEdit_editingFinished()
{
    if (osthread == NULL) {
        return;
    }
    osthread->paintArea->positionOffset = ui->offset_POSLineEdit->text().toDouble();
}

void MainWindow::on_offset_SPDLineEdit_editingFinished()
{
    if (osthread == NULL) {
        return;
    }
    osthread->paintArea->speedOffset = ui->offset_SPDLineEdit->text().toDouble();
}

void MainWindow::on_offset_CURLineEdit_editingFinished()
{
    if (osthread == NULL) {
        return;
    }
    osthread->paintArea->currentOffset = ui->offset_CURLineEdit->text().toDouble();
}

void MainWindow::on_prComboBox_currentIndexChanged(int index)
{
    if (osthread == NULL) {
        return;
    }
    switch(index) {
    case 0:
        osthread->paintArea->pr = 10;
        break;
    case 1:
        osthread->paintArea->pr = 50;
        break;
    case 2:
        osthread->paintArea->pr = 100;
        break;
    case 3:
        osthread->paintArea->pr = 1000;
        break;
    default:
        osthread->paintArea->pr = 10;
        break;
    }
}

void MainWindow::on_srComboBox_currentIndexChanged(int index)
{
    if (osthread == NULL) {
        return;
    }
    switch(index) {
    case 0:
        osthread->paintArea->sr = 0.5;
        break;
    case 1:
        osthread->paintArea->sr = 1.0;
        break;
    case 2:
        osthread->paintArea->sr = 2.0;
        break;
    default:
        osthread->paintArea->sr = 2.0;
        break;
    }
}

void MainWindow::on_crComboBox_currentIndexChanged(int index)
{
    if (osthread == NULL) {
        return;
    }
    switch(index) {
    case 0:
        osthread->paintArea->cr = 20;
        break;
    case 1:
        osthread->paintArea->cr = 60;
        break;
    case 2:
        osthread->paintArea->cr = 100;
        break;
    case 3:
        osthread->paintArea->cr = 200;
        break;
    default:
        osthread->paintArea->cr = 100;
        break;
    }
}

void MainWindow::on_ScanFrequencyComboBox_currentIndexChanged(int index)
{
    if (osthread == NULL) {
        return;
    }
    const int MaxFrequency = 10; // kHz
    osthread->paintArea->Interval = osthread->paintArea->ScanFrequency / MaxFrequency / OSCILLO_SCOPE_INTEVAL; // think about frequency division factor of 10 kHz(ScanFrequency / 10 = Interval * OSCILLO_SCOPE_INTEVAL)
    switch(index) {
    case 0: {// 10(ms) = osthread->paintArea->Interval * OSCILLO_SCOPE_INTEVAL
        osthread->paintArea->Interval = 10 / OSCILLO_SCOPE_INTEVAL;
        osthread->paintArea->ScanFrequency = 10 * MaxFrequency;
        break;
    }
    case 1: // 50ms
        osthread->paintArea->Interval = 50 / OSCILLO_SCOPE_INTEVAL;
        osthread->paintArea->ScanFrequency = 50 * MaxFrequency;
        break;
    case 2: // 100ms
        osthread->paintArea->Interval = 100 / OSCILLO_SCOPE_INTEVAL;
        osthread->paintArea->ScanFrequency = 100 * MaxFrequency;
        break;
    }
    uint8_t data[2] = {0,0};
    data[1]= (uint8_t)( (osthread->paintArea->ScanFrequency & 0xff00) >> 8 );
    data[0]= (uint8_t)( osthread->paintArea->ScanFrequency & 0xff );
    can1->controller.SendMsg(jointBeingUsed->ID,CMDTYPE_WR_NR,SCP_REC_TIM,data,0x02);
    can1->controller.delayMs(5);
}
