#include "guessdialog.h"
#include "chessmodel.h"
#include <QGridLayout>

GuessDialog::GuessDialog(QWidget *parent) :
    QDialog(parent)
{
    index = 13;
    QGridLayout *layout = new QGridLayout;
    for (int i = 1; i <= 12; i++) {
        btns[i].setText(ChessModel::pieceName[i]);
        layout->addWidget(btns + i, (i - 1) / 3, (i - 1) % 3);
        QObject::connect(btns + i, SIGNAL(clicked()), this, SLOT(guess()));
    }
    this->setLayout(layout);
}

int GuessDialog::getGuess(QWidget *parent)
{
    GuessDialog dialog(parent);
    if (dialog.exec() == QDialog::Accepted) {
        return dialog.index;
    }
    return 13;
}

void GuessDialog::guess()
{
    QPushButton* ptr = (QPushButton*)QObject::sender();
    for (int i = 1; i <= 12; i++)
        if (ptr == &btns[i]) {
            index = i;
            accept();
        }
}
