#ifndef GESTORFISICAS_H
#define GESTORFISICAS_H

#pragma once
#include <QObject>
#include <QGraphicsScene>
#include <QTimer>
#include <QPointF>
#include <vector>

/**
 * GestorFisicas
 * - Administra parámetros ambientales: gravedad, humedad, viento, iluminación.
 * - Expone métodos para validar movimiento/colisión a nivel de tile o bounding-box.
 * - Emite señales cuando eventos divinos o cambios de ambiente ocurren.
 *
 * NOTA: Para colisiones con entidades, usa QRectF/posiciones desde Entidad.
 */

class GestorFisicas : public QObject {
    Q_OBJECT
public:
    explicit GestorFisicas(QGraphicsScene* scene, QObject* parent = nullptr);
    ~GestorFisicas() override;

    // ciclo de actualización; dt en segundos
    void actualizar(float dt);

    // parámetros ambientales
    void establecerGravedad(float g);
    void establecerHumedad(float h);
    void establecerNivelLuz(float l);
    void establecerViento(const QPointF& v);

    float gravedad() const { return m_gravedad; }
    float humedad() const { return m_humedad; }
    float nivelLuz() const { return m_nivelLuz; }
    QPointF viento() const { return m_viento; }

    // mapa de colisiones simple (0 = libre, 1 = bloqueado)
    void establecerMapaColisiones(const std::vector<std::vector<int>>& mapa);
    bool esCaminable(int tileX, int tileY) const;
    bool hayColisionRect(const QRectF& a, const QRectF& b) const;

    // eventos "mano del creador"
    void aplicarLuzDivina(float intensidad = 1.0f, float duracionSeg = 4.0f);
    void brisaSuave(float fuerza = 0.2f);
    void terremotoSagrado(float magnitud = 1.0f);

signals:
    void nivelLuzCambiado(float nuevoNivel);
    void humedadCambiada(float nuevoNivel);
    void eventoDivinoIniciado(const QString& nombreEvento);
    void eventoDivinoTerminado(const QString& nombreEvento);

private slots:
    void onTimeoutDivino();

private:
    QGraphicsScene* m_scene;
    float m_gravedad;
    float m_humedad;
    float m_nivelLuz;
    QPointF m_viento;

    // mapa de colisiones
    std::vector<std::vector<int>> m_mapaColisiones;

    // control eventos divinos
    bool m_eventoDivinoActivo;
    QString m_eventoDivinoActual;
    QTimer m_timerDivino;
};

#endif // GESTORFISICAS_H
