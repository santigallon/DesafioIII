#include "NPC.h"
#include <QDebug>

NPC::NPC(Faccion f, QGraphicsItem* parent)
    : Entidad(parent),
    m_faccion(f),
    m_nivelPecado(0),
    m_hostil(false)
{
    m_nombre = "NPC";
    m_vida = 50;

    // Opcional: asigna un pixmap placeholder (útil para probar visualmente)
    // QPixmap px(":/sprites/npc_placeholder.png");
    // if (!px.isNull()) setSprite(px, false);
}

NPC::~NPC() = default;

Faccion NPC::faccion() const { return m_faccion; }
int NPC::nivelPecado() const { return m_nivelPecado; }
bool NPC::esHostil() const { return m_hostil; }
void NPC::setHostil(bool s) { m_hostil = s; }

void NPC::actualizar(float /*dt*/) {
    // comportamiento básico vacío (sobrescribir en subclases)
}

void NPC::interactuar(Entidad* /*otro*/) {
    // interacción genérica (sobrescribir)
}

void NPC::transformarA(Faccion nuevaFaccion) {
    m_faccion = nuevaFaccion;
    emit cambioDeFaccion(this, nuevaFaccion);
}
