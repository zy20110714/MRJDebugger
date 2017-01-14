#include "connectform.h"
#include "ui_connectform.h"

connectForm::connectForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::connectForm)
{
    ui->setupUi(this);
}

connectForm::~connectForm()
{
    delete ui;
}
