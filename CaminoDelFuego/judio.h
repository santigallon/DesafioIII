#ifndef JUDIO_H
#define JUDIO_H

#include "NPC.h"

class Judio : public NPC {
public:
    Judio(QGraphicsItem* parent = nullptr);
    ~Judio() override;

    void actualizar(float dt) override;
    void interactuar(Entidad* otro) override;
    void discutirEscritura(Entidad* quien);
};

#endif
