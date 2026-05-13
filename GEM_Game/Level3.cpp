#include "Level3.h"

#include <QBrush>
#include <QColor>
#include <QPen>
#include <QPixmap>
#include <QInputDialog>
#include <QMessageBox>
#include <QApplication>

/* ================================================================
   CONSTRUCTOR
   ================================================================ */

Level3::Level3()
{
    treasure = nullptr;

    treasureOpened = false;

    passwordSolved = false;

    closedTreasure.load(
        ":/new/prefix1/images/Treasureclosed.png"
        );

    openTreasure.load(
        ":/new/prefix1/images/Treasureopened.png"
        );

    treasureRect =
        QRect(
            1280,
            120,
            140,
            140
            );
}

/* ================================================================
   LOAD SCENE
   ================================================================ */

void Level3::loadScene(
    QGraphicsScene* scene
    )
{
    //================ BACKGROUND =================//

    QPixmap bg(
        ":/new/prefix1/images/level3.png"
        );

    background =
        scene->addPixmap(bg);

    scene->setSceneRect(
        bg.rect()
        );

    //================ TREASURE =================//

    treasure =
        scene->addPixmap(
            closedTreasure
            );

    treasure->setPos(
        treasureRect.x(),
        treasureRect.y()
        );

    treasure->setScale(0.22);

    treasure->setZValue(900);
}

/* ================================================================
   TREASURE COLLISION
   ================================================================ */

void Level3::checkTreasureCollision(
    QGraphicsPixmapItem* playerSprite
    )
{
    if(passwordSolved)
    {
        return;
    }

    if(!playerSprite || !treasure)
    {
        return;
    }

    QRectF playerRect =
        playerSprite->sceneBoundingRect();

    QRectF treasureArea =
        treasure->sceneBoundingRect();

    if(playerRect.intersects(treasureArea))
    {
        showPasswordPopup();
    }
}

/* ================================================================
   PASSWORD POPUP
   ================================================================ */

void Level3::showPasswordPopup()
{
    bool ok;

    QString text =
        QInputDialog::getText(
            nullptr,
            "Ancient Password",
            "Enter the 3-letter password:",
            QLineEdit::Normal,
            "",
            &ok
            );

    if(ok)
    {
        if(text.toUpper() == "MAN")
        {
            passwordSolved = true;

            treasureOpened = true;

            if(treasure)
            {
                treasure->setPixmap(
                    openTreasure
                    );
            }

            showTreasurePopup();
        }

        else
        {
            QMessageBox::warning(
                nullptr,
                "Wrong Password",
                "The treasure remains sealed."
                );
        }
    }
}

/* ================================================================
   TREASURE RECOVERED POPUP
   ================================================================ */

void Level3::showTreasurePopup()
{
    QMessageBox msg;

    msg.setWindowTitle(
        "Artifact Recovered"
        );

    msg.setText(
        "You recovered the legendary artifact!"
        );

    msg.setInformativeText(
        "The museum's greatest treasure has finally been found."
        );

    msg.setIconPixmap(
        QPixmap(
            ":/new/prefix1/images/opened_treasure.png"
            ).scaled(
                220,
                220,
                Qt::KeepAspectRatio,
                Qt::SmoothTransformation
                )
        );

    msg.exec();

    showVictoryPopup();
}

/* ================================================================
   FINAL VICTORY POPUP
   ================================================================ */

void Level3::showVictoryPopup()
{
    QWidget* overlay =
        new QWidget();

    overlay->setGeometry(
        300,
        100,
        900,
        700
        );

    overlay->setStyleSheet(
        "background-color:rgba(0,0,0,220);"
        );

    overlay->show();

    //================ CARD =================//

    QWidget* card =
        new QWidget(overlay);

    card->setFixedSize(700,500);

    card->move(100,100);

    card->setStyleSheet(
        "background-color:rgba(8,4,0,240);"
        "border:2px solid #c8a84b;"
        "border-radius:14px;"
        );

    //================ LAYOUT =================//

    QVBoxLayout* layout =
        new QVBoxLayout(card);

    layout->setContentsMargins(
        40,
        30,
        40,
        30
        );

    layout->setSpacing(20);

    //================ TITLE =================//

    QLabel* title =
        new QLabel(
            "HERITAGE PROTECTED"
            );

    title->setAlignment(
        Qt::AlignCenter
        );

    title->setStyleSheet(
        "font-size:34px;"
        "font-weight:bold;"
        "color:#f5d060;"
        "letter-spacing:6px;"
        "background:transparent;"
        "border:none;"
        );

    //================ IMAGE =================//

    QLabel* img =
        new QLabel();

    img->setPixmap(
        QPixmap(
            ":/new/prefix1/images/opened_treasure.png"
            ).scaled(
                320,
                320,
                Qt::KeepAspectRatio,
                Qt::SmoothTransformation
                )
        );

    img->setAlignment(
        Qt::AlignCenter
        );

    //================ TEXT =================//

    QLabel* text =
        new QLabel(
            "The sacred relic of Tutankhamun\n"
            "has finally been recovered.\n\n"

            "Tonight, history survives because of you."
            );

    text->setAlignment(
        Qt::AlignCenter
        );

    text->setStyleSheet(
        "font-size:17px;"
        "color:#e8d5a8;"
        "background:transparent;"
        "border:none;"
        );

    //================ BUTTONS =================//

    QPushButton* playAgain =
        new QPushButton(
            "▶   PLAY AGAIN"
            );

    QPushButton* exitBtn =
        new QPushButton(
            "✕   EXIT"
            );

    playAgain->setFixedHeight(54);

    exitBtn->setFixedHeight(54);

    QString style =
        "QPushButton {"
        "background-color:rgba(180,130,40,220);"
        "color:white;"
        "font-size:16px;"
        "font-weight:bold;"
        "letter-spacing:3px;"
        "border-radius:8px;"
        "border:2px solid #c8a84b;"
        "}"

        "QPushButton:hover {"
        "background-color:rgba(220,170,60,240);"
        "border:2px solid white;"
        "}";

    playAgain->setStyleSheet(style);

    exitBtn->setStyleSheet(style);

    //================ ADD =================//

    layout->addWidget(title);

    layout->addWidget(img);

    layout->addWidget(text);

    layout->addStretch();

    layout->addWidget(playAgain);

    layout->addWidget(exitBtn);

    card->show();

    //================ PLAY AGAIN =================//

    QBoxLayout::connect(
        playAgain,
        &QPushButton::clicked,
        [=]()
        {
            QApplication::quit();
        }
        );

    //================ EXIT =================//

    QBoxLayout::connect(
        exitBtn,
        &QPushButton::clicked,
        [=]()
        {
            QApplication::quit();
        }
        );
}
