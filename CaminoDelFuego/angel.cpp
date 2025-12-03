#include "Angel.h"
#include "GestorSonido.h"

#include <cmath>

Angel::Angel(QGraphicsItem* parent)
    : Entidad(parent),
    m_visibleParaJugador(false),
    m_tiempo(0.0f),
    m_sonido(nullptr)
{
    m_posInicial = posicion();
    setVisible(false);
}

void Angel::actualizar(float dt) {
    // Flotación suave
    float dy = std::sin(m_tiempo * 2.2f) * 10.0f;
    setPosicion(QPointF(m_posInicial.x(), m_posInicial.y() + dy));

    m_tiempo += dt;

    if (m_visibleParaJugador) {
        emitirEfectoLuz(QColor(200, 220, 255));
    }
}

void Angel::interactuar(Entidad* otro) {
    if (!otro) return;
    if (esJugador(otro)) {
        revelarAlJugador();
        enviarMensaje("No temas. El Altísimo está contigo.");
    }
}

void Angel::revelarAlJugador() {
    m_visibleParaJugador = true;
    setVisible(true);

    if (m_sonido)
        m_sonido->reproducirEfecto("angel_aparece.wav");

    emitirEfectoLuz(Qt::white);
}

void Angel::enviarMensaje(const QString& mensaje) {
    m_mensaje = mensaje;
    emit mensajeDivino(mensaje);

    if (m_sonido)
        m_sonido->reproducirEfecto("susurro_divino.wav");
}
