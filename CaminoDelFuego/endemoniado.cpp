#include "Endemoniado.h"

Endemoniado::Endemoniado(QGraphicsItem* parent)
    : NPC(Faccion::END, parent),
    m_nivelPosesion(1),
    m_exorcizado(false)
{
    m_nombre = "Endemoniado";
    m_vida = 40;
    m_hostil = true;
}

Endemoniado::~Endemoniado() = default;

void Endemoniado::actualizar(float /*dt*/) {
    // comportamiento demoníaco: ataques, maldiciones
}

void Endemoniado::interactuar(Entidad* otro) {
    Q_UNUSED(otro);
    // acciones de ataque o emitir voces
}

void Endemoniado::poseer(int nivel) {
    m_nivelPosesion = nivel;
    m_hostil = true;
}

void Endemoniado::expulsarDemonio() {
    // lógica básica: si nivel pequeño, se cura
    if (m_nivelPosesion <= 2) {
        m_exorcizado = true;
        m_hostil = false;
        transformarA(Faccion::CRISTIANO); // ejemplo: se convierte
    } else {
        // aún persistente
    }
}

int Endemoniado::nivelPosesion() const { return m_nivelPosesion; }
bool Endemoniado::estaExorcizado() const { return m_exorcizado; }
