#ifndef LEVEL2_H
#define LEVEL2_H

#include "level.h"
#include <QGraphicsEllipseItem>
#include <QGraphicsRectItem>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QPointF>
#include <QTimer>

/* ================================================================
   Level2 — The Hieroglyph Vault
   A pitch-black chamber. The player carries a torch — a small
   circle of light. Three sacred stones are hidden in the dark.
   A wraith hunts the player. The exit door pulses with golden
   light once all stones are collected.
   ================================================================ */

class Level2 : public Level
{
public:
    Level2();

    void loadScene(QGraphicsScene* scene) override;

    // Call every game tick to move the torch glow with the player
    void updatePlayerGlow(QGraphicsPixmapItem* playerSprite);

    // Lights up the exit door — call when all rocks collected
    void illuminateDoor();

    // Returns scene position of the exit door (for collision)
    QPointF doorPosition() const;

    QGraphicsRectItem* darkOverlay() const { return overlay; }

private:
    QGraphicsRectItem*    overlay;       // full-scene pitch-black overlay
    QGraphicsEllipseItem* playerGlow;    // tight torch circle around player
    QGraphicsEllipseItem* playerGlowOuter; // soft outer halo
    QGraphicsPixmapItem*  doorItem;
    QGraphicsRectItem*    doorLight;
    QGraphicsRectItem*    doorFrame;

    // Ambient candle flicker items (decorative)
    QList<QGraphicsEllipseItem*> candles;

    void addRock(QGraphicsScene* scene,
                 const QString& imagePath,
                 int x, int y, double scale);

    void addCandle(QGraphicsScene* scene, int x, int y);
    void addWallTorch(QGraphicsScene* scene, int x, int y);
};

#endif // LEVEL2_H
