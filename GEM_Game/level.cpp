#include "level.h"

QGraphicsItem* Level::checkArtifactCollision( QGraphicsPixmapItem* player )
{
    for(size_t i = 0; i < artifacts.size(); i++)
    {
        if(player->collidesWithItem(artifacts[i]))
        {
            return artifacts[i];
        }
    }

    return nullptr;
}
int Level::getArtifactCount() const
{
    return artifacts.size();
}
void Level::clearLevel(QGraphicsScene* scene)
{
    for(size_t i = 0; i < artifacts.size(); i++)
    {
        scene->removeItem(artifacts[i]);
    }

    for(size_t i = 0; i < obstacles.size(); i++)
    {
        scene->removeItem(obstacles[i]);
    }

    artifacts.clear();
}
/* ================= REMOVE ARTIFACT ================= */

void Level::removeArtifact(
    QGraphicsItem* artifact,
    QGraphicsScene* scene
    )
{
    scene->removeItem(artifact);

    for(size_t i = 0; i < artifacts.size(); i++)
    {
        if(artifacts[i] == artifact)
        {
            artifacts.erase(
                artifacts.begin() + i
                );

            break;
        }
    }
}
