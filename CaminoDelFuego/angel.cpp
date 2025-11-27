#include "Angel.h"

Angel::Angel(QGraphicsItem* parent)
    : NPC(Faccion::PAGANO, parent), // no pertenece a facción humana; usamos PAGANO por defecto
    m_mensaje(""),
    m_visibleParaJugador(false)
{
    m_nombre = "Ángel";
    m_vida = 9999; // inmortal simbólico
}

Angel::~Angel() = default;

void Angel::actualizar(float /*dt*/) {
    // Flota, aparece en momentos divinos
}

void Angel::interactuar(Entidad* /*otro*/) {
    // Revela mensajes o guía
}

void Angel::revelarAlJugador() {
    m_visibleParaJugador = true;
    // activar eventos o señales
}

void Angel::enviarMensaje(const QString& mensaje) {
    m_mensaje = mensaje;
    // emitir señal o notificar sistema de diálogo
}
