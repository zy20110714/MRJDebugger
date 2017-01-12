#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <vector>
#include <math.h>
#include <QDateTime>
#include <QDebug>

#define PI 3.1415926

using std::vector;
using std::cout;
using std::endl;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    // use shellscript to initialize the CAN path
    FILE *fp = popen("./../findPCAN.sh", "r"); // ./ represents the relative path to the build folder
    if(fp == NULL)
    {
       qDebug("The shellscript could not be found to initialize the CAN path.\n");
       exit(0);
    }
    // Save the path to a variable
    char* canPath = new char[1024];
    fgets(canPath, 1024, fp);
    // Delete the '\n' at the end of the variable
    int n = strlen(canPath);
    canPath[n-1]='\0';
    pclose(fp);

    if (strcmp(canPath, "The pcan-usb is not connected.") == 0) { // When the shellscript does not find the pcan, it prints this string
        qDebug("The pcan-usb is not connected.\n");
        exit(0);
    }

    can1 = new JointThread();
    qDebug("%s\n",canPath);
    can1->JointCanInit(canPath);
    can1->JointThreadStart(3000);

    jointBeingUsed = 0;
    EnableRun = false;
    bias = 0;
    paintArea = NULL;

    ui->setupUi(this);
    // Fill the items of combo box
    vector<int> allID = can1->getJointallID();
    for (vector<int>::iterator iter = allID.begin(); iter != allID.end(); ++iter) {
        ui->cmbID->addItem(QString::number(*iter, 10));
    }
    if (allID.size() == 0) {
        qDebug("The joint is not connected.\n");
        exit(0);
    }
    ui->cmbID->setCurrentIndex(allID.size() - 1); // 获取末尾ID，方便调试，index从0开始，size从1开始

    ui->manualMin->setText("-30");
    ui->manualMax->setText("30");
    txtBiasChangemanualSlider();

    time_clock = new QTimer(this);
    connect(time_clock,SIGNAL(timeout()),this,SLOT(slotTimeDone()));
    time_clock->start(MOTION_CONTROL_INTEVAL);

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
    scopeEnablePushButtonOn = "background-color: rgb(";
    scopeEnablePushButtonOn += QString::number(SCOPEENABLE_RGB_R) + ',' + QString::number(SCOPEENABLE_RGB_G) + ',' + QString::number(SCOPEENABLE_RGB_B) + ");";
    scopeEnablePushButtonOff = "background-color: rgb(";
    scopeEnablePushButtonOff += QString::number(SCOPEDISABLE_RGB_R) + ',' + QString::number(SCOPEDISABLE_RGB_G) + ',' + QString::number(SCOPEDISABLE_RGB_B) + ");";

    OscilloScope();
    // initialize PID - related content
    ENonPPushButtonOn = "background-color: rgb(";
    ENonPPushButtonOn += QString::number(ENonP_RGB_R) + ',' + QString::number(ENonP_RGB_G) + ',' + QString::number(ENonP_RGB_B) + ");";
    ENonPPushButtonOff = tgPOSPushButtonOff;
    PID();

    Set();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_cmbID_currentIndexChanged(const QString &arg1)
{
    jointBeingUsed = arg1.toInt(); // jointBeingUsed = ui->cmbID->currentText().toInt(); // 修改当前控制的模块ID
    int workMode = can1->getJointWorkMode(jointBeingUsed);
    ui->cmbWorkMode->setCurrentIndex(workMode); // 由当前工作模式更新控制板块中的ComboBox
    workModeUpdatetxtBias();
    txtBiasChangemanualSlider();

    // 4条状态的显示
    can1->JointSendMsg(jointBeingUsed,CMDTYPE_RD,SYS_MODEL_TYPE,NULL,0x02);
    usleep(1000);
    can1->JointSendMsg(jointBeingUsed,CMDTYPE_RD,SYS_FW_VERSION,NULL,0x02);
    usleep(1000);
    can1->JointSendMsg(jointBeingUsed,CMDTYPE_RD,SYS_REDU_RATIO,NULL,0x02);
//    usleep(1000);
//    can1->JointSendMsg(jointBeingUsed,CMDTYPE_RD,SYS_MODEL_TYPE,NULL,0x02);
    usleep(5000);
    uint16_t data_L = 0;
    can1->JointReadMsg(jointBeingUsed, SYS_MODEL_TYPE,data_L);
    modelTypeBeingUsed = data_L;
    QString tmp;
    switch (modelTypeBeingUsed)
    {
        case 16: tmp = "M14"; break;
        case 17: tmp = "M14E"; break;
        case 2:
        case 32: tmp = "M17"; break;
        case 33: tmp = "M17E"; break;
        case 48: tmp = "M20"; break;
        case 64: tmp = "LIFT"; break;
    }
    ui->typeLabel->setText("TYPE: " + tmp);
    can1->JointReadMsg(jointBeingUsed, SYS_FW_VERSION,data_L);
    ui->firmLabel->setText("FIRM: " + QString::number(data_L,10));
    can1->JointReadMsg(jointBeingUsed, SYS_REDU_RATIO,data_L);
    ui->ratioLabel->setText("RATIO: " + QString::number(data_L,10));

    if (paintArea == NULL) {
        return;
    }
    // 记录对象标志MASK的初始化
    can1->JointSendMsg(jointBeingUsed,CMDTYPE_RD,SCP_MASK,NULL,0x02);
    usleep(5000);
//    uint16_t data_L = 0;
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

    //由记录对象标志初始化要显示的测量曲线
    paintArea->showItems.clear();
    QPen pen;
//    pen.color().getRgb(r,g,b);
    pen.setBrush(QColor(TGCUR_RGB_R,TGCUR_RGB_G,TGCUR_RGB_B));
    paintArea->showItems.insert(0,*(new ShowItem((paintArea->Mask & MASK_TAGCUR) != 0x00,
                                               TAG_CURRENT_L,//SCP_TAGCUR_L,
                                               pen,
                                               paintArea->width())));
    // pen.setBrush(Qt::green);
    pen.setBrush(QColor(TGSPD_RGB_R,TGSPD_RGB_G,TGSPD_RGB_B));
    paintArea->showItems.insert(1,*(new ShowItem((paintArea->Mask & MASK_TAGSPD) != 0x00,
                                               TAG_SPEED_L,//SCP_TAGSPD_L,
                                               pen,
                                               paintArea->width())));
    // pen.setBrush(Qt::red);
    pen.setBrush(QColor(TGPOS_RGB_R,TGPOS_RGB_G,TGPOS_RGB_B));
    paintArea->showItems.insert(2,*(new ShowItem((paintArea->Mask & MASK_TAGPOS) != 0x00,
                                               TAG_POSITION_L,//SCP_TAGPOS_L,
                                               pen,
                                               paintArea->width())));
    // pen.setBrush(Qt::white);
    pen.setBrush(QColor(RLCUR_RGB_R,RLCUR_RGB_G,RLCUR_RGB_B));
    paintArea->showItems.insert(3,*(new ShowItem((paintArea->Mask & MASK_MEACUR) != 0x00,
                                               SYS_CURRENT_L,//SCP_MEACUR_L,
                                               pen,
                                               paintArea->width())));
    // pen.setBrush(Qt::yellow);
    pen.setBrush(QColor(RLSPD_RGB_R,RLSPD_RGB_G,RLSPD_RGB_B));
    paintArea->showItems.insert(4,*(new ShowItem((paintArea->Mask & MASK_MEASPD) != 0x00,
                                               SYS_SPEED_L,//SCP_MEASPD_L,
                                               pen,
                                               paintArea->width())));
    // pen.setBrush(Qt::magenta);
    pen.setBrush(QColor(RLPOS_RGB_R,RLPOS_RGB_G,RLPOS_RGB_B));
    paintArea->showItems.insert(5,*(new ShowItem((paintArea->Mask & MASK_MEAPOS) != 0x00,
                                               SYS_POSITION_L,//SCP_MEAPOS_L,
                                               pen,
                                               paintArea->width())));
    for (int i = 0; i < paintArea->showItems.count(); i++) {
        paintArea->showItems[i].sq->FillZero();
    }

    PID();
    Set();
}

