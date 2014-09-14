#include "userwidget.h"
#include <QPixmap>
#include <QPainter>
#include <QDebug>

UserWidget::UserWidget(QWidget *parent) :
    QWidget(parent)
{
}

void UserWidget::paintImage(QByteArray a)
{
    p.loadFromData(a);
    update();
}

void UserWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);
    double rate = (double)rect().width() / (double)rect().height();
    double cx = p.rect().center().x();
    double cy = p.rect().center().y();
    double lx = p.rect().width();
    double ly = p.rect().height();
    if (lx / ly < rate) {
        ly = lx / rate;
    } else {
        lx = ly * rate;
    }
    painter.drawPixmap(rect(), p, QRect(cx - lx / 2, cy - ly / 2, lx, ly));

}
