#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPen>
#include <math.h>
#include <QtGlobal>
#include <QString>
#include <QTime>
#include <vector>
#include "paintarea.h"
#include <qwt_symbol.h>

using std::cout;
using std::endl;
using std::vector;

unsigned __stdcall MainWindow::oscilloScopeThreadStaticEntryPoint(void* arg)
{
    if (arg == NULL) {
        return 1;
    }
    MainWindow * JT = (MainWindow*)arg;
//    //自定义高精度定时器，CPU占用51~70%左右
    while(true) { // < 执行的次数
        JT->slotTimeOscilloScopeDone();
//        GetLocalTime( &sys );
//        cout << sys.wHour << ":"
//        << sys.wMinute << ":"
//        << sys.wSecond << ":"
//        << sys.wMilliseconds << endl;
        JT->can1->controller.delayMs(10);
    } // CPU占用43%左右
    return 0;
}


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

    if (paintArea != NULL) {
//        delete paintArea;
//        paintArea = NULL;
    } else {
        paintArea = new PaintArea(this);
//        ui->gridLayout_OS->addWidget(paintArea);
//        cout << "addWidget(paintArea) done" << endl;
    }

    // 初始化qwt plot
    // 启动刷新控件的定时器
    if (timerOscilloScope == NULL) {
        timerOscilloScope = new QTimer(this);
        connect( timerOscilloScope, SIGNAL(timeout()), this, SLOT(updatePlot()) );
        timerOscilloScope->start(10);
    }

//    ui->plot->enableAxis(QwtPlot::yLeft, false);
    ui->plot->enableAxis(QwtPlot::xBottom , false);
    ui->plot->setCanvasBackground(Qt::gray);
    ui->plot->setTitle("电流曲线");
//    ui->plot->setAxisScale( QwtPlot::yLeft, -(ui->plot->height() / 2), ui->plot->height() / 2 );
    ui->plotSPD->enableAxis(QwtPlot::xBottom , false);
    ui->plotSPD->setCanvasBackground(Qt::gray);
    ui->plotSPD->setTitle("速度曲线");
    ui->plotPOS->enableAxis(QwtPlot::xBottom , false);
    ui->plotPOS->setCanvasBackground(Qt::gray);
    ui->plotPOS->setTitle("位置曲线");
//    ui->plot->setAxisAutoScale(QwtPlot::xBottom, false);
//    ui->plotSPD->setAxisAutoScale(QwtPlot::xBottom, false);
//    ui->plotPOS->setAxisAutoScale(QwtPlot::xBottom, false);


    for (vector<ShowItem>::iterator iItem = paintArea->showItems.begin(); iItem != paintArea->showItems.end(); ++iItem) {
//        iItem.base()->sq.MaxLength = ui->plot->width();
        iItem.base()->sq.MaxLength = 700; // grid一格100有7格
    }
    for (unsigned int i = 0; i < paintArea->showItems.size(); i++) {
        paintArea->showItems[i].sq.FillZero();
    }
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

