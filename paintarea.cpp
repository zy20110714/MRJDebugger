#include "paintarea.h"
#include "mainwindow.h"
#include <QPainter>
#include <QPointF>
#include <iostream>

using std::vector;
using std::cout;
using std::endl;

// Gets the size of the Widget in the layout
void PaintArea::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    this->setGeometry(this->rect());
    // Otherwise, the default value is always 100

//    //初始化数组pointX，当做要绘制曲线的X坐标，X坐标的值从0到Width - 1
//    pointX.clear();
//    for (int i = 0; i < this->width(); i++)
//    {
//        pointX.append(i);
//    }

    //由记录对象标志初始化要显示的测量曲线
    showItems.clear();
    QPen pen;
//    pen.color().getRgb(r,g,b);
    pen.setBrush(QColor(TGCUR_RGB_R,TGCUR_RGB_G,TGCUR_RGB_B));
    showItems.insert(0,*(new ShowItem((this->Mask & MASK_TAGCUR) != 0x00,
                                               TAG_CURRENT_L,//SCP_TAGCUR_L,
                                               pen,
                                               this->width())));
    // pen.setBrush(Qt::green);
    pen.setBrush(QColor(TGSPD_RGB_R,TGSPD_RGB_G,TGSPD_RGB_B));
    showItems.insert(1,*(new ShowItem((this->Mask & MASK_TAGSPD) != 0x00,
                                               TAG_SPEED_L,//SCP_TAGSPD_L,
                                               pen,
                                               this->width())));
    // pen.setBrush(Qt::red);
    pen.setBrush(QColor(TGPOS_RGB_R,TGPOS_RGB_G,TGPOS_RGB_B));
    showItems.insert(2,*(new ShowItem((this->Mask & MASK_TAGPOS) != 0x00,
                                               TAG_POSITION_L,//SCP_TAGPOS_L,
                                               pen,
                                               this->width())));
    // pen.setBrush(Qt::white);
    pen.setBrush(QColor(RLCUR_RGB_R,RLCUR_RGB_G,RLCUR_RGB_B));
    showItems.insert(3,*(new ShowItem((this->Mask & MASK_MEACUR) != 0x00,
                                               SYS_CURRENT_L,//SCP_MEACUR_L,
                                               pen,
                                               this->width())));
    // pen.setBrush(Qt::yellow);
    pen.setBrush(QColor(RLSPD_RGB_R,RLSPD_RGB_G,RLSPD_RGB_B));
    showItems.insert(4,*(new ShowItem((this->Mask & MASK_MEASPD) != 0x00,
                                               SYS_SPEED_L,//SCP_MEASPD_L,
                                               pen,
                                               this->width())));
    // pen.setBrush(Qt::magenta);
    pen.setBrush(QColor(RLPOS_RGB_R,RLPOS_RGB_G,RLPOS_RGB_B));
    showItems.insert(5,*(new ShowItem((this->Mask & MASK_MEAPOS) != 0x00,
                                               SYS_POSITION_L,//SCP_MEAPOS_L,
                                               pen,
                                               this->width())));
    for (int i = 0; i < showItems.count(); i++) {
        showItems[i].sq->FillZero();
    }
}

PaintArea::PaintArea(QWidget *parent) :
    QWidget(parent)
{
//    cout << this->geometry().width() << endl;
//    cout.flush();

    setPalette(QPalette(Qt::gray));
    setAutoFillBackground(true);

    Mask = 0;
    //另外的线程中需要引用
    Interval = 10; // Need additional initialization
    EnableScope = true;
    // ShowItem and Mask would initialize in mainwindow_OscilloScope.cpp

    //电流、速度、位置值的单位转换比率
    speedRatio = 1;//60.0 / 65536;
    positionRatio = 360.0 / 65536;
    currentRatio = 1.0;
//    //电流、速度、位置值在Y轴上的缩放比例 // Initialize in MainWindow::OscilloScope()
//    cr = 1;
//    sr = 1;
//    pr = 1;
    //    //从文件设置电流、速度、位置（Y轴）缩放比例，并显示到界面
    //    cr = Convert.ToDouble(IniFile.ContentValue("plRange", "Current", IniFile.StrProPath));
    //    sr = Convert.ToDouble(IniFile.ContentValue("plRange", "Speed", IniFile.StrProPath));
    //    pr = Convert.ToDouble(IniFile.ContentValue("plRange", "Position", IniFile.StrProPath));
    //电流、速度、位置曲线沿Y轴的偏移值
    currentOffset = 0.0f;
    speedOffset = 0.0f; ;
    positionOffset = 0.0f;
}

