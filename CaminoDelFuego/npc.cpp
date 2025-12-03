#include "NPC.h"

NPC::NPC(Faccion f, QGraphicsItem* parent)
    : Entidad(parent),
    m_faccion(f),
    m_nivelPecado(0),
    m_hostil(false)
{
    m_nombre = "NPC";
    m_vida = 50;
}

NPC::~NPC() = default;

Faccion NPC::faccion() const { return m_faccion; }
int NPC::nivelPecado() const { return m_nivelPecado; }
bool NPC::esHostil() const { return m_hostil; }
void NPC::setHostil(bool s) { m_hostil = s; }

void NPC::actualizar(float /*dt*/) {
    // comportamiento básico vacío
}

void NPC::interactuar(Entidad* /*otro*/) {
    // interacción genérica
}


void NPC::transformarA(Faccion nuevaFaccion) {
    m_faccion = nuevaFaccion;
    emit cambioDeFaccion(this, nuevaFaccion);
}
