#include "TiledLoader.h"
#include <QDebug>

TiledLoader::TiledLoader(QObject* parent)
    : QObject(parent)
{ }

bool TiledLoader::cargar(const QString& archivoJSON) {

    QFile f(archivoJSON);
    if (!f.open(QIODevice::ReadOnly)) {
        qDebug() << "TiledLoader::cargar - Error al abrir mapa:" << archivoJSON;
        return false;
    }

    const QByteArray raw = f.readAll();
    f.close();

    QJsonParseError perr;
    QJsonDocument doc = QJsonDocument::fromJson(raw, &perr);
    if (perr.error != QJsonParseError::NoError || !doc.isObject()) {
        qDebug() << "TiledLoader::cargar - Error parseando JSON:" << perr.errorString();
        return false;
    }

    QJsonObject root = doc.object();

    // Validaciones mínimas
    if (!root.contains("width") || !root.contains("height") || !root.contains("layers")) {
        qDebug() << "TiledLoader::cargar - JSON no tiene los campos esperados (width/height/layers).";
        return false;
    }

    m_width = root["width"].toInt(0);
    m_height = root["height"].toInt(0);
    m_tileWidth = root["tilewidth"].toInt(0);
    m_tileHeight = root["tileheight"].toInt(0);

    // Clear previo
    m_capas.clear();

    QJsonArray layers = root["layers"].toArray();
    for (const QJsonValue& layerVal : layers) {
        if (!layerVal.isObject()) continue;
        QJsonObject layer = layerVal.toObject();

        QString nombre = layer.value("name").toString();
        QString tipo = layer.value("type").toString();

        // Sólo procesamos tilelayer (ignorar objectgroup, etc.)
        if (tipo != "tilelayer") {
            // si quieres leer object layers deberías tratarlas aparte
            continue;
        }

        QJsonArray datos = layer.value("data").toArray();
        // seguridad: si data no tiene el tamaño expected, ignorar esta capa
        if (datos.size() < m_width * m_height) {
            qDebug() << "TiledLoader::cargar - capa" << nombre << "tiene data con tamaño inesperado:" << datos.size()
                     << "esperado:" << m_width * m_height << ". Se ignora capa.";
            continue;
        }

        std::vector<std::vector<int>> matriz;
        matriz.assign(m_height, std::vector<int>(m_width, -1)); // -1 = vacío

        int index = 0;
        for (int y = 0; y < m_height; ++y) {
            for (int x = 0; x < m_width; ++x) {
                int tileID = datos.at(index).toInt(0); // Tiled devuelve 0 para vacío, 1..n para tiles
                if (tileID <= 0) {
                    matriz[y][x] = -1; // vacío
                } else {
                    matriz[y][x] = tileID - 1; // convertimos a 0-based
                }
                ++index;
            }
        }

        m_capas.insert(nombre, std::move(matriz));
    }

    return true;
}

const std::vector<std::vector<int>>& TiledLoader::capa(const QString& nombre) const {
    static const std::vector<std::vector<int>> vacia; // referencia a devolver si no existe
    auto it = m_capas.constFind(nombre);
    if (it != m_capas.constEnd()) {
        return it.value();
    }
    return vacia;
}
