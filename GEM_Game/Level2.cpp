#include "Level2.h"
#include <QBrush>
#include <QColor>
#include <QPen>
#include <QPixmap>
#include <QFont>
#include <QRadialGradient>

/* ================================================================
   Level2 — The Hieroglyph Vault
   Design goals:
   - Pitch black room with a TIGHT torch radius (creates real fear)
   - Three rocks placed strategically: one near start, one mid-room,
     one in a far dark corner near the wraith's patrol area
   - Exit door far from start, across the room
   - Wall torches give faint ambient light (not enough to help much)
   - Door pulses gold when unlocked — player can navigate to it
   ================================================================ */

Level2::Level2()
    : overlay(nullptr)
    , playerGlow(nullptr)
    , playerGlowOuter(nullptr)
    , doorItem(nullptr)
    , doorLight(nullptr)
    , doorFrame(nullptr)
{
}

/* ================================================================
   LOAD SCENE
   ================================================================ */

void Level2::loadScene(QGraphicsScene* scene)
{
    // ── Background ──────────────────────────────────────────────
    QPixmap bg(":/new/prefix1/images/Level2.jpeg");
    background = scene->addPixmap(bg);
    background->setZValue(0);
    scene->setSceneRect(bg.rect());

    // ── Wall torches — faint amber glow fixed to walls ──────────
    // These give just enough ambient light that the room doesn't
    // feel completely hopeless, but not enough to see rocks from far
    addWallTorch(scene,  60,  80);    // top-left corner
    addWallTorch(scene, 680,  60);    // top-center
    addWallTorch(scene,1300,  80);    // top-right corner
    addWallTorch(scene,  60, 580);    // bottom-left
    addWallTorch(scene,1300, 580);    // bottom-right

    // ── Sacred Stones — placed with intent ──────────────────────
    //
    // STONE 1 (M) — near the start position (200,450)
    //   Easy to find first, builds confidence
    //
    // STONE 2 (A) — mid-room, slightly dark
    //   Moderate risk, forces player to move away from start
    //
    // STONE 3 (N) — far top-right corner, deep in darkness
    //   Near the wraith's starting position — most dangerous
    //
    addRock(scene, ":/new/prefix1/images/rock_M.png",   340,  480, 0.22);
    addRock(scene, ":/new/prefix1/images/rock_A-3.png", 750,  320, 0.22);
    addRock(scene, ":/new/prefix1/images/rock_N.png",  1080,  160, 0.22);

    // ── Exit Door — far right wall, locked until all rocks found ─
    // Faint outline visible in the dark so player knows it exists

    // Door frame outline — always visible (faint)
    doorFrame = scene->addRect(
        1260, 300, 90, 180,
        QPen(QColor(120, 90, 20, 80), 2),
        QBrush(QColor(40, 25, 5, 60))
        );
    doorFrame->setZValue(796);   // just above overlay

    // Door label — tiny text, barely readable in the dark
    QGraphicsTextItem* doorHint = scene->addText("EXIT");
    doorHint->setDefaultTextColor(QColor(180, 140, 40, 50));
    QFont f; f.setPixelSize(11); f.setLetterSpacing(QFont::AbsoluteSpacing, 3);
    doorHint->setFont(f);
    doorHint->setPos(1272, 385);
    doorHint->setZValue(797);

    // Door glow — starts invisible, illuminated when all rocks found
    doorLight = scene->addRect(
        1250, 290, 110, 200,
        QPen(Qt::NoPen),
        QBrush(QColor(255, 200, 50, 0))   // alpha=0 → invisible
        );
    doorLight->setZValue(9);

    // Invisible collision item for the door
    doorItem = scene->addPixmap(QPixmap());
    doorItem->setPos(1260, 300);
    doorItem->setZValue(10);

    // ── Player torch glow — TWO layers for realism ──────────────
    //
    // Inner glow: bright warm circle — this is the "torch light"
    //   Small enough that only nearby rocks are visible
    //
    // Outer glow: large soft fade — the "ambient spill"
    //   Very transparent — just shows the room edges dimly
    //
    playerGlowOuter = scene->addEllipse(
        -280, -280, 560, 560,
        QPen(Qt::NoPen),
        QBrush(QColor(255, 180, 80, 18))   // very transparent outer halo
        );
    playerGlowOuter->setZValue(796);   // just above dark overlay

    playerGlow = scene->addEllipse(
        -130, -130, 260, 260,
        QPen(Qt::NoPen),
        QBrush(QColor(255, 210, 120, 110))  // warm bright inner torch
        );
    playerGlow->setZValue(803);   // above wraith glow (800), below rocks (900)

    // ── Full-room pitch-black overlay ───────────────────────────
    // Z=795: below player glow (796+), below rocks (900)
    // Making this darker than before — 210 alpha vs 180 before
    // This is what makes the room actually scary
    overlay = scene->addRect(
        scene->sceneRect(),
        QPen(Qt::NoPen),
        QBrush(QColor(0, 0, 0, 210))
        );
    overlay->setZValue(795);
}

