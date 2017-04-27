#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>

#define PI 3.1415926

using std::string;

void MainWindow::moveInitialize()
{
    // 设置启动、停止按钮的样式表，background-color背景色
    stopButtonOn = "color: rgb(";
    stopButtonOn += QString::number(TGPOS_RGB_R) + ','
            + QString::number(TGPOS_RGB_G) + ','
            + QString::number(TGPOS_RGB_B) + ");";
    stopButtonOff = "color: rgb(128, 128, 128);";
    confirmButtonOn = "color: rgb(";
    confirmButtonOn += QString::number(TGSPD_RGB_R) + ','
            + QString::number(TGSPD_RGB_G) + ','
            + QString::number(TGSPD_RGB_B) + ");";
    confirmButtonOff = stopButtonOff;
    ui->stopButton->setStyleSheet(stopButtonOn);
    ui->confirmButton->setStyleSheet(confirmButtonOff);
    // 定义运动控制为关
    EnableRun = false;
    ui->confirmButton->setText("Click to run"); // 此外还有一处设置了setText
    // 首次启动则定义运动控制定时器
    if (timerMove != NULL) {
        ;
    } else {
        timerMove = new QTimer(this);
        connect(timerMove,SIGNAL(timeout()),this,SLOT(slotTimeMoveDone()));
        timerMove->start(MOTION_CONTROL_INTEVAL);
    }
    if (jointBeingUsed != NULL) {
        // 由当前工作模式更新控制板块中的ComboBox
        int workMode = jointBeingUsed->getWorkMode();
        ui->cmbWorkMode->setCurrentIndex(workMode); // 会触发，也可能不会触发on_cmbWorkMode_currentIndexChanged()
        // 防止没有调用on_cmbWorkMode_currentIndexChanged()，强制运行下列2个函数
        // 工作模式更新bias
        workModeUpdatetxtBias();
        // bias更新滑块
        txtBiasChangemanualSlider();
    }
}

void MainWindow::on_txtBias_editingFinished()
{
    int workMode = ui->cmbWorkMode->currentIndex();
    if (MODE_POSITION != workMode) { // 若是位置控制，则不限制bias
        float max = ui->manualMax->text().toDouble();
        float min = ui->manualMin->text().toDouble();
        if (ui->txtBias->text().toDouble() > max) {
            ui->txtBias->setValue(max);
        } else if (ui->txtBias->text().toDouble() < min) {
            ui->txtBias->setValue(min);
        }
    }
    bias = ui->txtBias->text().toDouble();
    txtBiasChangemanualSlider();
}

void MainWindow::on_cmbWorkMode_currentIndexChanged(int index)
{
    qDebug() << "in on_cmbWorkMode_currentIndexChanged";
    int workMode = index;

    jointBeingUsed->setWorkMode(workMode);
    can1->controller.delayMs(1);
    jointBeingUsed->updateWorkMode();
    can1->controller.delayMs(5);

    // 工作模式更新bias
    workModeUpdatetxtBias();
    // bias更新滑块
    txtBiasChangemanualSlider();
}

