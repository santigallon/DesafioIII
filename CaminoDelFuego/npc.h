#ifndef NPC_H
#define NPC_H
#include <entidad.h>
#include <QString>
#include <QPointF>

class NPC
{
protected:

    int sinLevel;
    bool hostile;

public:
    void interact();
    void update(float deltaTime);
    bool isHostile();
    void setHostile(bool state);
};

#endif // NPC_H
