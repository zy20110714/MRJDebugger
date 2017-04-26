#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSpinBox>

using std::cout;
using std::endl;

void MainWindow::PID()
{
    if (jointBeingUsed == NULL) {
        return;
    }
    // 初始化API中的MCT里的PID
    for (int i = SEV_CURRENT_P; i <= SEV_POSITION_DS; i++) {
        can1->controller.SendMsg(jointBeingUsed->ID,CMDTYPE_RD,i,NULL,0x02);
        can1->controller.delayMs(1);
    }
    for (int i = M_CURRENT_P; i <= M_POSITION_DS; i++) {
        can1->controller.SendMsg(jointBeingUsed->ID,CMDTYPE_RD,i,NULL,0x02);
        can1->controller.delayMs(1);
    }
    for (int i = L_CURRENT_P; i <= L_POSITION_DS; i++) {
        can1->controller.SendMsg(jointBeingUsed->ID,CMDTYPE_RD,i,NULL,0x02);
        can1->controller.delayMs(1);
    }
    can1->controller.delayMs(5);
    on_adjustGroupComboBox_currentIndexChanged(ui->adjustGroupComboBox->currentIndex());

    // 初始化API中的MCT里的限制值
    can1->controller.SendMsg(jointBeingUsed->ID,CMDTYPE_RD,LIT_MAX_CURRENT,NULL,0x02);
    can1->controller.delayMs(1);
    can1->controller.SendMsg(jointBeingUsed->ID,CMDTYPE_RD,LIT_MAX_SPEED,NULL,0x02);
    can1->controller.delayMs(1);
    can1->controller.SendMsg(jointBeingUsed->ID,CMDTYPE_RD,LIT_MAX_ACC,NULL,0x02);
    can1->controller.delayMs(1);
    can1->controller.SendMsg(jointBeingUsed->ID,CMDTYPE_RD,LIT_MIN_POSITION_L,NULL,0x08);
    can1->controller.delayMs(5);

    uint16_t data_L = 0;

    can1->controller.GetValueInTable(jointBeingUsed->ID, LIT_MAX_CURRENT,data_L);
    ui->maxCurLineEdit->setText(QString::number(data_L, 10));
    can1->controller.GetValueInTable(jointBeingUsed->ID, LIT_MAX_SPEED,data_L);
    ui->maxSpdLineEdit->setText(QString::number(data_L, 10));
    can1->controller.GetValueInTable(jointBeingUsed->ID, LIT_MAX_ACC,data_L);
    ui->maxAccLineEdit->setText(QString::number(data_L, 10));
    can1->controller.GetValueInTable(jointBeingUsed->ID, LIT_MIN_POSITION_L,data_L);
    ui->minPosLLineEdit->setText(QString::number(data_L, 10));
    can1->controller.GetValueInTable(jointBeingUsed->ID, LIT_MAX_POSITION_L,data_L);
    ui->maxPosLLineEdit->setText(QString::number(data_L, 10));
    can1->controller.GetValueInTable(jointBeingUsed->ID, LIT_MIN_POSITION_H,data_L);
    ui->minPosHLineEdit->setText(QString::number(data_L, 10));
    can1->controller.GetValueInTable(jointBeingUsed->ID, LIT_MAX_POSITION_H,data_L);
    ui->maxPosHLineEdit->setText(QString::number(data_L, 10));

}

