
// #include "Level3.h"
// #include <QBrush>
// #include <QColor>
// #include <QPen>
// #include <QPixmap>
// #include <QInputDialog>
// #include <QMessageBox>
// #include <QApplication>
// #include <QObject>
// #include <functional>

// /* ================================================================
//    CONSTRUCTOR
//    ================================================================ */

// Level3::Level3()
// {
//     treasure = nullptr;

//     treasureOpened = false;

//     passwordSolved = false;

//     popupShown = false;

//     collisionMask.load(
//         ":/new/prefix1/images/level3 BW.png"
//         );

//     closedTreasure.load(
//         ":/new/prefix1/images/Treasureclosed.png"
//         );

//     openTreasure.load(
//         ":/new/prefix1/images/Treasureopened.png"
//         );

//     // Treasure sits at the base of the Sphinx, center-front of the scene
//     treasureRect =
//         QRect(
//             820,
//             530,
//             160,
//             160
//             );
// }

// /* ================================================================
//    LOAD SCENE
//    ================================================================ */

// void Level3::loadScene(
//     QGraphicsScene* scene
//     )
// {
//     //================ BACKGROUND =================//

//     QPixmap bg(
//         ":/new/prefix1/images/level3.png"
//         );

//     background =
//         scene->addPixmap(bg);

//     scene->setSceneRect(
//         bg.rect()
//         );


//     //================ TREASURE =================//

//     treasure =
//         scene->addPixmap(
//             closedTreasure
//             );

//     treasure->setPos(
//         treasureRect.x(),
//         treasureRect.y()
//         );

//     treasure->setScale(0.09);

//     treasure->setZValue(900);
// }

// /* ================================================================
//    TREASURE COLLISION
//    ================================================================ */

// void Level3::checkTreasureCollision(
//     QGraphicsPixmapItem* playerSprite
//     )
// {
//     if(passwordSolved)
//     {
//         return;
//     }

//     if(!playerSprite || !treasure)
//     {
//         return;
//     }

//     QRectF playerRect =
//         playerSprite->sceneBoundingRect();

//     // Use the defined QRect trigger zone (in front of the Sphinx)
//     QRectF treasureArea = QRectF(treasureRect);

//     if(playerRect.intersects(treasureArea) && !popupShown)
//     {
//         popupShown =true;
//         showPasswordPopup();
//     }
// }

// /* ================================================================
//    PASSWORD POPUP
//    ================================================================ */

// void Level3::showPasswordPopup()
// {
//     bool ok;

//     QString text =
//         QInputDialog::getText(
//             nullptr,
//             "Ancient Password",
//             "Enter the 3-letter password:",
//             QLineEdit::Normal,
//             "",
//             &ok
//             );

//     if(ok)
//     {
//         if(text.toUpper() == "MAN")
//         {
//             passwordSolved = true;

//             treasureOpened = true;

//             if(treasure)
//             {
//                 treasure->setPixmap(
//                     openTreasure
//                     );
//             }

//             showTreasurePopup();
//         }

//         else
//         {
//             QMessageBox::warning(
//                 nullptr,
//                 "Wrong Password",
//                 "The treasure remains sealed."
//                 );
//         }
//     }
// }

// /* ================================================================
//    TREASURE RECOVERED POPUP
//    ================================================================ */

// void Level3::showTreasurePopup()
// {
//     QMessageBox msg;

//     msg.setWindowTitle(
//         "Artifact Recovered"
//         );

//     msg.setText(
//         "You recovered the legendary artifact!"
//         );

//     msg.setInformativeText(
//         "The museum's greatest treasure has finally been found."
//         );

//     msg.setIconPixmap(
//         QPixmap(
//             ":/new/prefix1/images/opened_treasure.png"
//             ).scaled(
//                 220,
//                 220,
//                 Qt::KeepAspectRatio,
//                 Qt::SmoothTransformation
//                 )
//         );

//     msg.exec();

//     showVictoryPopup();
// }

// /* ================================================================
//    FINAL VICTORY POPUP
//    ================================================================ */

// void Level3::showVictoryPopup()
// {
//     QWidget* overlay =
//         new QWidget();

//     overlay->setGeometry(
//         300,
//         100,
//         900,
//         700
//         );

//     overlay->setStyleSheet(
//         "background-color:rgba(0,0,0,220);"
//         );

