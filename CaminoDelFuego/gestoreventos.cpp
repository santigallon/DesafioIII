#include "GestorEventos.h"
#include <QDebug>
#include <cstdlib>

GestorEventos::GestorEventos(QObject* parent)
    : QObject(parent),
    m_temporizador(0.0f),
    m_intervalo(10.0f) // cada 10s por defecto
{
}

GestorEventos::~GestorEventos() = default;

void GestorEventos::agregarEventoAleatorio(const std::function<void()>& evento) {
    m_eventos.push_back(evento);
}

void GestorEventos::actualizar(float dt) {
    if (m_eventos.empty()) return;
    m_temporizador += dt;
    if (m_temporizador >= m_intervalo) {
        // elegir evento aleatorio
        int idx = std::rand() % m_eventos.size();
        ejecutarEvento(idx);
        m_temporizador = 0.0f;
    }
}

void GestorEventos::ejecutarEvento(int indice) {
    if (indice < 0 || indice >= int(m_eventos.size())) return;
    m_eventos[indice]();
    emit eventoEjecutado(indice);
    qDebug() << "GestorEventos: ejecutado evento" << indice;
}

int GestorEventos::cantidadEventos() const {
    return int(m_eventos.size());
}
