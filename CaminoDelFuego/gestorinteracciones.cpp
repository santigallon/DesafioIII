#include "GestorInteracciones.h"
#include "GestorSonido.h"
#include "GestorEventos.h"
#include "GestorBarrasEstado.h"
#include "Player.h"
#include "Entidad.h"

#include <QDebug>

GestorInteracciones::GestorInteracciones(QObject* parent)
    : QObject(parent),
    m_sonido(nullptr),
    m_eventos(nullptr),
    m_barras(nullptr)
{
    m_timerAccion.setSingleShot(true);
    connect(&m_timerAccion, &QTimer::timeout, this, &GestorInteracciones::onTimeoutAccion);
}

GestorInteracciones::~GestorInteracciones() = default;

void GestorInteracciones::setGestorSonido(GestorSonido* s) { m_sonido = s; }
void GestorInteracciones::setGestorEventos(GestorEventos* e) { m_eventos = e; }
void GestorInteracciones::setGestorBarras(GestorBarrasEstado* b) { m_barras = b; }

void GestorInteracciones::intentarExorcismo(Player* jugador, Entidad* objetivo) {
    Q_UNUSED(jugador);
    if (!objetivo) return;
    // Notificar inicio
    if (m_sonido) m_sonido->inicioExorcismo();
    emit empezarExorcismo(objetivo);

    // Simulación simple: arrancamos un timer que luego disparará resultado
    m_timerAccion.start(1500);
}

void GestorInteracciones::intentarOratoria(Player* jugador, Entidad* audiencia) {
    Q_UNUSED(jugador);
    if (!audiencia) return;
    if (m_sonido) m_sonido->inicioOratoria();
    emit empezarOratoria(audiencia);
    m_timerAccion.start(1200);
}

void GestorInteracciones::intentarSanacion(Player* jugador, Entidad* objetivo) {
    Q_UNUSED(jugador);
    if (!objetivo) return;
    if (m_sonido) m_sonido->inicioSanacion();
    emit empezarSanacion(objetivo);
    m_timerAccion.start(1000);
}

void GestorInteracciones::onTimeoutAccion() {
    // para demo: siempre exito temporal (la UI luego confirma)
    // En integración: la ventana UI decide y emite resultado que GestorInteracciones
    // escuchará y transformará en efectos sobre Entidad/Player.
    // Aquí emitimos señales neutras (rellenar según integración).
    qDebug() << "GestorInteracciones::onTimeoutAccion - acción simulada terminada";
}
