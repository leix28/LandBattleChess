#ifndef USERWIDGET_H
#define USERWIDGET_H

#include <QWidget>

class UserWidget : public QWidget
{
    Q_OBJECT
    QPixmap p;
public:
    explicit UserWidget(QWidget *parent = 0);
    void paintImage(QByteArray a);

protected:
    void paintEvent(QPaintEvent *event);

signals:

public slots:

};

#endif // USERWIDGET_H
