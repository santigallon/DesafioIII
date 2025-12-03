#include "Player.h"
#include <QDebug>

Player::Player(QGraphicsItem* parent)
    : Entidad(parent),
    m_energia(100.0f),
    m_velocidad(0,0),
    m_fe(10.0),
    m_pecado(0.0),
    m_ego(5.0),
    m_gracia(0.0),
    m_proyectiles(3),
    m_pergaminos(1),
    m_oracion(0),
    m_estadoActual("normal"),
    m_controlable(true)
{
    m_nombre = "Pablo";
    m_vida = 100;
    m_vivo = true;

    // regeneración básica (ej. cada 2s)
    connect(&m_regenTimer, &QTimer::timeout, this, &Player::onRegenTick);
    m_regenTimer.start(2000);
}

Player::~Player() = default;

float Player::energia() const { return m_energia; }

void Player::setEnergia(float e) {
    m_energia = e;
    emit energiaCambiada(m_energia);
}

double Player::fe() const { return m_fe; }
double Player::pecado() const { return m_pecado; }
double Player::ego() const { return m_ego; }
double Player::gracia() const { return m_gracia; }

int Player::proyectiles() const { return m_proyectiles; }
int Player::pergaminos() const { return m_pergaminos; }

QString Player::estadoActual() const { return m_estadoActual; }
bool Player::esControlable() const { return m_controlable; }

void Player::actualizar(float dt) {
    Q_UNUSED(dt);
    // Actualización ligera: aplicar velocidad a la posición
    if (!m_vivo) return;
    if (!m_velocidad.isNull()) {
        moveBy(m_velocidad.x() * dt, m_velocidad.y() * dt);
    }
    clampValores();
}

void Player::moverPorInput(bool up, bool down, bool left, bool right, float dt) {
    if (!m_controlable || !m_vivo) return;
    QPointF dir(0,0);
    if (up) dir.ry() -= 1;
    if (down) dir.ry() += 1;
    if (left) dir.rx() -= 1;
    if (right) dir.rx() += 1;
    // Normalizar y aplicar velocidad base
    float speed = 100.0f; // px/s base (ajustar)
    if (!dir.isNull()) {
        qreal len = std::hypot(dir.x(), dir.y());
        dir /= len;
        m_velocidad = dir * speed;
    } else {
        m_velocidad = QPointF(0,0);
    }
    actualizar(dt);
}

void Player::orar(int intensidad) {
    if (!m_vivo) return;
    m_oracion += intensidad;
    aumentarFe(0.5 * intensidad);
    m_gracia += 0.2 * intensidad;
    emit feCambiada(m_fe);
}

void Player::descansar(float segundos) {
    Q_UNUSED(segundos);
    setEnergia(m_energia + 10.0f);
}

void Player::interactuarCon(Entidad* e, bool ayudar) {
    if (!e) return;

    if (ayudar) {
        e->curar(5);
        emitirDialogo("Que el Señor te proteja.");
    }
    else {
        // Interacción base: charla o exorcismo según contexto
        emitirDialogo("¿Necesitas algo?");
    }
}

void Player::lanzarProyectil(const QPointF& dir) {
    Q_UNUSED(dir);
    if (m_proyectiles <= 0) return;

    m_proyectiles--;
    emit crearProyectil(posicion(), dir);
}

void Player::iniciarExorcismo(Entidad* objetivo) {
    if (!objetivo) return;
    emit pedirExorcismo(objetivo);
}

void Player::iniciarEscritura(const QString& texto) {
    emit abrirVentanaEscritura(texto);
}

void Player::iniciarOratoria(const QString& tema) {
    emit abrirVentanaOratoria(tema);
}

void Player::recibirProvidencia() {
    curar(20);
    aumentarFe(5.0);
}

void Player::aplicarDanio(int cantidad, const QString& razon) {
    Q_UNUSED(razon);
    recibirDanio(cantidad, razon);
    if (!m_vivo) emit murioJugador();
}

void Player::curarJugador(int cantidad) {
    curar(cantidad);
}

void Player::aumentarFe(double cantidad) {
    m_fe += cantidad;
    emit feCambiada(m_fe);
}

void Player::disminuirFe(double cantidad) {
    m_fe -= cantidad;
    if (m_fe < 0) m_fe = 0;
    emit feCambiada(m_fe);
}

void Player::agregarPecado(double cantidad) {
    m_pecado += cantidad;
    emit pecadoCambiado(m_pecado);
}

void Player::reducirPecado(double cantidad) {
    m_pecado -= cantidad;
    if (m_pecado < 0) m_pecado = 0;
    emit pecadoCambiado(m_pecado);
}

bool Player::feAlta(double umbral) const {
    return m_fe >= umbral;
}

bool Player::estaCorrupto(double umbral) const {
    return m_pecado >= umbral;
}

void Player::onRegenTick() {
    if (!m_vivo) return;
    // regeneración pasiva muy pequeña
    setEnergia(m_energia + 1.0f);
    if (m_energia > 100.0f) m_energia = 100.0f;
    // pequeña recuperación de fe por descanso pasivo
    m_fe += 0.1;
    emit feCambiada(m_fe);
}

void Player::clampValores() {
    if (m_energia < 0) m_energia = 0;
    if (m_energia > 100.0f) m_energia = 100.0f;
    if (m_fe < 0) m_fe = 0;
    if (m_pecado < 0) m_pecado = 0;
}




