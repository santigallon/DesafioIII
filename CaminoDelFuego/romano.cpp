#include "Romano.h"
#include <QDebug>

Romano::Romano(QGraphicsItem* parent)
    : NPC(Faccion::ROMANO, parent),
    m_nivelAutoridad(1)
{
    m_nombre = "Romano";
    m_vida = 70;
    m_hostil = true;
}

Romano::~Romano() = default;

void Romano::actualizar(float /*dt*/) {
    // patrulla o busca amenazas
}

void Romano::interactuar(Entidad* otro) {
    Q_UNUSED(otro);
    // acciones: arrestar, preguntar, perseguir
}

void Romano::perseguirJugador(Entidad* jugador) {
    Q_UNUSED(jugador);
    // lógica de persecución
}

void Romano::detenerPersecucion() {
    // parar
}
