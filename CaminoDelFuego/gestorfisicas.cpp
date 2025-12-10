#include "GestorFisicas.h"
#include <QRectF>

GestorFisicas::GestorFisicas(QGraphicsScene* scene, QObject* parent)
    : QObject(parent),
    m_scene(scene),
    m_gravedad(9.8f),
    m_humedad(0.0f),
    m_nivelLuz(1.0f),
    m_viento(0,0),
    m_eventoDivinoActivo(false),
    m_eventoDivinoActual("")
{
    m_timerDivino.setSingleShot(true);
    connect(&m_timerDivino, &QTimer::timeout, this, &GestorFisicas::onTimeoutDivino);
}

GestorFisicas::~GestorFisicas() = default;

void GestorFisicas::actualizar(float /*dt*/) {

}

void GestorFisicas::establecerGravedad(float g) {
    m_gravedad = g;
}

void GestorFisicas::establecerHumedad(float h) {
    m_humedad = h;
    emit humedadCambiada(m_humedad);
}

void GestorFisicas::establecerNivelLuz(float l) {
    m_nivelLuz = l;
    emit nivelLuzCambiado(m_nivelLuz);
}

void GestorFisicas::establecerViento(const QPointF& v) {
    m_viento = v;
}

void GestorFisicas::establecerMapaColisiones(const std::vector<std::vector<int>>& mapa) {
    m_mapaColisiones = mapa;
}

bool GestorFisicas::esCaminable(int tileX, int tileY) const {
    if (tileX < 0 || tileY < 0) return false;
    if (m_mapaColisiones.empty()) return true;
    if (tileY >= int(m_mapaColisiones.size())) return false;
    if (tileX >= int(m_mapaColisiones[tileY].size())) return false;
    return m_mapaColisiones[tileY][tileX] == 0;
}

bool GestorFisicas::hayColisionRect(const QRectF& a, const QRectF& b) const {
    return a.intersects(b);
}

void GestorFisicas::aplicarLuzDivina(float intensidad, float duracionSeg) {
    if (m_eventoDivinoActivo) return;
    m_eventoDivinoActivo = true;
    m_eventoDivinoActual = "LuzDivina";
    // aumentar nivel de luz temporalmente
    //float antiguo = m_nivelLuz;nesecito esto para las continuaciones de juego
    establecerNivelLuz(std::min(2.0f, m_nivelLuz + intensidad));
    emit eventoDivinoIniciado(m_eventoDivinoActual);
    m_timerDivino.start(int(duracionSeg * 1000));
}

void GestorFisicas::brisaSuave(float fuerza) {
    // ajustar viento levemente
    m_viento += QPointF(fuerza, 0.0f);
    // no usamos timer; es efecto corto
}

/*lo nesecito para otra parte del juego aun no ha sido implementada
 * void GestorFisicas::terremotoSagrado(float magnitud) {
    if (m_eventoDivinoActivo) return;
    m_eventoDivinoActivo = true;
    m_eventoDivinoActual = "TerremotoSagrado";
    emit eventoDivinoIniciado(m_eventoDivinoActual);
    // En un sistema completo harías shake en la cámara y daño leve a objetos frágiles
    m_timerDivino.start(1500); // ejemplo 1.5s
}*/

void GestorFisicas::onTimeoutDivino() {
    // finalizar evento divino
    if (m_eventoDivinoActual == "LuzDivina") {
        // reducir luz a nivel base (para demo 1.0)
        establecerNivelLuz(1.0f);
    }
    emit eventoDivinoTerminado(m_eventoDivinoActual);
    m_eventoDivinoActual.clear();
    m_eventoDivinoActivo = false;
}