void PaintArea::paintEvent(QPaintEvent *)
{
    QPoint point1;
    QPoint point2;

    // 设置画笔
    QPainter p1(this);
    pen.setStyle(Qt::SolidLine);
    pen.setWidth(1.8);
    pen.setBrush(Qt::black);
    p1.setPen(pen);
    // 在中间画1条横向实线
    point1 = QPoint(0,this->height() / 2);
    point2 = QPoint(this->width(), this->height() / 2);
    p1.drawLine(point1,point2);

//    cout << this->width() << endl; // width == 621
//    cout.flush();
//    cout << this->height() << endl; // height == 551
//    cout.flush();

    // 在中间画1条纵向实线
    point1 = QPoint(this->width() / 2, 0);
    point2 = QPoint(this->width() / 2, this->height());
    p1.drawLine(point1,point2);

    //隔 YDotNum 个数据点画1条横向虚线，给画笔指定颜色、线宽及线型
    const int YDotNum = 30;
    pen.setStyle(Qt::DashLine);
    pen.setWidth(1);
    pen.setBrush(Qt::darkGray);
    p1.setPen(pen);
    for (int i = 1; this->height() / 2 + i * YDotNum < this->height(); i++)
    {
        //隔 YDotNum 画中间线以下的横向虚线
        point1 = QPoint(0, this->height() / 2 + i * YDotNum);
        point2 = QPoint(this->width(), this->height() / 2 + i * YDotNum);
        p1.drawLine(point1,point2);
        //隔 YDotNum 画中间线以上的横向虚线
        point1 = QPoint(0, this->height() / 2 - i * YDotNum);
        point2 = QPoint(this->width(), this->height() / 2 - i * YDotNum);
        p1.drawLine(point1,point2);
    }

    //分别处理每条要测量的曲线
    for ( int i = 0; i < showItems.count(); i++ ) {
        //若队列非空并且勾选显示
        if (!(showItems[i].sq->IsEmpty()) && showItems[i].IsCheck) {
            // 由队列元素个数创建数组
            vector<double> pointY(showItems[i].sq->Count());
            // 将队列元素的值赋给数组的每个元素
            showItems[i].sq->Print(pointY);
            int m = pointY.size();
            // 由队列元素个数创建坐标对
            if (m > 1)
            {
                // set color of pen
                p1.setPen(showItems[i].PenForThisItem);
                double ratio;
                double offset;
                double r;
                //currentRatio - 单位转换比率，cr - Y轴缩放比例，YDotNum - 图上单位长度，Y方向的0位在屏幕顶端，(pointY[j] * currentRatio)带单位的数值
                switch (showItems[i].Item) {
                case TAG_CURRENT_L://(showItems[0].Item)://SCP_TAGCUR_L:
                case SYS_CURRENT_L://showItems[3].Item://SCP_MEACUR_L://若曲线是电流
                    ratio = currentRatio;
                    offset = currentOffset;
                    r = cr;
                    break;
                case TAG_SPEED_L://showItems[1].Item://SCP_TAGSPD_L:
                case SYS_SPEED_L://showItems[4].Item://SCP_MEASPD_L://若曲线是速度
                    ratio = speedRatio;
                    offset = speedOffset;
                    r = sr;
                    break;
                case TAG_POSITION_L://showItems[2].Item://SCP_TAGPOS_L:
                case SYS_POSITION_L://showItems[5].Item://SCP_MEAPOS_L://若曲线是位置
                    ratio = positionRatio;
                    offset = positionOffset;
                    r = pr;
                    break;
                }
                for (int j = 0; j < m - 1; j++) {//分别处理每一坐标对
                    double verticalValue1 = this->height() / 2.0 - (pointY[j] * ratio + offset) / r * YDotNum;
                    double verticalValue2 = this->height() / 2.0 - (pointY[j+1] * ratio + offset) / r * YDotNum;
                    point1 = QPoint(j, verticalValue1);
                    point2 = QPoint(j+1, verticalValue2);
                    p1.drawLine(point1,point2);
                    update();
                }
            }
        }
    }
}
