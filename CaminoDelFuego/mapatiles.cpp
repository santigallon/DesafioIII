#include "MapaTiles.h"
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QJsonDocument>
#include <QPainter>
#include <QDebug>
#include <QGraphicsPixmapItem>

MapaTiles::MapaTiles(QGraphicsScene* escena, QObject* parent)
    : QObject(parent),
    m_escena(escena),
    m_anchoTile(32),
    m_altoTile(32),
    m_anchoMapa(0),
    m_altoMapa(0),
    m_columnasTileset(0),
    m_margen(0),
    m_espaciado(0),
    m_primerGid(1)
{
}

MapaTiles::~MapaTiles() = default;

bool MapaTiles::cargarTileset(const QString& ruta, int anchoTile, int altoTile) {
    QPixmap px;
    QString path = ruta;

    // si la ruta es relativa y tenemos m_lastJsonDir, resolver respecto a ella
    if (!QFileInfo(path).isAbsolute() && !m_lastJsonDir.isEmpty()) {
        QString alt = QDir(m_lastJsonDir).filePath(path);
        if (QFile::exists(alt)) path = alt;
    }

    // Intentar cargar (ruta absoluta o relativa)
    if (QFile::exists(path)) {
        if (!px.load(path)) {
            qWarning() << "MapaTiles::cargarTileset: fallo al cargar imagen desde ruta:" << path;
            return false;
        }
    } else {
        // intentar como recurso qrc (:/prefix/...)
        QString r2 = path;
        if (!r2.startsWith(":/") && QFile::exists(QString(":/%1").arg(path))) r2 = QString(":/%1").arg(path);
        if (!px.load(r2)) {
            qWarning() << "MapaTiles::cargarTileset - no se encontró imagen tileset en:" << path << "ni en recurso:" << r2;
            return false;
        }
    }

    if (px.isNull()) {
        qWarning() << "MapaTiles::cargarTileset - pixmap nulo:" << ruta;
        return false;
    }

    m_tileset = px;
    m_anchoTile = anchoTile;
    m_altoTile = altoTile;
    if (m_anchoTile > 0)
        m_columnasTileset = (m_tileset.width() - 2*m_margen + m_espaciado) / (m_anchoTile + m_espaciado);
    else
        m_columnasTileset = 0;
    return true;
}

void MapaTiles::cargarMapa(const std::vector<std::vector<int>>& matrizTiles) {
    m_mapa = matrizTiles;
    m_altoMapa = (int)m_mapa.size();
    m_anchoMapa = (m_altoMapa > 0) ? (int)m_mapa[0].size() : 0;
}


void MapaTiles::renderizar(int camTileX, int camTileY, int tilesX, int tilesY) {
    if (!m_escena) return;
    // limpiamos la escena y llenamos con pixmaps en coordenadas globales
    m_escena->clear();

    if (m_mapa.empty() || m_tileset.isNull()) return;

    int filas = (int)m_mapa.size();
    int columnas = filas ? (int)m_mapa[0].size() : 0;

    for (int y = 0; y < filas; ++y) {
        for (int x = 0; x < columnas; ++x) {
            int id = m_mapa[y][x]; // id local (0..n-1) o 0 vacío
            if (id <= 0) continue;
            int tileIndex = id;
            if (m_columnasTileset <= 0) continue;
            int sx = (tileIndex % m_columnasTileset) * m_anchoTile;
            int sy = (tileIndex / m_columnasTileset) * m_altoTile;
            QPixmap sub = m_tileset.copy(sx, sy, m_anchoTile, m_altoTile);
            QGraphicsPixmapItem* it = new QGraphicsPixmapItem(sub);
            it->setPos(x * m_anchoTile, y * m_altoTile);
            it->setZValue(0);
            m_escena->addItem(it);
        }
    }
}