void MainWindow::on_txtBias_editingFinished()
{
    bias = ui->txtBias->text().toDouble();
    txtBiasChangemanualSlider();
}

void MainWindow::on_cmbWorkMode_currentIndexChanged(int index)
{
    int workMode = index;

    can1->setJointWorkMode(jointBeingUsed,workMode);
    usleep(1000);
    can1->readJointWorkMode(jointBeingUsed);
    usleep(5000);

    workModeUpdatetxtBias();
    txtBiasChangemanualSlider();
}

void MainWindow::workModeUpdatetxtBias()
{
    int workMode = ui->cmbWorkMode->currentIndex();

    switch(workMode) // 工作模式修改后，修改txtBias
    {
        case MODE_OPEN:
            can1->readTagPWM(jointBeingUsed);
            usleep(5000);
            ui->txtBias->setValue(can1->getTagPWM(jointBeingUsed)); // 由当前目标PWM更新手动控制中的偏移量
            break;
        case MODE_CURRENT:
            can1->readCurI(jointBeingUsed);
            usleep(5000);
            ui->txtBias->setValue(can1->getCurI(jointBeingUsed)); // 由当前实际电流更新手动控制中的偏移量
            break;
        case MODE_SPEED:
            can1->readCurSpeed(jointBeingUsed);
            usleep(5000);
            ui->txtBias->setValue(can1->getCurSpeed(jointBeingUsed, MODEL_TYPE_M14)); // 由当前实际电流更新手动控制中的偏移量
            break;
        case MODE_POSITION:
            can1->readCurPos(jointBeingUsed);
            usleep(5000);
            ui->txtBias->setValue(can1->getCurPos(jointBeingUsed, JOINT_ANGLE, MODEL_TYPE_M14)); // 由当前实际位置更新手动控制中的偏移量
            break;
        default: break;
    }
    on_txtBias_editingFinished();
}