void MainWindow::showSEVPID()
{
    uint16_t data_L = 0;

    can1->controller.GetValueInTable(jointBeingUsed->ID, SEV_POSITION_P,data_L);
    ui->POS_PSpinBox->setValue(data_L);
    can1->controller.GetValueInTable(jointBeingUsed->ID, SEV_POSITION_I,data_L);
    ui->POS_ISpinBox->setValue(data_L);
    can1->controller.GetValueInTable(jointBeingUsed->ID, SEV_POSITION_D,data_L);
    ui->POS_DSpinBox->setValue(data_L);
    can1->controller.GetValueInTable(jointBeingUsed->ID, SEV_POSITION_DS,data_L);
    ui->POS_DSSpinBox->setValue(data_L);

    can1->controller.GetValueInTable(jointBeingUsed->ID, SEV_SPEED_P,data_L);
    ui->SPD_PSpinBox->setValue(data_L);
    can1->controller.GetValueInTable(jointBeingUsed->ID, SEV_SPEED_I,data_L);
    ui->SPD_ISpinBox->setValue(data_L);
    can1->controller.GetValueInTable(jointBeingUsed->ID, SEV_SPEED_D,data_L);
    ui->SPD_DSpinBox->setValue(data_L);
    can1->controller.GetValueInTable(jointBeingUsed->ID, SEV_SPEED_DS,data_L);
    ui->SPD_DSSpinBox->setValue(data_L);

    can1->controller.GetValueInTable(jointBeingUsed->ID, SEV_CURRENT_P,data_L);
    ui->CUR_PSpinBox->setValue(data_L);
    can1->controller.GetValueInTable(jointBeingUsed->ID, SEV_CURRENT_I,data_L);
    ui->CUR_ISpinBox->setValue(data_L);
    can1->controller.GetValueInTable(jointBeingUsed->ID, SEV_CURRENT_D,data_L);
    ui->CUR_DSpinBox->setValue(data_L);
}

void MainWindow::showMPID()
{
    uint16_t data_L = 0;

    can1->controller.GetValueInTable(jointBeingUsed->ID, M_POSITION_P,data_L);
    ui->POS_PSpinBox->setValue(data_L);
    can1->controller.GetValueInTable(jointBeingUsed->ID, M_POSITION_I,data_L);
    ui->POS_ISpinBox->setValue(data_L);
    can1->controller.GetValueInTable(jointBeingUsed->ID, M_POSITION_D,data_L);
    ui->POS_DSpinBox->setValue(data_L);
    can1->controller.GetValueInTable(jointBeingUsed->ID, M_POSITION_DS,data_L);
    ui->POS_DSSpinBox->setValue(data_L);

    can1->controller.GetValueInTable(jointBeingUsed->ID, M_SPEED_P,data_L);
    ui->SPD_PSpinBox->setValue(data_L);
    can1->controller.GetValueInTable(jointBeingUsed->ID, M_SPEED_I,data_L);
    ui->SPD_ISpinBox->setValue(data_L);
    can1->controller.GetValueInTable(jointBeingUsed->ID, M_SPEED_D,data_L);
    ui->SPD_DSpinBox->setValue(data_L);
    can1->controller.GetValueInTable(jointBeingUsed->ID, M_SPEED_DS,data_L);
    ui->SPD_DSSpinBox->setValue(data_L);

    can1->controller.GetValueInTable(jointBeingUsed->ID, M_CURRENT_P,data_L);
    ui->CUR_PSpinBox->setValue(data_L);
    can1->controller.GetValueInTable(jointBeingUsed->ID, M_CURRENT_I,data_L);
    ui->CUR_ISpinBox->setValue(data_L);
    can1->controller.GetValueInTable(jointBeingUsed->ID, M_CURRENT_D,data_L);
    ui->CUR_DSpinBox->setValue(data_L);
}

