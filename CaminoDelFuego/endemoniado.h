#ifndef ENDEMONIADO_H
#define ENDEMONIADO_H

#pragma once
#include "NPC.h"

/**
 * Endemoniado - NPC con influencia demoníaca.
 * Soporta nivel de posesión y exorcismo.
 */
class Endemoniado : public NPC {
    Q_OBJECT
public:
    explicit Endemoniado(QGraphicsItem* parent = nullptr);
    ~Endemoniado() override;

    void actualizar(float dt) override;
    void interactuar(Entidad* otro) override;

    void poseer(int nivel);
    void expulsarDemonio(); // intento de exorcismo

    int nivelPosesion() const;
    bool estaExorcizado() const;

protected:
    int m_nivelPosesion;
    bool m_exorcizado;
};

#endif // ENDEMONIADO_H
