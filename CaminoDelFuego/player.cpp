#include "Player.h"
#include <QDebug>
#include <QFile>
#include <QCoreApplication>
#include <QDir>
#include <cmath>

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
    m_controlable(true),
    m_frameIndex(0),
    m_animTimer(nullptr),
    m_animIntervalMs(150),
    m_centerOffset(true)
{
    m_nombre = "Pablo";
    m_vida = 100;
    m_vivo = true;

    // regeneración básica (ej. cada 2s)
    connect(&m_regenTimer, &QTimer::timeout, this, &Player::onRegenTick);
    m_regenTimer.start(2000);

    // Intento automático de cargar el sprite sheet en la ruta que diste.
    // Asumimos por defecto una sheet 4x4 (ajusta cols/rows si cambia).
    QString ruta = "C:/Users/USUARIO/DesafioIII/sprites/PabloSprite.png";
    const int defaultCols = 4;
    const int defaultRows = 4;

    // Intentos de rutas relativas/recursos (en orden)
    QStringList pruebas;
    pruebas << ruta;
    pruebas << QCoreApplication::applicationDirPath() + QDir::separator() + "sprites" + QDir::separator() + "PabloSprite.png";
    pruebas << QCoreApplication::applicationDirPath() + QDir::separator() + "assets" + QDir::separator() + "sprites" + QDir::separator() + "PabloSprite.png";
    pruebas << QString(":/sprites/PabloSprite.png");

    QString found;
    for (const QString &p : pruebas) {
        if (p.isEmpty()) continue;
        bool exists = QFile::exists(p);
        qDebug() << "[Player] prueba ruta:" << p << " exists=" << exists;
        if (exists) { found = p; break; }
    }

    if (!found.isEmpty()) {
        // cargamos el QPixmap para determinar tamaño de celda
        QPixmap full(found);
        if (!full.isNull()) {
            int fw = full.width() / defaultCols;
            int fh = full.height() / defaultRows;
            if (fw > 0 && fh > 0) {
                if (cargarSpriteSheet(found, fw, fh, defaultCols, defaultRows, true)) {
                    qDebug() << "[Player] spritesheet cargado desde:" << found << " frames=" << m_frames.size();
                    setFrame(0);
                } else {
                    qWarning() << "[Player] fallo al cortar spritesheet desde:" << found;
                }
            } else {
                qWarning() << "[Player] dimensiones invalidas en spritesheet:" << full.size();
            }
        } else {
            qWarning() << "[Player] QPixmap nulo al intentar cargar:" << found;
        }
    } else {
        qWarning() << "[Player] no se encontró PabloSprite.png en rutas probadas. Se usará placeholder.";
        QPixmap placeholder(32,48);
        placeholder.fill(Qt::blue);
        setPixmap(placeholder);
        setOffset(-placeholder.width()/2, -placeholder.height());
    }
}

Player::~Player() {
    if (m_animTimer) {
        m_animTimer->stop();
        delete m_animTimer;
        m_animTimer = nullptr;
    }
}

bool Player::cargarSpriteSheet(const QString& ruta, int frameW, int frameH, int cols, int rows, bool centerOffset) {
    if (cols <= 0 || rows <= 0 || frameW <= 0 || frameH <= 0) return false;
    QPixmap full;
    if (!full.load(ruta)) {
        // intentar recurso
        if (!full.load(QString(":/%1").arg(ruta))) {
            qWarning() << "[Player] cargarSpriteSheet fallo load ruta:" << ruta;
            return false;
        }
    }
    m_frames.clear();
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            int sx = c * frameW;
            int sy = r * frameH;
            if (sx + frameW <= full.width() && sy + frameH <= full.height()) {
                m_frames.append(full.copy(sx, sy, frameW, frameH));
            }
        }
    }
    m_centerOffset = centerOffset;

    if (!m_frames.isEmpty()) {
        // Setear primer frame
        setPixmap(m_frames.first());
        if (m_centerOffset) setOffset(-pixmap().width()/2, -pixmap().height());
    }
    return !m_frames.isEmpty();
}

void Player::setFrame(int idx) {
    if (m_frames.isEmpty()) return;
    if (idx < 0) idx = 0;
    if (idx >= m_frames.size()) idx = m_frames.size()-1;
    m_frameIndex = idx;
    setPixmap(m_frames.at(m_frameIndex));
    if (m_centerOffset) setOffset(-pixmap().width()/2, -pixmap().height());
}

void Player::startAnimation(int intervalMs) {
    if (m_frames.size() <= 1) return;
    m_animIntervalMs = intervalMs;
    if (!m_animTimer) {
        m_animTimer = new QTimer(this);
        connect(m_animTimer, &QTimer::timeout, this, &Player::onAnimTick);
    }
    m_animTimer->start(m_animIntervalMs);
}

void Player::stopAnimation() {
    if (m_animTimer) m_animTimer->stop();
}

void Player::onAnimTick() {
    if (m_frames.isEmpty()) return;
    m_frameIndex = (m_frameIndex + 1) % m_frames.size();
    setPixmap(m_frames.at(m_frameIndex));
    if (m_centerOffset) setOffset(-pixmap().width()/2, -pixmap().height());
}

// --- resto de API (funciones anteriores adaptadas) ---

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
    float speed = 100.0f; // px/s base
    if (!dir.isNull()) {
        qreal len = std::hypot(dir.x(), dir.y());
        dir /= len;
        m_velocidad = dir * speed;
        // si quieres animación al moverte, activa:
        startAnimation(120);
    } else {
        m_velocidad = QPointF(0,0);
        stopAnimation();
        setFrame(0); // frame idle
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
    } else {
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

bool Player::feAlta(double umbral) const { return m_fe >= umbral; }
bool Player::estaCorrupto(double umbral) const { return m_pecado >= umbral; }

void Player::onRegenTick() {
    if (!m_vivo) return;
    setEnergia(m_energia + 1.0f);
    if (m_energia > 100.0f) m_energia = 100.0f;
    m_fe += 0.1;
    emit feCambiada(m_fe);
}

void Player::clampValores() {
    if (m_energia < 0) m_energia = 0;
    if (m_energia > 100.0f) m_energia = 100.0f;
    if (m_fe < 0) m_fe = 0;
    if (m_pecado < 0) m_pecado = 0;
}
