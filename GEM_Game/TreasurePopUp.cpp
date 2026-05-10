#include "TreasurePopup.h"

TreasurePopup::TreasurePopup(QWidget *parent)
    : QDialog(parent)
{
    setFixedSize(1000, 700);

    setWindowFlags(Qt::FramelessWindowHint);

    setStyleSheet(
        "background-color: rgba(0,0,0,220);"
        );

    // GIF LABEL
    gifLabel = new QLabel(this);

    gifLabel->setAlignment(Qt::AlignCenter);

    movie = new QMovie(":/images/treasure.gif");

    gifLabel->setMovie(movie);

    move->start();

    // WIN IMAGE
    winImage = new QLabel(this);

    winImage->setPixmap(
        QPixmap(":/images/youwin.png")
        );

    winImage->setScaledContents(true);

    winImage->hide();

    // LAYOUT
    QVBoxLayout *layout = new QVBoxLayout(this);

    layout->addWidget(gifLabel);

    layout->addWidget(winImage);

    setLayout(layout);

    // AFTER GIF ENDS SHOW WIN SCREEN
    QTimer::singleShot(5000, this, [=]()
                       {
                           gifLabel->hide();

                           winImage->show();
                       });
}
