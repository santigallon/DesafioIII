#include "Endemoniado.h"
#include <cmath>

Endemoniado::Endemoniado(QGraphicsItem* parent)
    : NPC(Faccion::ENDEMONIADO, parent),
    m_nivelPosesion(3),
    m_exorcizado(false),
    m_tiempoErratico(0.0f)
{
    // Usar el m_hostil heredado de NPC
    m_hostil = true;
}

void Endemoniado::actualizar(float dt) {
    if (!m_hostil || m_exorcizado) return;

    if (m_playerTarget) {

        float dist = distanciaA(m_playerTarget);

        if (dist < 180) {
            moverHacia(m_playerTarget->posicion(), 45.0f);
        } else {
            // Patrullar erráticamente
            m_tiempoErratico += dt;
            float dx = std::sin(m_tiempoErratico * 1.9f) * 10;
            float dy = std::cos(m_tiempoErratico * 1.7f) * 10;
            moverPor(dx, dy);
        }

        if (dist < 20)
            m_playerTarget->modificarVida(-8);
    }
}

void Endemoniado::interactuar(Entidad* otro) {
    if (!otro) return;

    if (esJugador(otro)) {
        emitirDialogo("¡GRAAAAH!");
        otro->modificarVida(-10);
    }
}

void Endemoniado::expulsarDemonio() {
    if (m_nivelPosesion <= 2) {
        m_exorcizado = true;
        m_hostil = false;
        transformarA(Faccion::CRISTIANO);
        emitirDialogo("Estoy libre... gracias...");
    } else {
        emitirDialogo("¡NO SALDRÉ!");
        m_nivelPosesion -= 1;
    }
}
