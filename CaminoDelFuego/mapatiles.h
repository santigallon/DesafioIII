#ifndef MAPATILES_H
#define MAPATILES_H

#pragma once
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QObject>
#include <QGraphicsScene>
#include <QPixmap>
#include <QVector>
#include <QPoint>
#include <QString>
#include <QSize>
#include <QRect>
#include <QMap>
#include <QCoreApplication>
#include <QDir>

/**
 * MapaTiles
 * - Carga un mapa basado en tiles (Tiled JSON).
 * - Guarda capas (tile layers y object layers).
 * - Permite renderizar la región visible en una QGraphicsScene o con QPainter.
 */

struct Capa {
    QString nombre;
    int ancho = 0;
    int alto  = 0;
    QVector<int> datos;
    bool esCapaObjetos = false;
};

struct PuntoObjeto {
    QString nombre;
    QString tipo;
    int x = 0, y = 0;
    QJsonObject propiedades;
};

class MapaTiles : public QObject {
    Q_OBJECT
public:
    explicit MapaTiles(QGraphicsScene* escena, QObject* parent = nullptr);
    ~MapaTiles() override;


    bool cargarTileset(const QString& ruta, int anchoTile, int altoTile);
    bool cargarDesdeTiledJson(const QString& rutaJson, int capaIndex = 0);

    void cargarMapa(const std::vector<std::vector<int>>& matrizTiles);


    void renderizar(int camTileX, int camTileY, int tilesX, int tilesY);

    void renderizar(QPainter& p, const QRect& rectVista /*pixeles*/, const QPoint& offsetCamara);

    // consultas
    int anchoTile() const { return m_anchoTile; }
    int altoTile() const  { return m_altoTile; }

    int tileWidth() const { return m_anchoTile; }
    int tileHeight() const{ return m_altoTile; }

    int width() const  { return m_anchoMapa; }
    int height() const { return m_altoMapa; }
    QSize mapPixelSize() const { return QSize(m_anchoMapa * m_anchoTile, m_altoMapa * m_altoTile); }

    bool esBloqueado(int x, int y) const;
    int tileID(int x, int y) const;

    bool esTileCaminable(int tx, int ty) const;
    bool esTileEncuentro(int tx, int ty) const;

    QVector<QPoint> puntosSpawn(const QString& nombreCapa) const;
    QVector<PuntoObjeto> capaObjetos(const QString& nombre) const;


    QVector<QPoint> spawnPointsForLayer(const QString& layerName) const { return puntosSpawn(layerName); }
    QVector<QRect> objectLayer(const QString& layerName) const;
    bool isWalkableTile(int x, int y) const { return esTileCaminable(x,y); }
    bool isEncounterTile(int x, int y) const { return esTileEncuentro(x,y); }

private:

    bool parsearJson(const QByteArray& raw);
    int gidAIndiceTileset(int gid) const;

private:
    QGraphicsScene* m_escena = nullptr;
    QPixmap m_tileset;
    int m_anchoTile = 32;
    int m_altoTile  = 32;


    std::vector<std::vector<int>> m_mapa;

    QVector<Capa> m_capas;
    QMap<QString, QVector<PuntoObjeto>> m_capasObjetos;

    int m_anchoMapa = 0;
    int m_altoMapa  = 0;


    int m_columnasTileset = 0;
    int m_margen = 0;
    int m_espaciado = 0;
    int m_primerGid = 1;

    // carpeta donde se encontró el JSON (ayuda a resolver imágenes relativas)
    QString m_lastJsonDir;
};

#endif // MAPATILES_H