/* ================================================================
   ADD ROCK
   ================================================================ */

void Level2::addRock(QGraphicsScene* scene,
                     const QString&  imagePath,
                     int x, int y, double scale)
{
    QGraphicsPixmapItem* item = scene->addPixmap(QPixmap(imagePath));
    item->setPos(x, y);
    item->setScale(scale);
    item->setZValue(900);   // above everything — visible when torch is near
    item->setData(0, "rock");
    artifacts.push_back(item);
}

/* ================================================================
   ADD WALL TORCH — small amber glow fixed to walls
   ================================================================ */

void Level2::addWallTorch(QGraphicsScene* scene, int x, int y)
{
    // Small fixed ambient glow — Z=796 so it shows through overlay
    // but is dim enough to not give away rock positions
    QGraphicsEllipseItem* glow = scene->addEllipse(
        x - 60, y - 60, 120, 120,
        QPen(Qt::NoPen),
        QBrush(QColor(255, 150, 40, 28))
        );
    glow->setZValue(796);
    candles.append(glow);

    // Tiny bright centre (the "flame")
    QGraphicsEllipseItem* flame = scene->addEllipse(
        x - 8, y - 8, 16, 16,
        QPen(Qt::NoPen),
        QBrush(QColor(255, 220, 100, 140))
        );
    flame->setZValue(797);
    candles.append(flame);
}

/* ================================================================
   ADD CANDLE
   ================================================================ */

void Level2::addCandle(QGraphicsScene* scene, int x, int y)
{
    QGraphicsEllipseItem* glow = scene->addEllipse(
        x - 40, y - 40, 80, 80,
        QPen(Qt::NoPen),
        QBrush(QColor(255, 160, 50, 22))
        );
    glow->setZValue(796);
    candles.append(glow);
}

/* ================================================================
   UPDATE PLAYER GLOW — call every tick
   ================================================================ */

void Level2::updatePlayerGlow(QGraphicsPixmapItem* playerSprite)
{
    if (!playerSprite) return;

    QPixmap px = playerSprite->pixmap();
    double  sc = playerSprite->scale();
    double  cx = playerSprite->x() + (px.width()  * sc) / 2.0;
    double  cy = playerSprite->y() + (px.height() * sc) / 2.0;

    if (playerGlow)      playerGlow->setPos(cx, cy);
    if (playerGlowOuter) playerGlowOuter->setPos(cx, cy);
}

/* ================================================================
   ILLUMINATE DOOR — called when all 3 rocks collected
   The door bursts into golden light — player can now see it
   and navigate toward it to finish the level.
   ================================================================ */

void Level2::illuminateDoor()
{
    if (!doorLight) return;

    // Make door glow bright gold — very visible even in the dark
    doorLight->setBrush(QBrush(QColor(255, 200, 50, 180)));
    doorLight->setPen(QPen(QColor(255, 220, 100, 200), 3));
    doorLight->setRect(1240, 285, 130, 215);
    doorLight->setZValue(798);   // above overlay, below rocks

    // Make door frame more visible too
    if (doorFrame)
    {
        doorFrame->setPen(QPen(QColor(255, 200, 50, 220), 3));
        doorFrame->setBrush(QBrush(QColor(120, 90, 20, 100)));
    }
}

/* ================================================================
   DOOR POSITION
   ================================================================ */

QPointF Level2::doorPosition() const
{
    return QPointF(1305, 390);
}
