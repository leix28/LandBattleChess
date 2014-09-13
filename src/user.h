#ifndef USER_H
#define USER_H

#include <QString>
#include <QByteArray>

class User
{
public:
    QString name;
    QByteArray image;
    User();
    void load();
    void clear();
};

#endif // USER_H
