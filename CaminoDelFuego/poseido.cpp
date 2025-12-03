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
    // comportamiento agresivo avanzado
    if (m_rabia > 5)
        moverAleatorio(35);
}

void Poseido::interactuar(Entidad* otro) {
    if (esJugador(otro))
        otro->modificarVida(-12);
}

void Poseido::desatarPoder() {
    m_rabia += 2;
    emitirEfectoLuz(Qt::red);
}

void Poseido::setNombreDemonio(const QString& nombre) {
    m_nombreDemonio = nombre;
}




