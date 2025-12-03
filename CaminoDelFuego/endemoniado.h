#ifndef ENDEMONIADO_H
#define ENDEMONIADO_H

#include "NPC.h"

class Endemoniado : public NPC {
    Q_OBJECT
public:
    Endemoniado(QGraphicsItem* parent = nullptr);

    void actualizar(float dt) override;

    void interactuar(Entidad* otro) override;

    void expulsarDemonio();
    void setPlayerTarget(NPC* p) { m_playerTarget = p; }

protected:
    int m_nivelPosesion;
    bool m_exorcizado;
    float m_tiempoErratico;

    NPC* m_playerTarget = nullptr;
};

#endif
