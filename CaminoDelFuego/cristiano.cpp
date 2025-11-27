#include "Cristiano.h"

Cristiano::Cristiano(QGraphicsItem* parent)
    : NPC(Faccion::CRISTIANO, parent),
    m_convertidoRecientemente(false),
    m_fuerzaOracion(5)
{
    m_nombre = "Cristiano";
    m_vida = 50;
}

Cristiano::~Cristiano() = default;

void Cristiano::actualizar(float /*dt*/) {
    // Comportamiento: ayudar, orar, ofrecer refugio
}

void Cristiano::interactuar(Entidad* otro) {
    Q_UNUSED(otro);
    // diálogo o curación
}

void Cristiano::orarPor(Entidad* objetivo) {
    Q_UNUSED(objetivo);
    // emitir bendición
}

void Cristiano::curarAliado(Entidad* objetivo, int cantidad) {
    if (!objetivo) return;
    objetivo->curar(cantidad);
}