//    //使用鼠标左键平移
//    (void) new QwtPlotPanner( ui->plot->canvas() );

    if (curveGrid == NULL) {
//        QPolygonF points;
//        // 画虚纵线
//        // 认为采样频率不变，都为100Hz，因为要间隔1s，故乘100
//        for (int i = 0; 100 * i < ui->plot->height() / 2; ++i) {
//            curveGrid = new QwtPlotCurve();
//            curveGrid->setPen(Qt::black, 0, Qt::DotLine );
//            curveGrid->setRenderHint( QwtPlotItem::RenderAntialiased, true );
//            points.clear();
//            points << QPointF(ui->plot->width() / 2 + 100 * i, - ui->plot->height()/2)
//                   << QPointF(ui->plot->width() / 2 + 100 * i, ui->plot->height()/2);
//            curveGrid->setSamples( points );
//            curveGrid->attach(ui->plot);
//            curveGrid->attach(ui->plotSPD);


//            curveGrid = new QwtPlotCurve();
//            curveGrid->setPen(Qt::black, 0, Qt::DotLine );
//            curveGrid->setRenderHint( QwtPlotItem::RenderAntialiased, true );
//            points.clear();
//            points << QPointF(ui->plot->width() / 2 - 100 * i, - ui->plot->height()/2)
//                   << QPointF(ui->plot->width() / 2 - 100 * i, ui->plot->height()/2);
//            curveGrid->setSamples( points );
//            curveGrid->attach(ui->plot);
//        }
    }

    if (curveRlCUR == NULL) {
        curveRlCUR = new QwtPlotCurve();
        curveRlCUR->setPen(QColor(RLCUR_RGB_R,RLCUR_RGB_G,RLCUR_RGB_B), 3 );
        curveRlCUR->setRenderHint( QwtPlotItem::RenderAntialiased, true );
//        QwtSymbol *symbol = new QwtSymbol( QwtSymbol::Ellipse,
//            QBrush( Qt::yellow ), QPen( Qt::red, 2 ), QSize( 1, 1) );//设置样本点的颜色、大小
//        curveRlCUR->setSymbol( symbol );//添加样本点形状
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
    uint16_t data_L = 0;
    // 记录对象标志MASK的初始化
    can1->controller.SendMsg(jointBeingUsed->ID,CMDTYPE_RD,SCP_MASK,NULL,0x02);
    can1->controller.delayMs(5);
    can1->controller.GetValueInTable(jointBeingUsed->ID, SCP_MASK,data_L);
    paintArea->Mask = data_L;
    //参数表中的“记录时间间隔（对10kHZ的分频值）”显示到测量条件选项卡中的对应控件里
    can1->controller.SendMsg(jointBeingUsed->ID,CMDTYPE_RD,SCP_REC_TIM,NULL,0x02);
    can1->controller.delayMs(5);
    can1->controller.GetValueInTable(jointBeingUsed->ID, SCP_REC_TIM,data_L);
    paintArea->ScanFrequency = data_L;
//    ui->ScanFrequencyComboBox->setCurrentIndex();
    ui->ScanFrequencyComboBox->setVisible(false); // 扫描频率设置改为不可见
    //同样，用参数表中的“记录时间间隔（对10kHZ的分频值）”设置扫描频率，初始化Interval
    paintArea->Interval = paintArea->ScanFrequency / 10 / OSCILLO_SCOPE_INTEVAL; // think about frequency division factor of 10 kHz(ScanFrequency / 10 = Interval * OSCILLO_SCOPE_INTEVAL)
//    cout << "paintArea->Interval: " << paintArea->Interval << endl;
    // EnableScope在paintArea的构造函数初始化了
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
//        ui->scopeEnablePushButton->setStyleSheet(scopeEnablePushButtonOn);
        ui->scopeEnablePushButton->setText("▹");
    } else {
//        ui->scopeEnablePushButton->setStyleSheet(scopeEnablePushButtonOff);
        ui->scopeEnablePushButton->setText("II");
    }
    // Initializes the pr cr sr
//    on_prComboBox_currentIndexChanged(ui->prComboBox->currentIndex());
    ui->prComboBox->setCurrentIndex(1);
//    on_crComboBox_currentIndexChanged(ui->crComboBox->currentIndex());
    ui->crComboBox->setCurrentIndex(2);
//    on_srComboBox_currentIndexChanged(ui->srComboBox->currentIndex());
    ui->srComboBox->setCurrentIndex(2);

//    timeOscilloScope = new QTimer(this);
//    connect(timeOscilloScope,SIGNAL(timeout()),this,SLOT(slotTimeOscilloScopeDone()));
//    timeOscilloScope->start(OSCILLO_SCOPE_INTEVAL);

//    cout << "prepare to deal paintArea->showItems" << endl;
    //由记录对象标志初始化要显示的测量曲线
    if (paintArea->showItems.empty()) {
        cout << "paintArea->showItems is empty" << endl;

    } else if (paintArea->showItems.size() == 6){
//        cout << "the size of paintArea->showItems is 6" << endl;
        if ((paintArea->Mask & MASK_TAGCUR) != 0x00) {
            paintArea->showItems[0].IsCheck = true;
        } else {
            paintArea->showItems[0].IsCheck = false;
        }
        if ((paintArea->Mask & MASK_TAGPOS) != 0x00) {
            paintArea->showItems[2].IsCheck = true;
        } else {
            paintArea->showItems[2].IsCheck = false;
        }
        if ((paintArea->Mask & MASK_TAGSPD) != 0x00) {
            paintArea->showItems[1].IsCheck = true;
        } else {
            paintArea->showItems[1].IsCheck = false;
        }
        if ((paintArea->Mask & MASK_MEACUR) != 0x00) {
            paintArea->showItems[3].IsCheck = true;
        } else {
            paintArea->showItems[3].IsCheck = false;
        }
        if ((paintArea->Mask & MASK_MEASPD) != 0x00) {
            paintArea->showItems[4].IsCheck = true;
        } else {
            paintArea->showItems[4].IsCheck = false;
        }
        if ((paintArea->Mask & MASK_MEAPOS) != 0x00) {
            paintArea->showItems[5].IsCheck = true;
        } else {
            paintArea->showItems[5].IsCheck = false;
        }
    }
    //由记录对象标志初始化要显示的测量曲线
