#include "Judio.h"

Judio::Judio(QGraphicsItem* parent)
    : NPC(Faccion::JUDIO, parent)
{
    m_nombre = "Judío";
    m_vida = 45;
}

Judio::~Judio() = default;

void Judio::actualizar(float /*dt*/) {
    // comportamiento base: tal vez conversar, patrullar el mercado, etc.
}

void Judio::interactuar(Entidad* otro) {
    Q_UNUSED(otro);
    // ejemplo: discutir con player
}

void Judio::discutirEscritura(Entidad* quien) {
    Q_UNUSED(quien);
    // Lógica de discusión (invocar diálogo)
}