// Renderizado directo con QPainter (usa capas y m_capas). rectVista en pixeles, offsetCamara en pixeles.
void MapaTiles::renderizar(QPainter& p, const QRect& rectVista, const QPoint& offsetCamara) {
    // Si no hay tileset, dibujamos un placeholder gris en tiles no vacíos
    if (m_tileset.isNull()) {
        p.setPen(Qt::NoPen);
        for (const Capa& C : m_capas) {
            if (C.esCapaObjetos) continue;
            for (int ty = 0; ty < C.alto; ++ty) {
                for (int tx = 0; tx < C.ancho; ++tx) {
                    int idx = ty * C.ancho + tx;
                    int gid = (idx >= 0 && idx < C.datos.size()) ? C.datos[idx] : 0;
                    if (gid == 0) continue;
                    QRect tileRect(tx*m_anchoTile - offsetCamara.x(), ty*m_altoTile - offsetCamara.y(), m_anchoTile, m_altoTile);
                    if (!tileRect.intersects(rectVista)) continue;
                    p.fillRect(tileRect, QColor(120,120,120,200));
                }
            }
        }
        return;
    }

    int startX = qMax(0, rectVista.left() / m_anchoTile);
    int endX   = qMin(m_anchoMapa - 1, rectVista.right() / m_anchoTile);
    int startY = qMax(0, rectVista.top() / m_altoTile);
    int endY   = qMin(m_altoMapa - 1, rectVista.bottom() / m_altoTile);

    for (const Capa& C : m_capas) {
        if (C.esCapaObjetos) continue;
        for (int ty = startY; ty <= endY; ++ty) {
            for (int tx = startX; tx <= endX; ++tx) {
                int idx = ty * C.ancho + tx;
                int gid = (idx >= 0 && idx < C.datos.size()) ? C.datos[idx] : 0;
                if (gid <= 0) continue;
                int tileIndex = gidAIndiceTileset(gid);
                if (tileIndex < 0) continue;
                int sx = m_margen + (tileIndex % m_columnasTileset) * (m_anchoTile + m_espaciado);
                int sy = m_margen + (tileIndex / m_columnasTileset) * (m_altoTile + m_espaciado);
                QRect src(sx, sy, m_anchoTile, m_altoTile);
                QRect dst(tx*m_anchoTile - offsetCamara.x(), ty*m_altoTile - offsetCamara.y(), m_anchoTile, m_altoTile);
                p.drawPixmap(dst, m_tileset, src);
            }
        }
    }
}

bool MapaTiles::esBloqueado(int x, int y) const {
    if (m_mapa.empty()) return true;
    if (y < 0 || y >= (int)m_mapa.size()) return true;
    if (x < 0 || x >= (int)m_mapa[y].size()) return true;
    // en nuestro esquema: 0 = vacío/transitable, <0 = muro, >0 = index de tile
    int val = m_mapa[y][x];
    return (val < 0);
}

int MapaTiles::tileID(int x, int y) const {
    if (y < 0 || y >= (int)m_mapa.size()) return -1;
    if (x < 0 || x >= (int)m_mapa[y].size()) return -1;
    return m_mapa[y][x];
}