void MainWindow::on_manualMax_editingFinished()
{
    float max = ui->manualMax->text().toFloat();
    QString s = QString("%1").arg(max * -1);
    ui->manualMin->setText(s);
    txtBiasChangemanualSlider();
}

void MainWindow::on_manualMin_editingFinished()
{
    txtBiasChangemanualSlider();
}

void MainWindow::txtBiasChangemanualSlider()
{
    float bias = ui->txtBias->text().toFloat();
    float min = ui->manualMin->text().toFloat();
    float max = ui->manualMax->text().toFloat();
    float f = 100 * (bias - min) / (max - min);
    int v = (int)f;
    ui->manualSlider->setValue(v);
}

void MainWindow::on_manualSlider_valueChanged(int value)
{
    float min = ui->manualMin->text().toFloat();
    float max = ui->manualMax->text().toFloat();
    if (ui->manualSlider->hasFocus()) {
        float bias = min + (max - min) * value / 100;
        ui->txtBias->setValue(bias);
        on_txtBias_editingFinished();
    }
}

void MainWindow::on_confirmButton_clicked()
{
    EnableRun = true;
}

void MainWindow::on_stopButton_clicked()
{
    EnableRun = false;
    int workMode = ui->cmbWorkMode->currentIndex();
    switch(workMode) // Different WorkMode Different Stop way
    {
    case MODE_OPEN: {
        int pwm = 0;
        can1->setTagPWM(jointBeingUsed, pwm);
        usleep(5000);
        break;
    }
    case MODE_CURRENT: {
        int curI = 0;
        can1->setTagI(jointBeingUsed, curI);
        usleep(5000);
        break;
    }
    case MODE_SPEED: {
        float speed = 0;
        can1->setTagSpeed(jointBeingUsed, speed, MODEL_TYPE_M20, CMDTYPE_WR_NR);
        usleep(5000);
        break;
    }
    case MODE_POSITION: {
        // Can not be directly set to 0.
        // If read the current position first, then set the target position to the current position, that may be too slow.
        // So use another method: switch the WorkMode to speed then set the speed to 0, then switch back. The hardware could ensure to update the target location.
        workMode = MODE_SPEED;
        can1->setJointWorkMode(jointBeingUsed,workMode);
        usleep(5000);
        float speed = 0;
        can1->setTagSpeed(jointBeingUsed, speed, MODEL_TYPE_M20, CMDTYPE_WR_NR);
        usleep(5000);
        workMode = MODE_POSITION;
        can1->setJointWorkMode(jointBeingUsed,workMode);
        usleep(5000);
        break;
    }
    default: break;
    }
    // In fact, Stop means setting the Bias, and the text of Bias could be updated by simulate WorkMode change
    workModeUpdatetxtBias();
    txtBiasChangemanualSlider();
}

