#include "GameWidget.h"
#include "VentanaCartasPulinas.h"
#include "HouseWindow.h"
#include "PatternWindow.h"
#include <QPainter>
#include <QKeyEvent>
#include <QDebug>
#include <QLabel>
#include <QCoreApplication>
#include <QFileInfo>

GameWidget::GameWidget(QWidget* parent)
    : QWidget(parent),
    m_player(1,1),
    m_playerVida(10),
    m_tileSize(48),
    m_cartasWnd(new VentanaCartasPulinas(this)),
    m_cameraX(0), m_cameraY(0)
{
    setFocusPolicy(Qt::StrongFocus);
    setMinimumSize(800, 600);

    connect(m_cartasWnd, &VentanaCartasPulinas::cartaSeleccionada, this, &GameWidget::usarCartaSeleccionada);
    connect(m_cartasWnd, &VentanaCartasPulinas::ventanaCerrada, this, &GameWidget::ventanaCartasCerrada);

    m_timerId = startTimer(200);

    // intenta cargar sprites automáticos (rutas por defecto)
    cargarSpritesPorDefecto();

    // cargar nivel inicial
    cargarNivel("assets/maps/caminoDamasco.json");
}

GameWidget::~GameWidget() = default;

void GameWidget::cargarSpritesPorDefecto() {
    QString base = QCoreApplication::applicationDirPath();
    // rutas relativas: ./sprites/...
    QString folder = QDir(base).filePath("sprites");

    auto tryLoad = [&](const QString& name)->QPixmap{
        QString p1 = QDir(folder).filePath(name);
        QString p2 = QDir(base).filePath(name); // alternativa
        if (QFileInfo::exists(p1)) {
            QPixmap px(p1);
            if (!px.isNull()) {
                qDebug() << "GameWidget: cargado sprite" << p1;
                return px;
            }
        }
        if (QFileInfo::exists(p2)) {
            QPixmap px(p2);
            if (!px.isNull()) {
                qDebug() << "GameWidget: cargado sprite" << p2;
                return px;
            }
        }
        // intentar recurso qrc
        QString rsrc = QString(":/sprites/%1").arg(name);
        if (QFileInfo::exists(rsrc) || QFile::exists(rsrc)) {
            QPixmap px(rsrc);
            if (!px.isNull()) {
                qDebug() << "GameWidget: cargado sprite (qrc)" << rsrc;
                return px;
            }
        }
        return QPixmap();
    };

    m_playerSprite = tryLoad("player.png");
    m_npcSprites[Faccion::CRISTIANO] = tryLoad("npc_cristiano.png");
    m_npcSprites[Faccion::ROMANO]    = tryLoad("npc_romano.png");
    m_npcSprites[Faccion::ENDEMONIADO] = tryLoad("npc_endemoniado.png");
    m_npcSprites[Faccion::POSEIDO] = tryLoad("npc_poseido.png");

    // si los sprites existen pero su tamaño no coincide con anchoTile/altoTile,
    // los escalaremos al dibujar (no forzamos aquí).
}

QPixmap GameWidget::spriteForFaction(Faccion f) const {
    if (m_npcSprites.contains(f)) return m_npcSprites[f];
    return QPixmap();
}

void GameWidget::cargarNivel(const QString& path) {
    if (!m_level.loadLevelFile(path)) {
        qWarning() << "No se pudo cargar nivel:" << path;
        return;
    }
    m_level.spawnInitialNPCs();
    // ubicar jugador en primer spawn cristiano o en (1,1)
    QGraphicsScene* sc = m_level.scene();
    if (sc) {
        QList<QGraphicsItem*> items = sc->items();
        bool found = false;
        for (QGraphicsItem* it : items) {
            // los items de spawn tal vez no sean QGraphicsItems colocados con data(0) == "Spawn2"
            if (it->data(0).toString() == "Spawn2") {
                QPointF p = it->pos();
                m_player = QPoint(int(p.x()) / m_level.map()->anchoTile(), int(p.y()) / m_level.map()->altoTile());
                found = true;
                break;
            }
        }
        if (!found) m_player = QPoint(1,1);
    } else {
        m_player = QPoint(1,1);
    }

    m_playerVida = 10;
    actualizarCamara();
    update();
}

