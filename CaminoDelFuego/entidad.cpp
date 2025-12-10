#include "Entidad.h"
#include "player.h"
#include <QLineF>
#include <cmath>

Entidad::Entidad(QGraphicsItem* parent)
    : QGraphicsPixmapItem(parent),
    m_nombre("Entidad"),
    m_vida(100),
    m_vivo(true)
{
}

Entidad::~Entidad() = default;

QString Entidad::nombre() const { return m_nombre; }
void Entidad::setNombre(const QString& n) { m_nombre = n; }

QPointF Entidad::posicion() const { return this->pos(); }
void Entidad::setPosicion(const QPointF& p) { setPos(p); }

int Entidad::vida() const { return m_vida; }
void Entidad::setVida(int v) {
    m_vida = v;
    emit vidaCambiada(m_vida);
    if (m_vida <= 0 && m_vivo) {
        m_vivo = false;
        emit murio(this);
    }
}

bool Entidad::estaVivo() const { return m_vivo; }

void Entidad::actualizar(float /*dt*/) {
    // Lógica base vacía. Sobrescribir en subclases.
}

void Entidad::interactuar(Entidad* /*otro*/) {
    // Interacción genérica: sobrescribir.
}

void Entidad::recibirDanio(int cantidad, const QString& /*razon*/) {
    setVida(m_vida - cantidad);
}

void Entidad::curar(int cantidad) {
    if (!m_vivo) return;
    setVida(m_vida + cantidad);
}

bool Entidad::esJugador(Entidad* e) {
    return dynamic_cast<Player*>(e) != nullptr;
}

float Entidad::distanciaA(Entidad* e) const {
    return QLineF(pos(), e->pos()).length();
}

void Entidad::moverPor(float dx, float dy) {
    setPos(x() + dx, y() + dy);
}

void Entidad::moverHacia(const QPointF& d, float vel) {
    QPointF v = d - pos();
    float dist = std::sqrt(v.x()*v.x() + v.y()*v.y());
    if (dist < 1) return;

    QPointF dir = v / dist;
    setPos(pos() + dir * vel);
}

void Entidad::emitirDialogo(const QString& texto) {
    emit dialogo(texto); // agrega signal dialogo(QString)
}

void Entidad::emitirEfectoLuz(const QColor& color) {
    emit efectoLuz(color); // agrega signal
}

void Entidad::modificarVida(int delta) {
    m_vida += delta;
    if (m_vida < 0) m_vida = 0;
}
void Entidad::moverAleatorio(float velocidad) {
    float dx = (rand() % 3 - 1) * velocidad;
    float dy = (rand() % 3 - 1) * velocidad;
    moverPor(dx, dy);
}

void Entidad::setSprite(const QPixmap& px, bool alignFeet, int tileHeight) {
    setPixmap(px);
    // Ajuste sencillo: centramos horizontalmente; si alignFeet y tenemos tileHeight,
    // colocamos el pixmap de modo que la "base" quede en la parte inferior del tile.
    int w = px.width();
    int h = px.height();
    // offset.x: centrado horizontal (posiciones en QGraphicsPixmapItem usan setOffset)
    qreal offsetX = -w / 2.0;
    qreal offsetY = 0;
    if (alignFeet && tileHeight > 0) {
        // colocamos al pie: el bottom del sprite coincide con bottom del tile
        offsetY = - (h - tileHeight);
    } else {
        // centrado vertical aproximado
        offsetY = -h / 2.0;
    }
    setOffset(offsetX, offsetY);
}
