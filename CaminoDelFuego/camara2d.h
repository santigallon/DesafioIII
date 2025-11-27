#ifndef CAMARA2D_H
#define CAMARA2D_H

#pragma once
#include <QObject>

/**
 * Camara2D
 * - Mantiene la vista actual en el mapa.
 * - Calcula cuántos tiles mostrar en pantalla.
 * - Se mueve suavemente hacia el jugador.
 */

class Camara2D : public QObject {
    Q_OBJECT
public:
    explicit Camara2D(QObject* parent = nullptr);
    ~Camara2D() override;

    void configurarPantalla(int anchoPixels, int altoPixels, int anchoTile, int altoTile);

    void centrarEn(float x, float y, float suavizado = 0.1f);

    int camX() const { return m_camX; }
    int camY() const { return m_camY; }

    int tilesX() const { return m_tilesX; }
    int tilesY() const { return m_tilesY; }

signals:
    void camaraMovida();

private:
    int m_camX;
    int m_camY;

    int m_tilesX;
    int m_tilesY;
};

#endif // CAMARA2D_H
