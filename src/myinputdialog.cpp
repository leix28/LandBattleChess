#include "myinputdialog.h"
#include <QGridLayout>
#include <QPushButton>

MyInputDialog::MyInputDialog(QWidget *parent) :
    QDialog(parent)
{
    line.setText("127.0.0.1");
    QGridLayout *layout = new QGridLayout();
    layout->addWidget(&line, 0, 0, 1, 2);

    this->setLayout(layout);
    for (int i = 0; i < 10; i++) {
        QPushButton *button = new QPushButton(QString().number(i), this);
        layout->addWidget(button, i / 3 + 1, i % 3);
        QObject::connect(button, SIGNAL(clicked()), this, SLOT(handle()));
    }
    QPushButton *button = new QPushButton(".", this);
    QObject::connect(button, SIGNAL(clicked()), this, SLOT(handle()));
    layout->addWidget(button, 4, 1);
    button = new QPushButton("Clear", this);
    QObject::connect(button, SIGNAL(clicked()), this, SLOT(handle()));
    layout->addWidget(button, 4, 2);
    button = new QPushButton("OK", this);
    button->setFocus();
    QObject::connect(button, SIGNAL(clicked()), this, SLOT(handle()));
    layout->addWidget(button, 0, 2);

}

QString MyInputDialog::getIp()
{
    MyInputDialog dialog;
    dialog.exec();
    return dialog.line.text();
}

void MyInputDialog::handle()
{
    QPushButton* btn = (QPushButton*)QObject::sender();
    if (btn->text() == "OK")
        close();
    else if (btn->text() == "Clear")
        line.setText("");
    else
        line.setText(line.text() + btn->text());
}
