#include "MapaTiles.h"
#include <QGraphicsPixmapItem>
#include <QRect>

MapaTiles::MapaTiles(QGraphicsScene* escena, QObject* parent)
    : QObject(parent),
    m_escena(escena),
    m_anchoTile(32),
    m_altoTile(32)
{
}

MapaTiles::~MapaTiles() = default;

bool MapaTiles::cargarTileset(const QString& ruta, int anchoTile, int altoTile) {
    m_tileset = QPixmap(ruta);
    if (m_tileset.isNull()) return false;
    m_anchoTile = anchoTile;
    m_altoTile = altoTile;
    return true;
}

void MapaTiles::cargarMapa(const std::vector<std::vector<int>>& matrizTiles) {
    m_mapa = matrizTiles;
}

void MapaTiles::renderizar(int camX, int camY, int tilesX, int tilesY) {
    if (m_mapa.empty()) return;

    m_escena->clear();

    for (int y = 0; y < tilesY; y++) {
        for (int x = 0; x < tilesX; x++) {

            int mapaX = camX + x;
            int mapaY = camY + y;

            if (mapaY >= 0 && mapaY < (int)m_mapa.size() &&
                mapaX >= 0 && mapaX < (int)m_mapa[mapaY].size()) {

                int id = m_mapa[mapaY][mapaX];

                int columnas = m_tileset.width() / m_anchoTile;
                int srcX = (id % columnas) * m_anchoTile;
                int srcY = (id / columnas) * m_altoTile;

                auto* item = new QGraphicsPixmapItem(
                    m_tileset.copy(srcX, srcY, m_anchoTile, m_altoTile)
                    );

                item->setPos(x * m_anchoTile, y * m_altoTile);
                m_escena->addItem(item);
            }
        }
    }
}

bool MapaTiles::esBloqueado(int x, int y) const {
    if (m_mapa.empty()) return false;
    if (y < 0 || y >= (int)m_mapa.size()) return true;
    if (x < 0 || x >= (int)m_mapa[y].size()) return true;

    return (m_mapa[y][x] == -1); // por ejemplo: -1 = muro
}

int MapaTiles::tileID(int x, int y) const {
    if (y < 0 || y >= (int)m_mapa.size()) return -1;
    if (x < 0 || x >= (int)m_mapa[y].size()) return -1;
    return m_mapa[y][x];
}
