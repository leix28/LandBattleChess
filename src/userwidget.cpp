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
    painter.drawPixmap(rect(), p, p.rect());
}
