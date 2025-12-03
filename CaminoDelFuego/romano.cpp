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
    // patrullar base
}

void Romano::interactuar(Entidad* otro) {
    if (esJugador(otro)) {
        emitirDialogo("¡Alto! Identifícate.");
    }
}

void Romano::perseguirJugador(Entidad* jugador) {
    if (!jugador) return;
    moverHacia(jugador->posicion(), 50.0f);
}

void Romano::detenerPersecucion() {
    moverPor(0, 0);
}
