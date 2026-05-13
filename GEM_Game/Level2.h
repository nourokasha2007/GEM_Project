#ifndef LEVEL2_H
#define LEVEL2_H

#include "level.h"
#include <QGraphicsEllipseItem>
#include <QGraphicsRectItem>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QPointF>

/* ================================================================
   Level2 — The Dark Hieroglyph Room
   Player must collect 3 sacred stones (rocks) before the timer runs out.
   The room is dark — a torch glow follows the player.
   A wraith enemy chases the player throughout.
   Once all rocks are collected the exit is illuminated.
   ================================================================ */

class Level2 : public Level
{
public:
    Level2();

    void loadScene(QGraphicsScene* scene) override;

    // Moves the player torch glow — call every frame
    void updatePlayerGlow(QGraphicsPixmapItem* playerSprite);

    // Lights up the exit door when all rocks collected
    void illuminateDoor();

    // Returns scene centre-position of the exit door
    QPointF doorPosition() const;

    // Expose dark overlay (GameWindow may want to remove it)
    QGraphicsRectItem* darkOverlay() const { return overlay; }

private:
    QGraphicsRectItem*    overlay;      // full-scene black overlay
    QGraphicsEllipseItem* playerGlow;   // soft circle around player
    QGraphicsPixmapItem*  doorItem;     // exit door sprite/rect
    QGraphicsRectItem*    doorLight;    // golden glow on exit door

    void addRock(QGraphicsScene* scene,
                 const QString&  imagePath,
                 int x, int y, double scale);
};

#endif // LEVEL2_H
