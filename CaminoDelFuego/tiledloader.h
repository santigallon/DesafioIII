#ifndef TILEDLOADER_H
#define TILEDLOADER_H

#pragma once
#include <QObject>
#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QHash>
#include <vector>

class TiledLoader : public QObject {
    Q_OBJECT
public:
    explicit TiledLoader(QObject* parent = nullptr);

    // Carga el JSON y devuelve true si todo OK
    bool cargar(const QString& archivoJSON);

    // Devuelve una referencia a la matriz (rows = height, cols = width) de IDs (-1 = vacío)
    // Si la capa no existe devuelve una referencia a un vector vacío estático.
    const std::vector<std::vector<int>>& capa(const QString& nombre) const;

    QStringList nombresCapas() const { return m_capas.keys(); }

    int width() const { return m_width; }
    int height() const { return m_height; }
    int tileWidth() const { return m_tileWidth; }
    int tileHeight() const { return m_tileHeight; }

private:
    int m_width = 0;
    int m_height = 0;
    int m_tileWidth = 0;
    int m_tileHeight = 0;

    QHash<QString, std::vector<std::vector<int>>> m_capas;
};

#endif // TILEDLOADER_H