//     overlay->show();

//     //================ CARD =================//

//     QWidget* card =
//         new QWidget(overlay);

//     card->setFixedSize(700,500);

//     card->move(100,100);

//     card->setStyleSheet(
//         "background-color:rgba(8,4,0,240);"
//         "border:2px solid #c8a84b;"
//         "border-radius:14px;"
//         );

//     //================ LAYOUT =================//

//     QVBoxLayout* layout =
//         new QVBoxLayout(card);

//     layout->setContentsMargins(
//         40,
//         30,
//         40,
//         30
//         );

//     layout->setSpacing(20);

//     //================ TITLE =================//

//     QLabel* title =
//         new QLabel(
//             "HERITAGE PROTECTED"
//             );

//     title->setAlignment(
//         Qt::AlignCenter
//         );

//     title->setStyleSheet(
//         "font-size:34px;"
//         "font-weight:bold;"
//         "color:#f5d060;"
//         "letter-spacing:6px;"
//         "background:transparent;"
//         "border:none;"
//         );

//     //================ IMAGE =================//

//     QLabel* img =
//         new QLabel();

//     img->setPixmap(
//         QPixmap(
//             ":/new/prefix1/images/opened_treasure.png"
//             ).scaled(
//                 320,
//                 320,
//                 Qt::KeepAspectRatio,
//                 Qt::SmoothTransformation
//                 )
//         );

//     img->setAlignment(
//         Qt::AlignCenter
//         );

//     //================ TEXT =================//

//     QLabel* text =
//         new QLabel(
//             "The sacred relic of Tutankhamun\n"
//             "has finally been recovered.\n\n"

//             "Tonight, history survives because of you."
//             );

//     text->setAlignment(
//         Qt::AlignCenter
//         );

//     text->setStyleSheet(
//         "font-size:17px;"
//         "color:#e8d5a8;"
//         "background:transparent;"
//         "border:none;"
//         );

//     //================ BUTTONS =================//

//     QPushButton* playAgain =
//         new QPushButton(
//             "▶   PLAY AGAIN"
//             );

//     QPushButton* exitBtn =
//         new QPushButton(
//             "✕   EXIT"
//             );

//     playAgain->setFixedHeight(54);

//     exitBtn->setFixedHeight(54);

//     QString style =
//         "QPushButton {"
//         "background-color:rgba(180,130,40,220);"
//         "color:white;"
//         "font-size:16px;"
//         "font-weight:bold;"
//         "letter-spacing:3px;"
//         "border-radius:8px;"
//         "border:2px solid #c8a84b;"
//         "}"

//         "QPushButton:hover {"
//         "background-color:rgba(220,170,60,240);"
//         "border:2px solid white;"
//         "}";

//     playAgain->setStyleSheet(style);

//     exitBtn->setStyleSheet(style);

//     //================ ADD =================//

//     layout->addWidget(title);

//     layout->addWidget(img);

//     layout->addWidget(text);

//     layout->addStretch();

//     layout->addWidget(playAgain);

//     layout->addWidget(exitBtn);

//     card->show();

//     //================ PLAY AGAIN =================//

//     QObject::connect(
//         playAgain,
//         &QPushButton::clicked,
//         [=]()
//         {
//             overlay->deleteLater();
//             if(restartCallback)
//                 restartCallback();
//         }
//         );

//     //================ EXIT =================//

//     //================ EXIT =================//

//     QObject::connect(
//         exitBtn,
//         &QPushButton::clicked,
//         [=]()
//         {
//             QApplication::quit();
//         }
//         );

// } // <--- ADD THIS BRACE HERE
#include "Level3.h"
#include <QBrush>
#include <QColor>
#include <QPen>
#include <QPixmap>
#include <QApplication>
#include <QObject>
#include <QScreen>
#include <functional>

/* ================================================================
   CONSTRUCTOR
   ================================================================ */

Level3::Level3()
{
    treasure        = nullptr;
    treasureOpened  = false;
    passwordSolved  = false;
    popupShown      = false;

    collisionMask.load(
        ":/new/prefix1/images/level3 BW.png"
        );

    closedTreasure.load(
        ":/new/prefix1/images/Treasureclosed.png"
        );

    openTreasure.load(
        ":/new/prefix1/images/Treasureopened.png"
        );

    // Trigger zone: in front of the Sphinx base, scene coords
    // Scene is ~1920x1080; Sphinx base is near center ~x:780-980, y:530-680
    treasureRect = QRect(530, 565, 200, 110);
}