void MainWindow::showLPID()
{
    uint16_t data_L = 0;

    can1->controller.GetValueInTable(jointBeingUsed->ID, L_POSITION_P,data_L);
    ui->POS_PSpinBox->setValue(data_L);
    can1->controller.GetValueInTable(jointBeingUsed->ID, L_POSITION_I,data_L);
    ui->POS_ISpinBox->setValue(data_L);
    can1->controller.GetValueInTable(jointBeingUsed->ID, L_POSITION_D,data_L);
    ui->POS_DSpinBox->setValue(data_L);
    can1->controller.GetValueInTable(jointBeingUsed->ID, L_POSITION_DS,data_L);
    ui->POS_DSSpinBox->setValue(data_L);

    can1->controller.GetValueInTable(jointBeingUsed->ID, L_SPEED_P,data_L);
    ui->SPD_PSpinBox->setValue(data_L);
    can1->controller.GetValueInTable(jointBeingUsed->ID, L_SPEED_I,data_L);
    ui->SPD_ISpinBox->setValue(data_L);
    can1->controller.GetValueInTable(jointBeingUsed->ID, L_SPEED_D,data_L);
    ui->SPD_DSpinBox->setValue(data_L);
    can1->controller.GetValueInTable(jointBeingUsed->ID, L_SPEED_DS,data_L);
    ui->SPD_DSSpinBox->setValue(data_L);

    can1->controller.GetValueInTable(jointBeingUsed->ID, L_CURRENT_P,data_L);
    ui->CUR_PSpinBox->setValue(data_L);
    can1->controller.GetValueInTable(jointBeingUsed->ID, L_CURRENT_I,data_L);
    ui->CUR_ISpinBox->setValue(data_L);
    can1->controller.GetValueInTable(jointBeingUsed->ID, L_CURRENT_D,data_L);
    ui->CUR_DSpinBox->setValue(data_L);
}

void MainWindow::on_adjustGroupComboBox_currentIndexChanged(int index)
{
    switch(index) {
    case 0:
        showSEVPID();
        break;
    case 1:
        showMPID();
        break;
    case 2:
        showLPID();
        break;
    }
}

void MainWindow::setNewPID(int value,int index)
{
    // 向下位机请求数据
    uint8_t data[2] = {0,0};
    data[1] = (uint8_t)( (value & 0xff00) >> 8 );
    data[0] = (uint8_t)( value & 0xff );
    can1->controller.SendMsg(jointBeingUsed->ID, CMDTYPE_WR_NR, index, data, 2);
    can1->controller.delayMs(1);
    cout << "MainWindow::setNewPID(): value = " << value << endl;
    can1->controller.SendMsg(jointBeingUsed->ID,CMDTYPE_RD,index,NULL,0x02);
    can1->controller.delayMs(5);
}

void MainWindow::on_POS_PSpinBox_editingFinished()
{
    int index = 0;

    switch(ui->adjustGroupComboBox->currentIndex()) {
    case 0:
        index = SEV_POSITION_P;
        break;
    case 1:
        index = M_POSITION_P;
        break;
    case 2:
        index = L_POSITION_P;
        break;
    }
    setNewPID(ui->POS_PSpinBox->value(),index);

    uint16_t data_L = 0;

    can1->controller.GetValueInTable(jointBeingUsed->ID, index,data_L);
    cout << data_L << endl;
    ui->POS_PSpinBox->setValue(data_L);
    cout << ui->POS_PSpinBox->value() << endl;

}

void MainWindow::on_POS_ISpinBox_editingFinished()
{
    int index = 0;

    switch(ui->adjustGroupComboBox->currentIndex()) {
    case 0:
        index = SEV_POSITION_I;
        break;
    case 1:
        index = M_POSITION_I;
        break;
    case 2:
        index = L_POSITION_I;
        break;
    }
    setNewPID(ui->POS_ISpinBox->value(),index);

    uint16_t data_L = 0;

    can1->controller.GetValueInTable(jointBeingUsed->ID, index,data_L);
    ui->POS_ISpinBox->setValue(data_L);

}

void MainWindow::on_POS_DSpinBox_editingFinished()
{
    int index = 0;

    switch(ui->adjustGroupComboBox->currentIndex()) {
    case 0:
        index = SEV_POSITION_D;
        break;
    case 1:
        index = M_POSITION_D;
        break;
    case 2:
        index = L_POSITION_D;
        break;
    }
    setNewPID(ui->POS_DSpinBox->value(),index);

    uint16_t data_L = 0;

    can1->controller.GetValueInTable(jointBeingUsed->ID, index,data_L);
    ui->POS_DSpinBox->setValue(data_L);

}

