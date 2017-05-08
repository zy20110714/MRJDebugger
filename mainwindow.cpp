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
    can1(new UserControlOnCan),     // 调用API开启CAN线上的线程
    ui(new Ui::MainWindow),
    jointBeingUsed(NULL),
    timerMove(NULL),
    paintArea(NULL),
    timerOscilloScope(NULL),        // 并没有使用
    curveTgPOS(NULL),
    curveTgSPD(NULL),
    curveTgCUR(NULL),
    curveRlPOS(NULL),
    curveRlSPD(NULL),
    curveRlCUR(NULL),
    grid(NULL),
    curveGrid(NULL),
    timerMonitor(NULL),             // 监视器用的定时器
    timerBottom(NULL)
{
    if( !can1->Init("pcan0") ) {
        // 不能捕获CAN API输出的信息
        QMessageBox::warning(this,"警告","CAN初始化失败！");
        // 临时的处理方法，直接退出程序
        exit(0);
    }

    // 启动ui界面
    ui->setupUi(this);
    // 开启新线程准备显示示波器曲线
    OscilloScopeThread osthread(this);
    osthread.start();
    // 从JointThread读已有的ID，添加到ui的cmbID控件上
    updatecmbID();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mouseMoveEvent(QMouseEvent *e)
{
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

    // Bar中的控件
    QPoint Bar = ui->Bar->pos();
    QRect btnFlash = QRect(ui->btnFlash->pos() + centralWidget + Bar, ui->btnFlash->size());
    QRect cmbID = QRect(ui->cmbID->pos() + centralWidget + Bar, ui->cmbID->size());
    QRect firmLabel = QRect(ui->firmLabel->pos() + centralWidget + Bar, ui->firmLabel->size());
    QRect helpLabel = QRect(ui->helpLabel->pos() + centralWidget + Bar, ui->helpLabel->size());
    QRect ratioLabel = QRect(ui->ratioLabel->pos() + centralWidget + Bar, ui->ratioLabel->size());
    QRect typeLabel = QRect(ui->typeLabel->pos() + centralWidget + Bar, ui->typeLabel->size());

    // 在helpTextEdit中显示提示内容
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
    // PID中的控件 // 由于离开控件时取消显示的问题，暂不显示信息
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
    // 按住ctrl、shift、alt和F7，则进入命令发送高级模式
    if (e->modifiers() == (Qt::ShiftModifier | Qt::ControlModifier | Qt::AltModifier) && e->key() == Qt::Key_F7) {
            advControlForm = new AdvancedControl(this, can1);
            advControlForm->show();
    }
}