/* ================================================================
   LOAD SCENE
   ================================================================ */

void Level3::loadScene(QGraphicsScene* scene)
{
    //================ BACKGROUND =================//

    QPixmap bg(":/new/prefix1/images/level3.png");

    background = scene->addPixmap(bg);

    scene->setSceneRect(bg.rect());

    //================ TREASURE =================//
    // Place chest visually at the Sphinx base center

    treasure = scene->addPixmap(closedTreasure);

    // Position the chest sprite at the trigger zone top-left
    treasure->setPos(treasureRect.x(), treasureRect.y());

    // Scale: source PNG is ~1536px wide, we want ~130px wide on screen
    treasure->setScale(0.15);

    treasure->setZValue(900);
}

/* ================================================================
   TREASURE COLLISION
   ================================================================ */

void Level3::checkTreasureCollision(QGraphicsPixmapItem* playerSprite)
{
    if(passwordSolved)  return;
    if(!playerSprite || !treasure) return;

    QRectF playerRect  = playerSprite->sceneBoundingRect();

    // Use explicit QRect trigger zone — reliable regardless of sprite scale
    QRectF triggerZone = QRectF(treasureRect);

    if(playerRect.intersects(triggerZone) && !popupShown)
    {
        popupShown = true;
        showPasswordPopup();
    }
}

/* ================================================================
   PASSWORD POPUP  — styled full-screen overlay
   ================================================================ */

void Level3::showPasswordPopup()
{
    //================ FULL-SCREEN OVERLAY =================//

    QWidget* overlay = new QWidget();

    QRect screen = QApplication::primaryScreen()->geometry();

    overlay->setGeometry(screen);

    overlay->setStyleSheet(
        "background-color:rgba(0,0,0,210);"
        );

    overlay->show();

    overlay->raise();

    //================ CARD =================//

    QWidget* card = new QWidget(overlay);

    card->setFixedSize(640, 440);

    card->move(
        (overlay->width()  - 640) / 2,
        (overlay->height() - 440) / 2
        );

    card->setStyleSheet(
        "background-color:rgba(8,4,0,245);"
        "border:2px solid #c8a84b;"
        "border-radius:16px;"
        );

    //================ LAYOUT =================//

    QVBoxLayout* layout = new QVBoxLayout(card);

    layout->setContentsMargins(50, 36, 50, 36);

    layout->setSpacing(16);

    //================ ICON ROW =================//

    QLabel* icon = new QLabel("🗿");

    icon->setAlignment(Qt::AlignCenter);

    icon->setStyleSheet(
        "font-size:48px;"
        "background:transparent;"
        "border:none;"
        );

    //================ TITLE =================//

    QLabel* title = new QLabel("THE SPHINX SPEAKS");

    title->setAlignment(Qt::AlignCenter);

    title->setStyleSheet(
        "font-size:26px;"
        "font-weight:bold;"
        "color:#f5d060;"
        "letter-spacing:5px;"
        "background:transparent;"
        "border:none;"
        );

    //================ HINT TEXT =================//

    QLabel* hint = new QLabel(
        "The Great Sphinx guards this treasure.\n"
        "Answer the ancient riddle to claim what was lost:\n\n"
        "\"What walks on four legs in the morning,\n"
        " two legs at noon, and three in the evening?\"\n\n"
        "Enter the 3-letter answer:"
        );

    hint->setAlignment(Qt::AlignCenter);

    hint->setWordWrap(true);

    hint->setStyleSheet(
        "font-size:14px;"
        "color:#e8d5a8;"
        "background:transparent;"
        "border:none;"
        "line-height:1.6;"
        );

    //================ INPUT =================//

    QLineEdit* input = new QLineEdit();

    input->setFixedHeight(48);

    input->setAlignment(Qt::AlignCenter);

    input->setPlaceholderText("Type your answer...");

    input->setStyleSheet(
        "QLineEdit {"
        "  background-color:rgba(255,255,255,12);"
        "  border:2px solid rgba(200,160,60,160);"
        "  border-radius:8px;"
        "  color:#f5e6c8;"
        "  font-size:20px;"
        "  font-weight:bold;"
        "  letter-spacing:8px;"
        "}"
        "QLineEdit:focus {"
        "  border:2px solid #f5d060;"
        "  background-color:rgba(255,255,255,20);"
        "}"
        );

    //================ ERROR LABEL =================//

    QLabel* errorLabel = new QLabel("");

    errorLabel->setAlignment(Qt::AlignCenter);

    errorLabel->setStyleSheet(
        "font-size:13px;"
        "color:#ff7070;"
        "background:transparent;"
        "border:none;"
        );

    //================ SUBMIT BUTTON =================//

    QPushButton* submitBtn = new QPushButton("▶   UNLOCK THE TREASURE");

    submitBtn->setFixedHeight(52);

    submitBtn->setCursor(Qt::PointingHandCursor);

    submitBtn->setStyleSheet(
        "QPushButton {"
        "  background-color:rgba(180,130,40,220);"
        "  color:#fff8e7;"
        "  font-size:15px;"
        "  font-weight:bold;"
        "  letter-spacing:3px;"
        "  border:2px solid #c8a84b;"
        "  border-radius:8px;"
        "}"
        "QPushButton:hover {"
        "  background-color:rgba(220,170,60,240);"
        "  border:2px solid white;"
        "}"
        "QPushButton:pressed {"
        "  background-color:rgba(130,90,20,255);"
        "}"
        );

    //================ ASSEMBLE =================//

    layout->addWidget(icon);
    layout->addWidget(title);
    layout->addWidget(hint);
    layout->addWidget(input);
    layout->addWidget(errorLabel);
    layout->addStretch();
    layout->addWidget(submitBtn);

    card->show();

    input->setFocus();

    //================ SUBMIT LOGIC =================//

    QObject::connect(
        submitBtn,
        &QPushButton::clicked,
        [=]()
        {
            QString answer = input->text().trimmed().toUpper();

            if(answer == "MAN")
            {
                passwordSolved = true;
                treasureOpened = true;

                if(treasure)
                    treasure->setPixmap(openTreasure);

                overlay->deleteLater();

                showVictoryPopup();
            }
            else
            {
                errorLabel->setText(
                    "✗   Wrong answer — the treasure remains sealed."
                    );

                input->clear();

                input->setFocus();

                // Allow player to walk back and try again
                popupShown = false;
            }
        }
        );

    // Submit on Enter key
    QObject::connect(
        input,
        &QLineEdit::returnPressed,
        submitBtn,
        &QPushButton::click
        );
}