void MainWindow::on_POS_DSSpinBox_editingFinished()
{
    int index = 0;

    switch(ui->adjustGroupComboBox->currentIndex()) {
    case 0:
        index = SEV_POSITION_DS;
        break;
    case 1:
        index = M_POSITION_DS;
        break;
    case 2:
        index = L_POSITION_DS;
        break;
    }
    setNewPID(ui->POS_DSSpinBox->value(),index);

    uint16_t data_L = 0;

    can1->controller.GetValueInTable(jointBeingUsed->ID, index,data_L);
    ui->POS_DSSpinBox->setValue(data_L);

}

void MainWindow::on_SPD_PSpinBox_editingFinished()
{
    int index = 0;

    switch(ui->adjustGroupComboBox->currentIndex()) {
    case 0:
        index = SEV_SPEED_P;
        break;
    case 1:
        index = M_SPEED_P;
        break;
    case 2:
        index = L_SPEED_P;
        break;
    }
    setNewPID(ui->SPD_PSpinBox->value(),index);

    uint16_t data_L = 0;

    can1->controller.GetValueInTable(jointBeingUsed->ID, index,data_L);
    ui->SPD_PSpinBox->setValue(data_L);

}

void MainWindow::on_SPD_ISpinBox_editingFinished()
{
    int index = 0;

    switch(ui->adjustGroupComboBox->currentIndex()) {
    case 0:
        index = SEV_SPEED_I;
        break;
    case 1:
        index = M_SPEED_I;
        break;
    case 2:
        index = L_SPEED_I;
        break;
    }
    setNewPID(ui->SPD_ISpinBox->value(),index);

    uint16_t data_L = 0;

    can1->controller.GetValueInTable(jointBeingUsed->ID, index,data_L);
    ui->SPD_ISpinBox->setValue(data_L);

}

void MainWindow::on_SPD_DSpinBox_editingFinished()
{
    int index = 0;

    switch(ui->adjustGroupComboBox->currentIndex()) {
    case 0:
        index = SEV_SPEED_D;
        break;
    case 1:
        index = M_SPEED_D;
        break;
    case 2:
        index = L_SPEED_D;
        break;
    }
    setNewPID(ui->SPD_DSpinBox->value(),index);

    uint16_t data_L = 0;

    can1->controller.GetValueInTable(jointBeingUsed->ID, index,data_L);
    ui->SPD_DSpinBox->setValue(data_L);

}

void MainWindow::on_SPD_DSSpinBox_editingFinished()
{
    int index = 0;

    switch(ui->adjustGroupComboBox->currentIndex()) {
    case 0:
        index = SEV_SPEED_DS;
        break;
    case 1:
        index = M_SPEED_DS;
        break;
    case 2:
        index = L_SPEED_DS;
        break;
    }
    setNewPID(ui->SPD_DSSpinBox->value(),index);

    uint16_t data_L = 0;

    can1->controller.GetValueInTable(jointBeingUsed->ID, index,data_L);
    ui->SPD_DSSpinBox->setValue(data_L);

}

void MainWindow::on_CUR_PSpinBox_editingFinished()
{
    int index = 0;

    switch(ui->adjustGroupComboBox->currentIndex()) {
    case 0:
        index = SEV_CURRENT_P;
        break;
    case 1:
        index = M_CURRENT_P;
        break;
    case 2:
        index = L_CURRENT_P;
        break;
    }
    setNewPID(ui->CUR_PSpinBox->value(),index);

    uint16_t data_L = 0;

    can1->controller.GetValueInTable(jointBeingUsed->ID, index,data_L);
    ui->CUR_PSpinBox->setValue(data_L);

}

