#include "Poseido.h"
#include <QDebug>

Poseido::Poseido(QGraphicsItem* parent)
    : Endemoniado(parent),
    m_nombreDemonio("Legión"),
    m_rabia(5)
{
    m_nombre = "Poseído";
    m_vida = 80;
    m_nivelPosesion = 8;
    m_hostil = true;
}

Poseido::~Poseido() = default;

void Poseido::actualizar(float /*dt*/) {
    // comportamiento avanzado: especial ataques, fases
}

void Poseido::interactuar(Entidad* otro) {
    Q_UNUSED(otro);
    // interacciones agresivas
}

void Poseido::desatarPoder() {
    // por ejemplo, aumenta daño o invoca entidades menores
    m_rabia += 2;
}

void Poseido::setNombreDemonio(const QString& nombre) {
    m_nombreDemonio = nombre;
}
