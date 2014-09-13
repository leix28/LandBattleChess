#include "user.h"
#include <QInputDialog>
#include <QFileDialog>
User::User()
{
}

void User::load()
{
    name = QInputDialog::getText(0, "UserName", "UserName");
    QFile file(QFileDialog::getOpenFileName(0, QString("Open Avatar"), QDir::currentPath(), QString("Images (*.jpg)")));
    file.open(QIODevice::ReadOnly);
    image = file.readAll();
}

void User::clear()
{
    name = "";
    image = QByteArray();
}
