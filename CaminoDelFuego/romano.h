#ifndef ROMANO_H
#define ROMANO_H

#pragma once
#include "NPC.h"

/**
 * Romano - guardias, centuriones, autoridades romanas
 */
class Romano : public NPC {
    Q_OBJECT
public:
    explicit Romano(QGraphicsItem* parent = nullptr);
    ~Romano() override;

    void actualizar(float dt) override;
    void interactuar(Entidad* otro) override;

    void perseguirJugador(Entidad* jugador);
    void detenerPersecucion();
private:
    int m_nivelAutoridad;
};
#endif // ROMANO_H


