#include "CargadorTiled.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

bool CargadorTiled::cargarJSON(
    const QString& ruta,
    std::vector<std::vector<int>>& matriz,
    int& width,
    int& height,
    int& tileW,
    int& tileH
    ) {
    QFile archivo(ruta);
    if (!archivo.open(QIODevice::ReadOnly))
        return false;

    QJsonDocument doc = QJsonDocument::fromJson(archivo.readAll());
    archivo.close();

    if (!doc.isObject())
        return false;

    QJsonObject root = doc.object();

    width  = root["width"].toInt();
    height = root["height"].toInt();
    tileW  = root["tilewidth"].toInt();
    tileH  = root["tileheight"].toInt();

    QJsonArray layers = root["layers"].toArray();
    if (layers.isEmpty()) return false;

    QJsonObject capa0 = layers[0].toObject();

    QJsonArray data = capa0["data"].toArray();
    if (data.isEmpty()) return false;

    // Convertimos 1D → 2D
    matriz.resize(height, std::vector<int>(width, 0));

    int index = 0;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            matriz[y][x] = data[index].toInt() - 1;  // Ajustar ID (Tiled usa 1-based)
            index++;
        }
    }

    return true;
}
