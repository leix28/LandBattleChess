#ifndef GUESSDIALOG_H
#define GUESSDIALOG_H

#include <QDialog>
#include <QPushButton>

class GuessDialog : public QDialog
{
    Q_OBJECT
    QPushButton btns[13];
    int index;
public:
    explicit GuessDialog(QWidget *parent = 0);
    static int getGuess(QWidget *parent);
signals:

public slots:
    void guess();

};

#endif // GUESSDIALOG_H
