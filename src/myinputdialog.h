#ifndef MYINPUTDIALOG_H
#define MYINPUTDIALOG_H

#include <QDialog>
#include <QLineEdit>

class MyInputDialog : public QDialog
{
    Q_OBJECT
    QLineEdit line;
public:
    explicit MyInputDialog(QWidget *parent = 0);
    static QString getIp();
signals:

public slots:
    void handle();
};

#endif // MYINPUTDIALOG_H
