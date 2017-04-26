#include <vector>
#include <math.h>
#include <QDateTime>
#include <QDebug>
#include <QPicture>
#include <QPixmap>
#include <QPainter>
#include <QMouseEvent>
#include <QMessageBox>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "connectform.h"
#include "ui_connectform.h"
#include <QThread>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // 调用API开启CAN线上的线程
    can1 = new UserControlOnCan();
    if(!can1->Init("pcan0")) { // 不能捕获CAN API输出的信息
        QMessageBox::warning(this,"警告","CAN初始化失败！");
        // 临时的处理方法，直接退出程序
        exit(0);
    }

    jointBeingUsed = 0;
    timerMove = NULL;
    timerMonitor = NULL; // 监视器用的定时器
    timerBottom = NULL;
    timerOscilloScope = NULL; // 并没有使用
    paintArea = NULL;
    curveTgPOS = NULL;
    curveTgSPD = NULL;
    curveTgCUR = NULL;
    curveRlPOS = NULL;
    curveRlSPD = NULL;
    curveRlCUR = NULL;
    curveGrid = NULL;
    grid = NULL;


    // 启动ui界面
    ui->setupUi(this);
    // 测试代码
//    ui->labelShowEnablePic->setPixmap(QPixmap("1.jpg"));
    ui->labelShowEnablePic->hide();


    // 开启新线程准备显示示波器曲线
//    QThread t(this);
    OscilloScopeThread osthread(this);
    osthread.start();
//    osthread.moveToThread(&t);
//    t.start();
//    osthread.selfRun();