//    cout << "paintArea->showItems IsCheck done" << endl;
    for (vector<ShowItem>::iterator i = paintArea->showItems.begin(); i < paintArea->showItems.end(); ++i) {
//        cout << "prepare paintArea->showItems sq FillZero()" << endl;
        i.base()->sq.FillZero();
    }
//    cout << "paintArea->showItems sq FillZero() done" << endl;
}

// 更新示波器显示
void MainWindow::updatePlot() {
    //分别处理每条要测量的曲线
    for ( unsigned int i = 0; i < paintArea->showItems.size(); i++ ) {
        QPolygonF points;
        //若队列非空并且勾选显示
        if (!(paintArea->showItems[i].sq.IsEmpty()) && paintArea->showItems[i].IsCheck) {
            for (int j = 0; j < paintArea->showItems[i].sq.Count(); j++) {//分别处理每一坐标对
                double verticalValue1 = paintArea->showItems[i].sq.GetValue(j);
                points.append(QPointF(j, verticalValue1));
            }
        }
        switch (i) {
            case 0:curveTgCUR->setSamples( points );break;
            case 1:curveTgSPD->setSamples( points );break;
            case 2:curveTgPOS->setSamples( points );break;
            case 3:curveRlCUR->setSamples( points );break;
            case 4:curveRlSPD->setSamples( points );break;
            case 5:curveRlPOS->setSamples( points );break;
        }
    }
    ui->plot->replot();
    ui->plotSPD->replot();
    ui->plotPOS->replot();
}

void MainWindow::slotTimeOscilloScopeDone()
{
    if (paintArea == NULL) {
        return;
    }
    if (paintArea->showItems.size() != 6) {
//        cout << "slotTimeOscilloScopeDone() - paintArea->showItems.count(): " << paintArea->showItems.count() << endl;
        return;
    }
    if (jointBeingUsed == NULL) {
        return;
    }
//    cout << QDateTime::currentDateTime().toString("hh:mm:ss:zzz").toStdString() << endl;

    //示波器显示曲线用的计数量
    static int gatherCount = 0;

    //示波器绘制曲线使能开启
//    if (false) {
    if (paintArea->EnableScope) {
        //间隔计数会从1开始判断
        ++gatherCount;
        //按照Interval限定的间隔去执行
//        if (true) {
        if (gatherCount >= paintArea->Interval) {
            // 间隔计数清零
            gatherCount = 0;
            // 分别处理每个显示项
            // 显示项的显示队列不为0才向队列追加一个新值，该32位有符号值由2个16位有符号数组合而成
            for (unsigned int i = 0; i < paintArea->showItems.size(); i++) {
                if (paintArea->showItems[i].sq.MaxLength != 0 && paintArea->showItems[i].IsCheck) {
                    uint16_t data_L = 0;
                    uint16_t data_H = 0;
                    can1->controller.GetValueInTable(jointBeingUsed->ID, paintArea->showItems[i].Item, data_L);
                    can1->controller.GetValueInTable(jointBeingUsed->ID, paintArea->showItems[i].Item + 1, data_H);
                    double temp = data_L + (data_H * 65536);

                    switch (paintArea->showItems[i].Item) {
                    case SCP_TAGCUR_L://(showItems[0].Item)://TAG_CURRENT_L:
                    case SCP_MEACUR_L://showItems[3].Item://SYS_CURRENT_L://若曲线是电流
                        break;
                    case SCP_TAGSPD_L://showItems[1].Item://TAG_SPEED_L:
                    case SCP_MEASPD_L://showItems[4].Item://SYS_SPEED_L://若曲线是速度
//                        cout << temp << endl;
                        temp *= 60; // rpm
                        temp /= 65536; // rpm
                        if (modelTypeBeingUsed == MODEL_TYPE_M14) { temp /= GEAR_RATIO_M14; }
                        else if (modelTypeBeingUsed == MODEL_TYPE_M14E) { temp /= GEAR_RATIO_M14E; }
                        else if (modelTypeBeingUsed == MODEL_TYPE_M17) { temp /= GEAR_RATIO_M17; }
                        else if (modelTypeBeingUsed == MODEL_TYPE_M17E) { temp /= GEAR_RATIO_M17E; }
                        else if (modelTypeBeingUsed == MODEL_TYPE_M20) { temp /= GEAR_RATIO_M20; }
                        else if (modelTypeBeingUsed == MODEL_TYPE_LIFT) { temp /= GEAR_RATIO_LIFT; }

//                        temp /= modelTypeBeingUsed;
                        break;
                    case SCP_TAGPOS_L://showItems[2].Item://TAG_POSITION_L:
                    case SCP_MEAPOS_L://showItems[5].Item://SYS_POSITION_L://若曲线是位置
                        temp = temp * 360 / 65536; // degree（会在paintArea里进行转换显示）
//                        temp = data_L; // 位置要去掉圈数，只显示绝对编码器的数值 2017-3-28
                        break;
                    }

                    //向队尾追加值
                    paintArea->showItems[i].sq.Append(temp);
//                    cout << "paintArea->showItems[" << i << "].sq: " << paintArea->showItems[i].sq.GetValue(paintArea->showItems[i].sq.Count() - 1) << endl;
//                    paintArea->update(); // 开启后出现问题

                }
            }
//            updatePlot(); // QCoreApplication::sendPostedEvents: Cannot send posted events for objects in another thread
        }
    }
}

