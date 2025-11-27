#include "Camara2D.h"
#include <cmath>

Camara2D::Camara2D(QObject* parent)
    : QObject(parent),
    m_camX(0),
    m_camY(0),
    m_tilesX(20),
    m_tilesY(15)
{
}

Camara2D::~Camara2D() = default;

void Camara2D::configurarPantalla(int anchoPixels, int altoPixels, int anchoTile, int altoTile) {
    m_tilesX = anchoPixels / anchoTile;
    m_tilesY = altoPixels / altoTile;
}

void Camara2D::centrarEn(float x, float y, float suavizado) {
    int newX = int(std::floor(x));
    int newY = int(std::floor(y));

    // movimiento suave estilo Fire Emblem / Advance Wars
    m_camX += (newX - m_camX) * suavizado;
    m_camY += (newY - m_camY) * suavizado;

    emit camaraMovida();
}
