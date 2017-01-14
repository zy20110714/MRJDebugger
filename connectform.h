#ifndef CONNECTFORM_H
#define CONNECTFORM_H

#include <QWidget>

namespace Ui {
class connectForm;
}

class connectForm : public QWidget
{
    Q_OBJECT

public:
    explicit connectForm(QWidget *parent = 0);
    ~connectForm();

private:
    Ui::connectForm *ui;
};

#endif // CONNECTFORM_H