void MainWindow::setMask()
{
    // 向下位机请求数据
    uint8_t data[2] = {0,0};
    data[1] = (uint8_t)( (paintArea->Mask & 0xff00) >> 8 );
    data[0] = (uint8_t)( paintArea->Mask & 0xff );
    can1->controller.SendMsg(jointBeingUsed->ID, CMDTYPE_WR_NR, SCP_MASK, data, 2);
    can1->controller.SendMsg(jointBeingUsed->ID,CMDTYPE_RD,SCP_MASK,NULL,0x02);
    can1->controller.delayMs(5);
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
        paintArea->showItems[2].sq.FillZero();
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
        paintArea->showItems[1].sq.FillZero();
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
        paintArea->showItems[0].sq.FillZero();
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
        paintArea->showItems[5].sq.FillZero();
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
        paintArea->showItems[4].sq.FillZero();
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
        paintArea->showItems[3].sq.FillZero();
    }
    setMask();
}

void MainWindow::on_scopeEnablePushButton_clicked()
{
    if (paintArea->EnableScope) {
//        plotMag->setAxisEnabled(QwtPlot::yLeft, true);
//        plotSPDMag->setAxisEnabled(QwtPlot::yLeft, true);
//        plotPOSMag->setAxisEnabled(QwtPlot::yLeft, true);
        paintArea->EnableScope = false;
//        ui->scopeEnablePushButton->setStyleSheet(scopeEnablePushButtonOff);
        ui->scopeEnablePushButton->setText("II");
    } else {
        paintArea->EnableScope = true;
//        ui->scopeEnablePushButton->setStyleSheet(scopeEnablePushButtonOn);
        ui->scopeEnablePushButton->setText("▹");
//        plotMag->setAxisEnabled(QwtPlot::yLeft, false);
//        plotSPDMag->setAxisEnabled(QwtPlot::yLeft, false);
//        plotPOSMag->setAxisEnabled(QwtPlot::yLeft, false);
//        ui->plot->setAxisAutoScale(QwtPlot::yLeft, true);
//        ui->plotSPD->setAxisAutoScale(QwtPlot::yLeft, true);
//        ui->plotPOS->setAxisAutoScale(QwtPlot::yLeft, true);
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
        paintArea->sr = 0.5;
        break;
    case 1:
        paintArea->sr = 1.0;
        break;
    case 2:
        paintArea->sr = 2.0;
        break;
    default:
        paintArea->sr = 2.0;
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
        paintArea->cr = 60;
        break;
    case 2:
        paintArea->cr = 100;
        break;
    case 3:
        paintArea->cr = 200;
        break;
    default:
        paintArea->cr = 100;
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
    can1->controller.SendMsg(jointBeingUsed->ID,CMDTYPE_WR_NR,SCP_REC_TIM,data,0x02);
    can1->controller.delayMs(5);
}
