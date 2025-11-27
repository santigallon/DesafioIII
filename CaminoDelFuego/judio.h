#ifndef JUDIO_H
#define JUDIO_H

#pragma once
#include "NPC.h"

/**
 * Judio - NPC de origen judío
 */
class Judio : public NPC {
    Q_OBJECT
public:
    explicit Judio(QGraphicsItem* parent = nullptr);
    ~Judio() override;

    void actualizar(float dt) override;
    void interactuar(Entidad* otro) override;

    void discutirEscritura(Entidad* quien);
};

#endif // ENTIDAD_H
