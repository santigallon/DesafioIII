#include "Entidad.h"

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
