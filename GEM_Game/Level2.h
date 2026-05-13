#ifndef LEVEL2_H
#define LEVEL2_H
#include "Level.h"
#include <QGraphicsEllipseItem>
#include <QGraphicsRectItem>

class Level2 : public Level
{
private:

    //================ DARKNESS =================//

    QGraphicsRectItem* overlay;

    //================ PLAYER LIGHT =================//

    QGraphicsEllipseItem* playerGlow;

public:

    Level2();

    //================ LOAD LEVEL =================//

    void loadScene(
        QGraphicsScene* scene
        ) override;

    //================ ADD ROCK =================//

    void addRock(
        QGraphicsScene* scene,
        const QString& imagePath,
        int x,
        int y,
        double scale
        );

    //================ UPDATE LIGHT =================//

    void updatePlayerGlow(
        QGraphicsPixmapItem* playerSprite
        );
};

#include "level.h"
#include <QGraphicsEllipseItem>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QString>
#include <vector>

/* ================================================================
   Level2 — The Dark Hieroglyph Room
   ================================================================
   The player must collect 3 stone rocks each bearing the Egyptian
   hieroglyph for MAN (𓀀) before the timer runs out.

   Visual design:
   • Room is pitch-black — a large dark overlay covers the scene.
   • A soft ambient glow follows the player (his own torch).
   • The wandering wraith carries its own glow (see Level2Enemy).
   • Once all 3 rocks are collected a golden light floods the exit
     door so the player knows where to walk to finish the level.
   ================================================================ */

class Level2 : public Level
{
public:
    Level2();

    void loadScene(QGraphicsScene* scene) override;

    // Call each frame with the player sprite so the player's
    // torch glow tracks them correctly.
    void updatePlayerGlow(QGraphicsPixmapItem* playerSprite);

    // Lights up the exit door — called by GameWindow when all
    // rocks have been collected.
    void illuminateDoor();

    // Returns the scene-position of the door centre (used for
    // collision detection in GameWindow).
    QPointF doorPosition() const;

    // Expose the dark overlay so GameWindow can remove it on
    // level completion if desired.
    QGraphicsRectItem* darkOverlay() const { return overlay; }

private:
    QGraphicsRectItem*   overlay;       // full-scene black overlay
    QGraphicsEllipseItem* playerGlow;   // soft circle around the player
    QGraphicsPixmapItem*  doorItem;     // the exit door sprite / rect
    QGraphicsRectItem*    doorLight;    // golden rectangle that appears on win

    void addRock(QGraphicsScene* scene,
                 const QString& imagePath,
                 int x, int y, double scale);
};

#endif // LEVEL2_H