void MainWindow::on_CUR_ISpinBox_editingFinished()
{
    int index = 0;

    switch(ui->adjustGroupComboBox->currentIndex()) {
    case 0:
        index = SEV_CURRENT_I;
        break;
    case 1:
        index = M_CURRENT_I;
        break;
    case 2:
        index = L_CURRENT_I;
        break;
    }
    setNewPID(ui->CUR_ISpinBox->value(),index);

    uint16_t data_L = 0;

    can1->controller.GetValueInTable(jointBeingUsed->ID, index,data_L);
    ui->CUR_ISpinBox->setValue(data_L);

}

void MainWindow::on_CUR_DSpinBox_editingFinished()
{
    int index = 0;

    switch(ui->adjustGroupComboBox->currentIndex()) {
    case 0:
        index = SEV_CURRENT_D;
        break;
    case 1:
        index = M_CURRENT_D;
        break;
    case 2:
        index = L_CURRENT_D;
        break;
    }
    setNewPID(ui->CUR_DSpinBox->value(),index);

    uint16_t data_L = 0;

    can1->controller.GetValueInTable(jointBeingUsed->ID, index,data_L);
    ui->CUR_DSpinBox->setValue(data_L);

}

void MainWindow::on_maxCurLineEdit_editingFinished()
{
    setNewPID(ui->maxCurLineEdit->text().toInt(),LIT_MAX_CURRENT);

    uint16_t data_L = 0;

    can1->controller.GetValueInTable(jointBeingUsed->ID, LIT_MAX_CURRENT,data_L);
    ui->maxCurLineEdit->setText(QString::number(data_L, 10));
}

void MainWindow::on_maxSpdLineEdit_editingFinished()
{
    setNewPID(ui->maxSpdLineEdit->text().toInt(),LIT_MAX_SPEED);

    uint16_t data_L = 0;

    can1->controller.GetValueInTable(jointBeingUsed->ID, LIT_MAX_SPEED,data_L);
    ui->maxSpdLineEdit->setText(QString::number(data_L, 10));
}

void MainWindow::on_maxAccLineEdit_editingFinished()
{
    setNewPID(ui->maxAccLineEdit->text().toInt(),LIT_MAX_ACC);

    uint16_t data_L = 0;

    can1->controller.GetValueInTable(jointBeingUsed->ID, LIT_MAX_ACC,data_L);
    ui->maxAccLineEdit->setText(QString::number(data_L, 10));
}

void MainWindow::on_minPosLLineEdit_editingFinished()
{
    setNewPID(ui->minPosLLineEdit->text().toInt(),LIT_MIN_POSITION_L);

    uint16_t data_L = 0;

    can1->controller.GetValueInTable(jointBeingUsed->ID, LIT_MIN_POSITION_L,data_L);
    ui->minPosLLineEdit->setText(QString::number(data_L, 10));
}

void MainWindow::on_maxPosLLineEdit_editingFinished()
{
    setNewPID(ui->maxPosLLineEdit->text().toInt(),LIT_MAX_POSITION_L);

    uint16_t data_L = 0;

    can1->controller.GetValueInTable(jointBeingUsed->ID, LIT_MAX_POSITION_L,data_L);
    ui->maxPosLLineEdit->setText(QString::number(data_L, 10));
}

void MainWindow::on_minPosHLineEdit_editingFinished()
{
    setNewPID(ui->minPosHLineEdit->text().toInt(),LIT_MIN_POSITION_H);

    uint16_t data_L = 0;

    can1->controller.GetValueInTable(jointBeingUsed->ID, LIT_MIN_POSITION_H,data_L);
    ui->minPosHLineEdit->setText(QString::number(data_L, 10));
}

void MainWindow::on_maxPosHLineEdit_editingFinished()
{
    setNewPID(ui->maxPosHLineEdit->text().toInt(),LIT_MAX_POSITION_H);

    uint16_t data_L = 0;

    can1->controller.GetValueInTable(jointBeingUsed->ID, LIT_MAX_POSITION_H,data_L);
    ui->maxPosHLineEdit->setText(QString::number(data_L, 10));
}
