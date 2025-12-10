#include "GestorBarrasEstado.h"
#include "Entidad.h"
#include "Player.h"

GestorBarrasEstado::GestorBarrasEstado(QObject* parent)
    : QObject(parent),
    m_objetivo(nullptr),
    m_vida(0),
    m_energia(0.0f),
    m_fe(0.0),
    m_pecado(0.0)
{
}

GestorBarrasEstado::~GestorBarrasEstado() = default;

void GestorBarrasEstado::setObjetivo(Entidad* objetivo) {
    m_objetivo = objetivo;
    if (!m_objetivo) return;
    m_vida = m_objetivo->vida();
    // Si objetivo es Player, obtener más datos (bajo acoplamiento se podrían usar señales)
    Player* p = dynamic_cast<Player*>(m_objetivo);
    if (p) {
        m_energia = p->energia();
        m_fe = p->fe();
        m_pecado = p->pecado();
    }
}
void GestorBarrasEstado::actualizarEnergia(float valor) {
    m_energia = valor;

    if (barraEnergia) {
        barraEnergia->setRect(0, 0, valor, 10);
    }

    emit energiaCambiada(valor);
}

void GestorBarrasEstado::actualizarFe(double valor) {
    m_fe = valor;

    if (barraFe) {
        barraFe->setRect(0, 0, valor, 10);
    }

    emit feCambiada(valor);
}

void GestorBarrasEstado::establecerVida(int v) {
    m_vida = v;
    emit vidaCambiada(m_vida);
}

void GestorBarrasEstado::establecerEnergia(float e) {
    m_energia = e;
    emit energiaCambiada(m_energia);
}

void GestorBarrasEstado::establecerFe(double f) {
    m_fe = f;
    emit feCambiada(m_fe);
}

void GestorBarrasEstado::establecerPecado(double p) {
    m_pecado = p;
    emit pecadoCambiado(m_pecado);
}

int GestorBarrasEstado::vida() const { return m_vida; }
float GestorBarrasEstado::energia() const { return m_energia; }
double GestorBarrasEstado::fe() const { return m_fe; }
double GestorBarrasEstado::pecado() const { return m_pecado; }