bool MapaTiles::cargarDesdeTiledJson(const QString& rutaJson, int capaIndex)
{

    qDebug() << "MapaTiles::cargarDesdeTiledJson - intentanto abrir:" << rutaJson;

    QFileInfo fi(rutaJson);
    QString pathToOpen = fi.absoluteFilePath();

    if (!fi.exists() || !fi.isFile()) {

        QString alt = QCoreApplication::applicationDirPath() + QDir::separator() + rutaJson;
        qDebug() << "Archivo no encontrado en la ruta dada. Intentando alternativa:" << alt;
        QFileInfo fi2(alt);
        if (fi2.exists() && fi2.isFile()) {
            pathToOpen = fi2.absoluteFilePath();
            qDebug() << "Encontrado en alternativa:" << pathToOpen;
        } else {
            QString rc = rutaJson;
            if (!rc.startsWith(":/")) {
                rc = QString(":/%1").arg(rutaJson);
            }
            if (QFile::exists(rc)) {
                qDebug() << "Encontrado en recursos (qrc):" << rc;
                QFile f(rc);
                if (!f.open(QIODevice::ReadOnly)) {
                    qWarning() << "MapaTiles: no se pudo abrir recurso:" << rc;
                    return false;
                }
                QByteArray raw = f.readAll();
                f.close();
                if (!parsearJson(raw)) { qWarning() << "Error parseando JSON (qrc)"; return false; }
                if (m_escena) m_escena->setSceneRect(0, 0, m_anchoMapa * m_anchoTile, m_altoMapa * m_altoTile);
                return true;
            }

            qWarning() << "MapaTiles: no se encontró el JSON en ninguna ruta. Rutas comprobadas:"
                       << rutaJson << alt << rc;
            return false;
        }
    }

    QFile f(pathToOpen);
    if (!f.open(QIODevice::ReadOnly)) {
        qWarning() << "MapaTiles: no se pudo abrir JSON:" << pathToOpen;
        return false;
    }
    QByteArray raw = f.readAll();
    f.close();

    // guardamos directorio del JSON para resolver rutas relativas a imagenes/tilesets
    m_lastJsonDir = QFileInfo(pathToOpen).absolutePath();

    if (!parsearJson(raw)) {
        qWarning() << "Error parseando JSON:" << pathToOpen;
        return false;
    }

    // fijar rect de la escena para que QGraphicsView lo muestre correctamente
    if (m_escena) m_escena->setSceneRect(0, 0, m_anchoMapa * m_anchoTile, m_altoMapa * m_altoTile);

    return true;
}

int MapaTiles::gidAIndiceTileset(int gid) const {
    // devuelve índice 0-based en tileset o -1 si inválido
    if (gid <= 0) return -1;
    int local = gid - m_primerGid;
    return local >= 0 ? local : -1;
}

// Parsear JSON y llenar m_capas, m_capasObjetos y propiedades generales
bool MapaTiles::parsearJson(const QByteArray& raw) {
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(raw, &err);
    if (err.error != QJsonParseError::NoError || !doc.isObject()) {
        qWarning() << "MapaTiles parsearJson: error parseando JSON:" << err.errorString();
        return false;
    }
    QJsonObject raiz = doc.object();

    m_anchoMapa = raiz.value("width").toInt(0);
    m_altoMapa  = raiz.value("height").toInt(0);
    m_anchoTile = raiz.value("tilewidth").toInt(m_anchoTile);
    m_altoTile  = raiz.value("tileheight").toInt(m_altoTile);

    // tileset (tomamos el primero)
    QJsonArray tilesets = raiz.value("tilesets").toArray();
    if (!tilesets.isEmpty()) {
        QJsonObject ts = tilesets.at(0).toObject();
        m_primerGid = ts.value("firstgid").toInt(1);
        QString imagePath = ts.value("image").toString();
        if (!imagePath.isEmpty()) {
            // Intentar resolver respecto al directorio del JSON (m_lastJsonDir)
            QString fullPath = imagePath;
            if (!QFileInfo(fullPath).isAbsolute() && !m_lastJsonDir.isEmpty()) {
                QString alt = QDir(m_lastJsonDir).filePath(imagePath);
                if (QFile::exists(alt)) fullPath = alt;
            }
            int tw = raiz.value("tilewidth").toInt(m_anchoTile);
            int th = raiz.value("tileheight").toInt(m_altoTile);
            if (cargarTileset(fullPath, tw, th)) {
                qDebug() << "MapaTiles: tileset cargado automaticamente:" << fullPath;
            } else {
                qDebug() << "MapaTiles: no pudo auto-cargar tileset desde:" << fullPath;
            }
        }
    }

    m_capas.clear();
    m_capasObjetos.clear();

    QJsonArray capas = raiz.value("layers").toArray();
    for (auto v : capas) {
        QJsonObject L = v.toObject();
        QString tipo = L.value("type").toString();
        Capa capa;
        capa.nombre = L.value("name").toString();
        if (tipo == "tilelayer") {
            capa.esCapaObjetos = false;
            capa.ancho = L.value("width").toInt(m_anchoMapa);
            capa.alto  = L.value("height").toInt(m_altoMapa);
            QJsonArray datos = L.value("data").toArray();
            capa.datos.resize(capa.ancho * capa.alto);
            for (int i = 0; i < datos.size() && i < capa.datos.size(); ++i)
                capa.datos[i] = datos.at(i).toInt(0);
            m_capas.push_back(capa);
        } else if (tipo == "objectgroup") {
            capa.esCapaObjetos = true;
            QVector<PuntoObjeto> lista;
            QJsonArray objs = L.value("objects").toArray();
            for (auto o : objs) {
                QJsonObject obj = o.toObject();
                PuntoObjeto po;
                po.nombre = obj.value("name").toString();
                po.tipo = obj.value("type").toString();
                po.x = obj.value("x").toInt();
                po.y = obj.value("y").toInt();
                if (obj.contains("properties")) po.propiedades = obj.value("properties").toObject();
                lista.append(po);
            }
            m_capasObjetos.insert(capa.nombre, lista);
            // añadir capa de objeto también a m_capas si quieres mantener orden
            capa.datos.clear();
            capa.ancho = 0; capa.alto = 0;
            m_capas.push_back(capa);
        } else {
            // otro tipo: ignoramos por ahora
        }
    }

    return true;
}

