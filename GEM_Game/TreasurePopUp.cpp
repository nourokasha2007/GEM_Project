#include "TreasurePopup.h"

TreasurePopup::TreasurePopup(QWidget *parent) : QDialog(parent)
{
    setFixedSize(1000, 700);
    setWindowFlags(Qt::FramelessWindowHint);
    setStyleSheet("background-color: rgba(0,0,0,220);");

    gifLabel = new QLabel(this);
    gifLabel->setAlignment(Qt::AlignCenter);
    movie = new QMovie(":/images/treasure.gif");
    gifLabel->setMovie(movie);
    movie->start();

    winImage = new QLabel(this);
    winImage->setPixmap(QPixmap(":/images/youwin.png"));
    winImage->setScaledContents(true);
    winImage->hide();

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(gifLabel);
    layout->addWidget(winImage);
    setLayout(layout);

    QTimer::singleShot(5000, this, [=]() {
        gifLabel->hide();
        winImage->show();
    });
}