void MainWindow::workModeUpdatetxtBias()
{
    int workMode = ui->cmbWorkMode->currentIndex();

    switch(workMode) // 工作模式修改后，修改txtBias
    {
        case MODE_OPEN:
            jointBeingUsed->updateTagPWM();
            can1->controller.delayMs(5);
            ui->txtBias->setValue(jointBeingUsed->getTagPWM()); // 由当前目标PWM更新手动控制中的偏移量
            ui->manualMax->setText("30");
            ui->manualMin->setText("-30");
            break;
        case MODE_CURRENT:
            jointBeingUsed->updateCurI();
            can1->controller.delayMs(5);
//            ui->txtBias->setValue(can1->getCurI(jointBeingUsed)); // 由当前实际电流更新手动控制中的偏移量，实际中处于0位的机械臂2关节突然掉下
            ui->txtBias->setValue(0); // 和上面注释的那句效果一样，不如就改成0
            ui->manualMax->setText("500");
            ui->manualMin->setText("-500");
            break;
        case MODE_SPEED:
            jointBeingUsed->updateCurSpeed();
            can1->controller.delayMs(5);
            ui->txtBias->setValue(jointBeingUsed->getCurSpeed()); // 由当前实际电流更新手动控制中的偏移量
            ui->manualMax->setText("20");
            ui->manualMin->setText("-20");
            break;
        case MODE_POSITION: {
//            qDebug() << "ID: " << jointBeingUsed->ID;
            jointBeingUsed->updateCurPos();
            can1->controller.delayMs(5);
            float tempf = jointBeingUsed->getCurPos(JOINT_ANGLE);
//            qDebug() << tempf;
            double tempd = (double)tempf;
//            qDebug() << tempd;
//            string s;
//            can1->controller.GetErrorText(s);
//            qDebug() << s.c_str();
            ui->txtBias->setValue(tempd); // 由当前实际位置更新手动控制中的偏移量
            ui->manualMax->setText("90");
            ui->manualMin->setText("-90");
            break;
        }
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
    int workMode = ui->cmbWorkMode->currentIndex();
    float bias = ui->txtBias->text().toFloat();
    float min = ui->manualMin->text().toFloat();
    float max = ui->manualMax->text().toFloat();
    if (MODE_POSITION == workMode) { // 如果是位置控制模式，滑块极值相应改变
        if (ui->manualSlider->hasFocus() || ui->manualMax->hasFocus() || ui->manualMin->hasFocus()) {
            ;
        } else {
            max = bias + 90;
            min = bias - 90;
            ui->manualMax->setText(QString::number(max, 'f' , 2));
            ui->manualMin->setText(QString::number(min, 'f' , 2));
        }
    }
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
    ui->confirmButton->setText("Running");
    ui->confirmButton->setStyleSheet(confirmButtonOn);
    ui->stopButton->setStyleSheet(stopButtonOff);
}

void MainWindow::on_stopButton_clicked()
{
    EnableRun = false;
    ui->stopButton->setStyleSheet(stopButtonOn);
    ui->confirmButton->setText("Click to run");
    ui->confirmButton->setStyleSheet(confirmButtonOff);
    int workMode = ui->cmbWorkMode->currentIndex();
    switch(workMode) // Different WorkMode Different Stop way
    {
    case MODE_OPEN: {
        int pwm = 0;
        jointBeingUsed->setTagPWM(pwm);
        can1->controller.delayMs(5);
        break;
    }
    case MODE_CURRENT: {
        int curI = 0;
        jointBeingUsed->setTagI(curI);
        can1->controller.delayMs(5);
        break;
    }
    case MODE_SPEED: {
        float speed = 0;
        jointBeingUsed->setTagSpeed(speed);
        can1->controller.delayMs(5);
        break;
    }
    case MODE_POSITION: {
        // Can not be directly set to 0.
        // If read the current position first, then set the target position to the current position, that may be too slow.
        // So use another method: switch the WorkMode to speed then set the speed to 0, then switch back. The hardware could ensure to update the target location.
        // 但实际使用时，各关节都处于0位，2关节和4关节受到重力影响，这样的停止方式会发生运动，而且不停地按stop会持续性地运动

        jointBeingUsed->updateCurPos();
        can1->controller.delayMs(5);
        float currentPos = jointBeingUsed->getCurPos(JOINT_ANGLE);
        jointBeingUsed->setTagPos(currentPos, JOINT_ANGLE);
        break;
    }
    default: break;
    }
    // In fact, Stop means setting the Bias, and the text of Bias could be updated by simulate WorkMode change
    workModeUpdatetxtBias();
    // 实际使用中，在速度模式下，Stop后不会马上变成0，导致显示的速度不为0，而实际已经停止
    ui->txtBias->setValue(0.0f); // 补充修改Bias的值为0
    txtBiasChangemanualSlider();
}

/*
 * 运动控制的定时器
 */
void MainWindow::slotTimeMoveDone()
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
        jointBeingUsed->setTagPWM(pwm);
        break;
    }
    case MODE_CURRENT: {
        int curI = (int)value;
        jointBeingUsed->setTagI(curI);
        break;
    }
    case MODE_SPEED: {
        float speed = (float)value;
        jointBeingUsed->setTagSpeed(speed);
        break;
    }
    case MODE_POSITION: {
        float angle = (float)value;
        jointBeingUsed->setTagPos(angle, JOINT_ANGLE);
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