//    try {
//        HANDLE handle = (HANDLE)_beginthreadex(NULL, 0, oscilloScopeThreadStaticEntryPoint, this, 0, NULL);
//        WaitForSingleObject(handle, 5);
//        qDebug() << "oscilloScopeThread init success !" << endl;
//    } catch (std::exception) {
//        QMessageBox::warning(this,"警告","线程初始化失败！");
//        exit(EXIT_FAILURE);
//    }
    // 从JointThread读已有的ID，添加到ui的cmbID控件上
    updatecmbID();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mouseMoveEvent(QMouseEvent *e) {
    QPoint centralWidget = ui->centralWidget->pos(); // (0,0)

    // TestRun中的控件
    QPoint TestRun = ui->TestRun->pos();
    QRect AMPLabel = QRect(ui->AMPLabel->pos() + centralWidget + TestRun, ui->AMPLabel->size());
    QRect BIASLabel = QRect(ui->BIASLabel->pos() + centralWidget + TestRun, ui->BIASLabel->size());
    QRect FREQLabel = QRect(ui->FREQLabel->pos() + centralWidget + TestRun, ui->FREQLabel->size());
    QRect MODELabel = QRect(ui->MODELabel->pos() + centralWidget + TestRun, ui->MODELabel->size());
    QRect TARGETLabel = QRect(ui->TARGETLabel->pos() + centralWidget + TestRun, ui->TARGETLabel->size());
    QRect cmbWorkMode = QRect(ui->cmbWorkMode->pos() + centralWidget + TestRun, ui->cmbWorkMode->size());
    QRect confirmButton = QRect(ui->confirmButton->pos() + centralWidget + TestRun, ui->confirmButton->size());
    QRect manualSlider = QRect(ui->manualSlider->pos() + centralWidget + TestRun, ui->manualSlider->size());
    QRect stopButton = QRect(ui->stopButton->pos() + centralWidget + TestRun, ui->stopButton->size());
    QRect waveModeCombo = QRect(ui->waveModeCombo->pos() + centralWidget + TestRun, ui->waveModeCombo->size());
    // QRect txtBias = QRect(ui->txtBias->pos() + centralWidget + TestRun, ui->txtBias->size());
    // QRect AmplitudeLineEdit = QRect(ui->AmplitudeLineEdit->pos() + centralWidget + TestRun, ui->AmplitudeLineEdit->size());
    // QRect frequencyLineEdit = QRect(ui->frequencyLineEdit->pos() + centralWidget + TestRun, ui->frequencyLineEdit->size());
    // QRect manualMax = QRect(ui->manualMax->pos() + centralWidget + TestRun, ui->manualMax->size());
    // QRect manualMin = QRect(ui->manualMin->pos() + centralWidget + TestRun, ui->manualMin->size());

    // 示波器中的控件
    QRect ScanFrequencyComboBox = QRect(ui->ScanFrequencyComboBox->pos() + centralWidget, ui->ScanFrequencyComboBox->size());
    QRect crComboBox = QRect(ui->crComboBox->pos() + centralWidget, ui->crComboBox->size());
    QRect prComboBox = QRect(ui->prComboBox->pos() + centralWidget, ui->prComboBox->size());
    QRect rlCURPushButton = QRect(ui->rlCURPushButton->pos() + centralWidget, ui->rlCURPushButton->size());
    QRect rlPOSPushButton = QRect(ui->rlPOSPushButton->pos() + centralWidget, ui->rlPOSPushButton->size());
    QRect rlSPDPushButton = QRect(ui->rlSPDPushButton->pos() + centralWidget, ui->rlSPDPushButton->size());
    QRect tgCURPushButton = QRect(ui->tgCURPushButton->pos() + centralWidget, ui->tgCURPushButton->size());
    QRect tgPOSPushButton = QRect(ui->tgPOSPushButton->pos() + centralWidget, ui->tgPOSPushButton->size());
    QRect tgSPDPushButton = QRect(ui->tgSPDPushButton->pos() + centralWidget, ui->tgSPDPushButton->size());
    QRect scopeEnablePushButton = QRect(ui->scopeEnablePushButton->pos() + centralWidget, ui->scopeEnablePushButton->size());
    QRect srComboBox = QRect(ui->srComboBox->pos() + centralWidget, ui->srComboBox->size());

//    // PID中的控件
//    QRect adjustGroupComboBox = QRect(ui->adjustGroupComboBox->pos() + centralWidget, ui->adjustGroupComboBox->size());
//    QRect limitAccLabel = QRect(ui->limitAccLabel->pos() + centralWidget, ui->limitAccLabel->size());
//    QRect limitCurLabel = QRect(ui->limitCurLabel->pos() + centralWidget, ui->limitCurLabel->size());
//    QRect limitSpdLabel = QRect(ui->limitSpdLabel->pos() + centralWidget, ui->limitSpdLabel->size());

    // Bar中的控件
    QPoint Bar = ui->Bar->pos();
    QRect btnFlash = QRect(ui->btnFlash->pos() + centralWidget + Bar, ui->btnFlash->size());
    QRect cmbID = QRect(ui->cmbID->pos() + centralWidget + Bar, ui->cmbID->size());
    QRect firmLabel = QRect(ui->firmLabel->pos() + centralWidget + Bar, ui->firmLabel->size());
    QRect helpLabel = QRect(ui->helpLabel->pos() + centralWidget + Bar, ui->helpLabel->size());
    QRect ratioLabel = QRect(ui->ratioLabel->pos() + centralWidget + Bar, ui->ratioLabel->size());
    QRect typeLabel = QRect(ui->typeLabel->pos() + centralWidget + Bar, ui->typeLabel->size());

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


    // 测试信息输出
//     cout << "e->pos(): (" << e->pos().x() << "," << e->pos().y() << ")" << endl;
//     cout << "TestRun->pos(): (" << TestRun.x() << "," << TestRun.y() << ")" << endl;
//     cout << "AmplitudeLineEdit: (" << AmplitudeLineEdit.x() << "," << AmplitudeLineEdit.y() << ")" << endl;
//     cout << "ENonPPushButton: (" << ENonPPushButton.x() << "," << ENonPPushButton.y() << ")" << endl;
//     cout << "ScanFrequencyCombBox: (" << ScanFrequencyComboBox.x() << "," << ScanFrequencyComboBox.y() << ")" << endl;
//     cout.flush();

    ui->helpTextEdit->setText("");
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
    if(waveModeCombo.contains(e->pos())) {
        ui->helpTextEdit->setText(ui->waveModeCombo->toolTip());
        return;
    }
    // 示波器中的控件
    if(ScanFrequencyComboBox.contains(e->pos())) {
        ui->helpTextEdit->setText(ui->ScanFrequencyComboBox->toolTip());
        return;
    }
    if(crComboBox.contains(e->pos())) {
        ui->helpTextEdit->setText(ui->crComboBox->toolTip());
        return;
    }
    if(prComboBox.contains(e->pos())) {
        ui->helpTextEdit->setText(ui->prComboBox->toolTip());
        return;
    }
    if(rlCURPushButton.contains(e->pos())) {
        ui->helpTextEdit->setText(ui->rlCURPushButton->toolTip());
        return;
    }
    if(rlPOSPushButton.contains(e->pos())) {
        ui->helpTextEdit->setText(ui->rlPOSPushButton->toolTip());
        return;
    }
    if(rlSPDPushButton.contains(e->pos())) {
        ui->helpTextEdit->setText(ui->rlSPDPushButton->toolTip());
        return;
    }
    if(tgCURPushButton.contains(e->pos())) {
        ui->helpTextEdit->setText(ui->tgCURPushButton->toolTip());
        return;
    }
    if(tgPOSPushButton.contains(e->pos())) {
        ui->helpTextEdit->setText(ui->tgPOSPushButton->toolTip());
        return;
    }
    if(tgSPDPushButton.contains(e->pos())) {
        ui->helpTextEdit->setText(ui->tgSPDPushButton->toolTip());
        return;
    }
    if(scopeEnablePushButton.contains(e->pos())) {
        ui->helpTextEdit->setText(ui->scopeEnablePushButton->toolTip());
        return;
    }
    if(srComboBox.contains(e->pos())) {
        ui->helpTextEdit->setText(ui->srComboBox->toolTip());
        return;
    }
//    // PID中的控件 // 由于离开控件时取消显示的问题，暂不显示信息
//    if(adjustGroupComboBox.contains(e->pos())) {
//        ui->helpTextEdit->setText(ui->adjustGroupComboBox->toolTip());
//        return;
//    }
//    if(limitAccLabel.contains(e->pos())) {
//        ui->helpTextEdit->setText(ui->limitAccLabel->toolTip());
//        return;
//    }
//    if(limitCurLabel.contains(e->pos())) {
//        ui->helpTextEdit->setText(ui->limitCurLabel->toolTip());
//        return;
//    }
//    if(limitSpdLabel.contains(e->pos())) {
//        ui->helpTextEdit->setText(ui->limitSpdLabel->toolTip());
//        return;
//    }
    // Bar中的控件
    if(btnFlash.contains(e->pos())) {
        ui->helpTextEdit->setText(ui->btnFlash->toolTip());
        return;
    }
    if(cmbID.contains(e->pos())) {
        ui->helpTextEdit->setText(ui->cmbID->toolTip());
        return;
    }
    if(firmLabel.contains(e->pos())) {
        ui->helpTextEdit->setText(ui->firmLabel->toolTip());
        return;
    }
    if(helpLabel.contains(e->pos())) {
        ui->helpTextEdit->setText(ui->helpLabel->toolTip());
        return;
    }
    if(ratioLabel.contains(e->pos())) {
        ui->helpTextEdit->setText(ui->ratioLabel->toolTip());
        return;
    }
    if(typeLabel.contains(e->pos())) {
        ui->helpTextEdit->setText(ui->typeLabel->toolTip());
        return;
    }
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    // 按住ctrl、shift、alt和F7，再单击左键，则进入命令发送高级模式
    if (e->modifiers() == (Qt::ShiftModifier | Qt::ControlModifier | Qt::AltModifier) && e->key() == Qt::Key_F7) {
//        if (QApplication::mouseButtons() == Qt::LeftButton) {
            advControlForm = new AdvancedControl(this, can1);
            advControlForm->show();
//            QMessageBox::information(this,"提示","命令发送高级模式！");
//        }
    }
}