void GameWidget::paintEvent(QPaintEvent*) {
    QPainter p(this);
    p.fillRect(rect(), Qt::black);

    if (m_level.map()) {
        // pedimos al mapa que prepare la region visible (renderizar)
        int tilesX = width() / m_level.map()->anchoTile() + 2;
        int tilesY = height() / m_level.map()->altoTile() + 2;

        // renderizar en la QGraphicsScene (esto añade QGraphicsPixmapItem al scene)
        m_level.map()->renderizar(m_cameraX, m_cameraY, tilesX, tilesY);

        // dibujamos la escena asociada al mapa directamente sobre el painter,
        // renderizando la porción de mapa que esté en la cámara.
        if (m_level.scene()) {
            QRectF sourceRect(m_cameraX, m_cameraY, width(), height());
            m_level.scene()->render(&p, QRectF(0,0,width(),height()), sourceRect);
        }
    }

    // dibujar NPCs desde la lista del LevelManager: usar pixmap de cada NPC si existe
    p.setPen(Qt::NoPen);
    for (NPC* n : m_level.npcs()) {
        if (!n) continue;
        QPointF np = n->pos();
        QRect dst(int(np.x()) - m_cameraX, int(np.y()) - m_cameraY,
                  m_level.map()->anchoTile(), m_level.map()->altoTile());
        QPixmap pm = n->pixmap();
        if (!pm.isNull()) {
            QPixmap scaled = pm.scaled(m_level.map()->anchoTile(), m_level.map()->altoTile(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
            int dx = dst.left() + (dst.width() - scaled.width())/2;
            int dy = dst.top() + (dst.height() - scaled.height());
            p.drawPixmap(dx, dy, scaled);
        } else {
            QColor c = Qt::gray;
            if (n->faccion() == Faccion::ENDEMONIADO || n->faccion() == Faccion::POSEIDO) c = Qt::magenta;
            else if (n->faccion() == Faccion::CRISTIANO) c = Qt::green;
            else if (n->faccion() == Faccion::ROMANO) c = Qt::red;
            p.fillRect(dst.adjusted(6,6,-6,-6), c);
        }
    }

    // jugador (usar sprite si existe en disco - reemplaza la ruta por la tuya)
    QPixmap playerPx;
    // Intentamos primero usar un sprite asignado a un Player en la escena (no obligatorio)
    // fallback: cargar sprite de archivo
    playerPx.load("C:/Users/USUARIO/DesafioIII/sprites/pablo.png");

    QRect playerRect(m_player.x()*m_level.map()->anchoTile() - m_cameraX,
                     m_player.y()*m_level.map()->altoTile() - m_cameraY,
                     m_level.map()->anchoTile(), m_level.map()->altoTile());
    if (!playerPx.isNull()) {
        QPixmap sp = playerPx.scaled(playerRect.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        int dx = playerRect.left() + (playerRect.width() - sp.width())/2;
        int dy = playerRect.top() + (playerRect.height() - sp.height());
        p.drawPixmap(dx, dy, sp);
    } else {
        p.fillRect(playerRect.adjusted(4,4,-4,-4), Qt::blue);
    }

    dibujarHUD(p);
}

void GameWidget::keyPressEvent(QKeyEvent* ev) {
    if (ev->key() == Qt::Key_Escape) { window()->close(); return; }

    if (ev->key() == Qt::Key_Left) moverJugador(-1,0);
    else if (ev->key() == Qt::Key_Right) moverJugador(1,0);
    else if (ev->key() == Qt::Key_Up) moverJugador(0,-1);
    else if (ev->key() == Qt::Key_Down) moverJugador(0,1);
    else if (ev->key() == Qt::Key_Space) {
        QStringList cartas = {"Cura pequeña", "Ataque espiritual", "Escudo breve"};
        m_cartasWnd->iniciar(cartas);
    } else if (ev->key() == Qt::Key_E) {
        QGraphicsScene* sc = m_level.scene();
        if (sc) {
            int px = m_player.x();
            int py = m_player.y();
            QRectF checkRect((px-1)*m_level.map()->anchoTile(), (py-1)*m_level.map()->altoTile(),
                             3 * m_level.map()->anchoTile(), 3 * m_level.map()->altoTile());
            QList<QGraphicsItem*> items = sc->items(checkRect);
            for (QGraphicsItem* it : items) {
                if (it->data(0).toString().toLower().contains("puert")) {
                    QPointF pos = it->pos();
                    QRect doorRect(int(pos.x())/m_level.map()->anchoTile(),
                                   int(pos.y())/m_level.map()->altoTile(), 1, 1);
                    HouseWindow hw(&m_level, doorRect, this);
                    connect(&hw, &HouseWindow::accionRealizada, this, &GameWidget::onHouseAction);
                    hw.exec();
                    break;
                }
            }
        }
    } else QWidget::keyPressEvent(ev);

    update();
}

void GameWidget::moverJugador(int dx, int dy) {
    QPoint nueva = m_player + QPoint(dx, dy);
    if (!m_level.map()) return;
    if (nueva.x() < 0 || nueva.y() < 0) return;
    if (m_level.map()->esBloqueado(nueva.x(), nueva.y())) return;

    m_player = nueva;
    comprobarEncuentro();
    actualizarCamara();
    update();
}

void GameWidget::comprobarEncuentro() {
    if (!m_level.map()) return;
    QGraphicsScene* sc = m_level.scene();
    if (!sc) return;
    int px = m_player.x();
    int py = m_player.y();
    QRectF rect(px * m_level.map()->anchoTile(), py * m_level.map()->altoTile(),
                m_level.map()->anchoTile(), m_level.map()->altoTile());
    for (QGraphicsItem* it : sc->items(rect)) {
        if (it->data(0).toString().toLower().contains("encuentro")) {
            QLabel* l = new QLabel("¡Has llegado a la meta de este nivel!", this);
            l->show();
            return;
        }
    }
}

void GameWidget::actualizarCamara() {
    if (!m_level.map()) return;
    QSize mp = m_level.map()->mapPixelSize();
    int mapW = mp.width();
    int mapH = mp.height();
    int px = m_player.x()*m_level.map()->anchoTile();
    int py = m_player.y()*m_level.map()->altoTile();
    int camX = px - width()/2 + m_level.map()->anchoTile()/2;
    int camY = py - height()/2 + m_level.map()->altoTile()/2;
    camX = qBound(0, camX, qMax(0, mapW - width()));
    camY = qBound(0, camY, qMax(0, mapH - height()));
    m_cameraX = camX; m_cameraY = camY;
}

void GameWidget::usarCartaSeleccionada(const QString& carta) {
    if (carta == "Cura pequeña") {
        m_playerVida += 4; if (m_playerVida > 10) m_playerVida = 10;
    } else if (carta == "Ataque espiritual") {
        for (NPC* n : m_level.editableNpcs()) {
            if (!n) continue;
            QPointF npcPos = n->pos();
            QPoint npcTilePos(qRound(npcPos.x()) / m_level.map()->anchoTile(), qRound(npcPos.y()) / m_level.map()->altoTile());
            if ((npcTilePos - m_player).manhattanLength() <= 1) {
                n->modificarVida(-4);
            }
        }
    } else if (carta == "Escudo breve") {
        m_playerVida += 1; if (m_playerVida > 10) m_playerVida = 10;
    }
    update();
}

void GameWidget::onHouseAction(const QString& tipo, bool exito) {
    if (tipo.contains("Exorcismo") && exito) {
        for (int i = m_level.editableNpcs().size()-1; i >= 0; --i) {
            NPC* n = m_level.editableNpcs().at(i);
            if (!n) continue;
            QPointF np = n->pos();
            QPoint npcTile(qRound(np.x())/m_level.map()->anchoTile(), qRound(np.y())/m_level.map()->altoTile());
            if ((npcTile - m_player).manhattanLength() <= 3
                && (n->faccion() == Faccion::ENDEMONIADO || n->faccion() == Faccion::POSEIDO)) {
                m_level.scene()->removeItem(n);
                delete n;
                m_level.editableNpcs().remove(i);
            }
        }
    }
    update();
}

void GameWidget::timerEvent(QTimerEvent* ev) {
    if (ev->timerId() != m_timerId) { QWidget::timerEvent(ev); return; }
    m_level.updateAI();
    update();
}

void GameWidget::dibujarHUD(QPainter& p) {
    p.setPen(Qt::white);
    p.setFont(QFont("Arial", 12));
    QString s = QString("Vida: %1").arg(m_playerVida);
    p.drawText(8, 20, s);
    p.drawText(8, 40, "Flechas:mover · E:Interactuar · Space:Cartas");
}

void GameWidget::ventanaCartasCerrada() {
    this->setFocus();
}

void GameWidget::resetJuego() {
    if (m_level.map()) {
        m_level.spawnInitialNPCs();
    }
    m_player = QPoint(1,1);
    m_playerVida = 10;
    actualizarCamara();
    update();
}