QVector<QPoint> MapaTiles::puntosSpawn(const QString& nombreCapa) const {
    QVector<QPoint> out;
    if (m_capasObjetos.contains(nombreCapa)) {
        for (const PuntoObjeto& op : m_capasObjetos.value(nombreCapa)) {
            int tx = (m_anchoTile > 0) ? (op.x / m_anchoTile) : 0;
            int ty = (m_altoTile  > 0) ? (op.y / m_altoTile)  : 0;
            out.append({tx, ty});
        }
        return out;
    }
    // Si no hay object layer, intentar buscar en tilelayers por nombre
    for (const Capa& C : m_capas) {
        if (!C.esCapaObjetos && C.nombre.compare(nombreCapa, Qt::CaseInsensitive) == 0) {
            for (int ty = 0; ty < C.alto; ++ty)
                for (int tx = 0; tx < C.ancho; ++tx) {
                    int idx = ty * C.ancho + tx;
                    int gid = (idx >= 0 && idx < C.datos.size()) ? C.datos[idx] : 0;
                    if (gid != 0) out.append({tx, ty});
                }
            return out;
        }
    }
    return out;
}

QVector<PuntoObjeto> MapaTiles::capaObjetos(const QString& nombre) const {
    return m_capasObjetos.value(nombre);
}

QVector<QRect> MapaTiles::objectLayer(const QString& layerName) const {
    QVector<QRect> out;
    if (!m_capasObjetos.contains(layerName)) return out;
    for (const PuntoObjeto& p : m_capasObjetos.value(layerName)) {
        QRect r(p.x, p.y, m_anchoTile, m_altoTile);
        out.append(r);
    }
    return out;
}

bool MapaTiles::esTileCaminable(int tx, int ty) const {
    if (tx < 0 || ty < 0 || tx >= m_anchoMapa || ty >= m_altoMapa) return false;
    for (const Capa& C : m_capas) {
        if (!C.esCapaObjetos && C.nombre.compare("Caminable", Qt::CaseInsensitive) == 0) {
            int idx = ty * C.ancho + tx;
            int gid = (idx >= 0 && idx < C.datos.size()) ? C.datos[idx] : 0;
            return gid != 0;
        }
    }
    // si no hay capa "Caminable", asumimos transitable
    return true;
}

bool MapaTiles::esTileEncuentro(int tx, int ty) const {
    if (tx < 0 || ty < 0 || tx >= m_anchoMapa || ty >= m_altoMapa) return false;
    for (const Capa& C : m_capas) {
        if (!C.esCapaObjetos && C.nombre.compare("Encuentro", Qt::CaseInsensitive) == 0) {
            int idx = ty * C.ancho + tx;
            int gid = (idx >= 0 && idx < C.datos.size()) ? C.datos[idx] : 0;
            return gid != 0;
        }
    }
    return false;
}
