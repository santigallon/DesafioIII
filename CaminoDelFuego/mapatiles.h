#ifndef MAPATILES_H
#define MAPATILES_H

#pragma once
#include <QObject>
#include <QGraphicsScene>
#include <QPixmap>
#include <vector>

/**
 * MapaTiles
 * - Carga un mapa basado en tiles (matriz bidimensional).
 * - Renderiza el mapa en un QGraphicsScene.
 * - Informa si un tile es sólido, decorativo o transitable.
 *
 * Funciona junto a:
 *   • Camara2D
 *   • GestorFisicas
 */

class MapaTiles : public QObject {
    Q_OBJECT
public:
    explicit MapaTiles(QGraphicsScene* escena, QObject* parent = nullptr);
    ~MapaTiles() override;

    bool cargarTileset(const QString& ruta, int anchoTile, int altoTile);
    void cargarMapa(const std::vector<std::vector<int>>& matrizTiles);

    void renderizar(int camX, int camY, int tilesX, int tilesY);

    int anchoTile() const { return m_anchoTile; }
    int altoTile() const { return m_altoTile; }

    bool esBloqueado(int x, int y) const;
    int tileID(int x, int y) const;

private:
    QGraphicsScene* m_escena;
    QPixmap m_tileset;
    int m_anchoTile;
    int m_altoTile;

    std::vector<std::vector<int>> m_mapa;
};
#endif // MAPATILES_H