void MainWindow::slotTimeDone()
{
    static unsigned int s_iCount = 0;
    static unsigned int s_iCountforwave = 0;
    if (EnableRun) {
        s_iCount++;
        //根据所选波形进入相应控制步骤
        switch (ui->waveModeCombo->currentIndex())
        {
        case MODE_MANUAL: {
            s_iCount = 0;
            SetValue(bias);
            break;
        }
        case MODE_SQUARE: { // 方波时根据选定频率发送，经过半个周期变换一次方向，所以是乘500
            static bool s_bHigh = false;
            double time = s_iCount * MOTION_CONTROL_INTEVAL;
            if (time >= 500.0 / frequency) {
                s_iCount = 0;
                s_bHigh = !s_bHigh;
                SetValue((int)(amplitude * (s_bHigh ? 1 : -1) + bias));
            }
            break;
        }
        case MODE_TRIANGLE: {//三角波时 TriangleInterval 个 Interval 发送一次
            const unsigned int TriangleInterval = 1;
            if (s_iCount >= TriangleInterval) {
                s_iCount = 0;
                //得到三角波的周期（s）
                double T = 1.0 / frequency;
                //校准指令发送的间隔时间
                s_iCountforwave++;
                //获得发送指令时真正的时间（ms）
                double time = s_iCountforwave * TriangleInterval * MOTION_CONTROL_INTEVAL;
                //若当前时间超过一个周期，校准时间使得时间回到周期开始
                if (time / 1000 >= T) {
                    s_iCountforwave = 0;
                }
                //由当前时间得到当前控制值
                double tempf = time / 1000 * amplitude / T;
                //发送控制值
                SetValue(tempf + bias);
            }
            break;
        }
        case MODE_SINE: { // 正弦波时SineInterval 个Interval 发送一次
            const unsigned int SineInterval = 1;
            if (s_iCount >= SineInterval) {
                s_iCount = 0;
                //得到正弦波的周期（s）
                double T = 1.0 / frequency;
                //校准指令发送的间隔时间
                s_iCountforwave++;
                //获得发送指令时真正的时间（ms）
                double time = s_iCountforwave * SineInterval * MOTION_CONTROL_INTEVAL;
                //若当前时间超过一个周期，校准时间使得时间回到周期开始
                if (time / 1000 >= T) {
                    s_iCountforwave = 0;
                }
                //由当前时间得到当前控制值
                double tempf = sin(time / 1000 * frequency * 2 * PI) * amplitude;
                //发送控制值
                SetValue((short)(tempf + bias));
            }
            break;
        }
        }
    }
}

void MainWindow::SetValue(double value)
{
    int workMode = ui->cmbWorkMode->currentIndex();
    switch(workMode) // 不同控制模式，控制指令不同
    {
    case MODE_OPEN: {
        int pwm = (int)value;
        can1->setTagPWM(jointBeingUsed, pwm);
        break;
    }
    case MODE_CURRENT: {
        int curI = (int)value;
        can1->setTagI(jointBeingUsed, curI);
        break;
    }
    case MODE_SPEED: {
        float speed = (float)value;
        can1->setTagSpeed(jointBeingUsed, speed, MODEL_TYPE_M14, CMDTYPE_WR_NR);
        break;
    }
    case MODE_POSITION: {
        float angle = (float)value;
        can1->setTagPos(jointBeingUsed, angle, JOINT_ANGLE, MODEL_TYPE_M14, CMDTYPE_WR_NR);
        break;
    }
    default: break;
    }
}


void MainWindow::on_waveModeCombo_currentIndexChanged(int index)
{
    switch (index)
    {
    case MODE_MANUAL: {
        ui->frequencyLineEdit->setEnabled(false);
        ui->frequencyLineEdit->setText("");
        ui->AmplitudeLineEdit->setEnabled(false);
        ui->AmplitudeLineEdit->setText("");
    }
    case MODE_SQUARE: {
        ui->frequencyLineEdit->setEnabled(true);
        ui->frequencyLineEdit->setText("0.5");
        on_frequencyLineEdit_editingFinished();
        ui->AmplitudeLineEdit->setEnabled(true);
        ui->AmplitudeLineEdit->setText("20");
        on_AmplitudeLineEdit_editingFinished();
    }
    case MODE_TRIANGLE: {
        ui->frequencyLineEdit->setEnabled(true);
        ui->frequencyLineEdit->setText("0.5");
        on_frequencyLineEdit_editingFinished();
        ui->AmplitudeLineEdit->setEnabled(true);
        ui->AmplitudeLineEdit->setText("20");
        on_AmplitudeLineEdit_editingFinished();
    }
    case MODE_SINE: {
        ui->frequencyLineEdit->setEnabled(true);
        ui->frequencyLineEdit->setText("0.5");
        on_frequencyLineEdit_editingFinished();
        ui->AmplitudeLineEdit->setEnabled(true);
        ui->AmplitudeLineEdit->setText("20");
        on_AmplitudeLineEdit_editingFinished();
    }
    }
}

void MainWindow::on_frequencyLineEdit_editingFinished()
{
    frequency = ui->frequencyLineEdit->text().toDouble();
}

void MainWindow::on_AmplitudeLineEdit_editingFinished()
{
    amplitude = ui->AmplitudeLineEdit->text().toDouble();
}