/* ================================================================
   VICTORY POPUP — full-screen, styled, includes Sphinx nose story
   ================================================================ */

void Level3::showVictoryPopup()
{
    //================ FULL-SCREEN OVERLAY =================//

    QWidget* overlay = new QWidget();

    QRect screen = QApplication::primaryScreen()->geometry();

    overlay->setGeometry(screen);

    overlay->setStyleSheet(
        "background-color:rgba(0,0,0,230);"
        );

    overlay->show();

    overlay->raise();

    //================ CARD =================//

    QWidget* card = new QWidget(overlay);

    card->setFixedSize(860, 640);

    card->move(
        (overlay->width()  - 860) / 2,
        (overlay->height() - 640) / 2
        );

    card->setStyleSheet(
        "background-color:rgba(8,4,0,250);"
        "border:3px solid #c8a84b;"
        "border-radius:18px;"
        );

    //================ LAYOUT =================//

    QVBoxLayout* layout = new QVBoxLayout(card);

    layout->setContentsMargins(50, 28, 50, 28);

    layout->setSpacing(12);

    //================ TOP BADGE =================//

    QLabel* badge = new QLabel("🏺   ARTIFACT RECOVERED   🏺");

    badge->setAlignment(Qt::AlignCenter);

    badge->setStyleSheet(
        "font-size:13px;"
        "font-weight:bold;"
        "color:#c8a84b;"
        "letter-spacing:5px;"
        "background:rgba(180,130,40,30);"
        "border:1px solid rgba(200,160,60,100);"
        "border-radius:6px;"
        "padding:6px 20px;"
        );

    //================ TITLE =================//

    QLabel* title = new QLabel("HERITAGE PROTECTED");

    title->setAlignment(Qt::AlignCenter);

    title->setStyleSheet(
        "font-size:38px;"
        "font-weight:bold;"
        "color:#f5d060;"
        "letter-spacing:7px;"
        "background:transparent;"
        "border:none;"
        );

    //================ TREASURE IMAGE =================//

    QLabel* img = new QLabel();

    img->setPixmap(
        QPixmap(":/new/prefix1/images/Treasureopened.png")
            .scaled(
                200, 200,
                Qt::KeepAspectRatio,
                Qt::SmoothTransformation
                )
        );

    img->setAlignment(Qt::AlignCenter);

    img->setStyleSheet(
        "background:transparent;"
        "border:none;"
        );

    //================ STORY TEXT =================//

    QLabel* story = new QLabel(
        "Deep inside the treasure chest, beneath layers of golden coins\n"
        "and ancient jewels, you discovered the impossible —\n\n"
        "<span style='font-size:18px;'>🗿</span>  "
        "<b><span style='color:#f5d060;'>The Lost Nose of the Great Sphinx</span></b>  "
        "<span style='font-size:18px;'>🗿</span>\n\n"
        "Broken off and hidden for centuries, its return will restore\n"
        "the Sphinx to its original glory. The museum's greatest\n"
        "mystery has finally been solved.\n\n"
        "<i><span style='color:rgba(200,160,60,200);'>Tonight, history survives because of you.</span></i>"
        );

    story->setAlignment(Qt::AlignCenter);

    story->setWordWrap(true);

    story->setTextFormat(Qt::RichText);

    story->setStyleSheet(
        "font-size:13px;"
        "color:#e8d5a8;"
        "background:transparent;"
        "border:none;"
        "line-height:1.7;"
        );

    //================ DIVIDER =================//

    QFrame* line = new QFrame();

    line->setFrameShape(QFrame::HLine);

    line->setFixedHeight(1);

    line->setStyleSheet(
        "background-color:rgba(200,160,60,60);"
        "border:none;"
        );

    //================ BUTTON STYLE =================//

    QString btnBase =
        "QPushButton {"
        "  font-size:15px;"
        "  font-weight:bold;"
        "  letter-spacing:3px;"
        "  border-radius:10px;"
        "  padding:14px;"
        "  color:#fff8e7;"
        "}"
        "QPushButton:hover { border-color:white; color:white; }"
        "QPushButton:pressed { background-color:rgba(100,70,10,255); }";

    //================ PLAY AGAIN BUTTON =================//

    QPushButton* playAgain = new QPushButton("▶   PLAY AGAIN FROM THE START");

    playAgain->setFixedHeight(54);

    playAgain->setCursor(Qt::PointingHandCursor);

    playAgain->setStyleSheet(
        btnBase +
        "QPushButton { background-color:rgba(180,130,40,220); border:2px solid #c8a84b; }"
        "QPushButton:hover { background-color:rgba(220,170,60,240); }"
        );

    //================ EXIT BUTTON =================//

    QPushButton* exitBtn = new QPushButton("✕   EXIT GAME");

    exitBtn->setFixedHeight(54);

    exitBtn->setCursor(Qt::PointingHandCursor);

    exitBtn->setStyleSheet(
        btnBase +
        "QPushButton { background-color:rgba(80,20,10,220); border:2px solid #8a3020; }"
        "QPushButton:hover { background-color:rgba(140,40,20,240); }"
        );

    //================ BUTTON ROW =================//

    QHBoxLayout* btnRow = new QHBoxLayout();

    btnRow->setSpacing(20);

    btnRow->addWidget(playAgain);

    btnRow->addWidget(exitBtn);

    //================ ASSEMBLE =================//

    layout->addWidget(badge,  0, Qt::AlignCenter);
    layout->addWidget(title);
    layout->addWidget(img);
    layout->addWidget(story);
    layout->addWidget(line);
    layout->addStretch();
    layout->addLayout(btnRow);

    card->show();

    //================ PLAY AGAIN — restart from Level 1 =================//

    QObject::connect(
        playAgain,
        &QPushButton::clicked,
        [=]()
        {
            overlay->deleteLater();

            if(restartCallback)
                restartCallback();
        }
        );

    //================ EXIT =================//

    QObject::connect(
        exitBtn,
        &QPushButton::clicked,
        [=]()
        {
            QApplication::quit();
        }
        );
}
