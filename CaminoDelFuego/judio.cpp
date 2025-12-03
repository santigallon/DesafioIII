#include "Judio.h"

Judio::Judio(QGraphicsItem* parent)
    : NPC(Faccion::JUDIO, parent)
{}

Judio::~Judio() = default;

void Judio::actualizar(float /*dt*/) {
    // Simple patrulla o diálogo ocasional
}

void Judio::interactuar(Entidad* otro) {
    if (esJugador(otro)) {
        emitirDialogo("¿Has leído la Torá?");
        discutirEscritura(otro);
    }
}

void Judio::discutirEscritura(Entidad* quien) {
    Q_UNUSED(quien);
    emitirDialogo("La Escritura es clara, pero pocos la comprenden.");
}
