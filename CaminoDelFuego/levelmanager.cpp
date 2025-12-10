#include "LevelManager.h" // CORRECCIÓN: nombre correcto con M mayúscula
#include "Level.h"
#include "NPC.h"
#include "MapaTiles.h"
#include <QRandomGenerator>
#include <QFileInfo>
#include <QDebug>

LevelManager::LevelManager(QObject* parent)
    : QObject(parent),
    m_indiceActual(-1),
    m_map(nullptr),
    m_scene(nullptr)
{
    // Crear escena y mapa. Pasamos 'this' como parent para que Qt gestione la destrucción
    m_scene = new QGraphicsScene(this);
    m_map = new MapaTiles(m_scene, this);
}

LevelManager::~LevelManager()
{
    // Liberar NPCs manualmente (fueron creados con new)
    for (NPC* p : m_npcs) {
        delete p;
    }
    m_npcs.clear();

    // NOTA: m_scene y m_map fueron creados con 'this' como parent, por lo que Qt
    // los destruirá automáticamente cuando este objeto (LevelManager) sea destruido.
    // Evitamos hacer `delete m_scene`/`delete m_map` para no correr riesgo de doble liberación.
    m_map = nullptr;
    m_scene = nullptr;
}

void LevelManager::agregarNivel(Level* nivel) {
    if (!nivel) return;
    m_niveles.push_back(nivel);
    if (m_indiceActual < 0) m_indiceActual = 0;
}

Level* LevelManager::nivelActual() const {
    if (m_indiceActual < 0 || m_indiceActual >= m_niveles.size()) return nullptr;
    return m_niveles.at(m_indiceActual);
}

void LevelManager::avanzarNivel() {
    if (m_indiceActual < 0) return;
    m_indiceActual++;
    if (m_indiceActual >= m_niveles.size()) {
        emit todosLosNivelesCompletados();
        return;
    }
    emit nivelCambio(nivelActual());
}

void LevelManager::reiniciarNivel() {
    emit nivelCambio(nivelActual());
}

bool LevelManager::loadLevelFile(const QString& path) {
    if (!m_map) {
        qWarning() << "LevelManager::loadLevelFile: m_map nulo";
        return false;
    }
    // Usa la API de MapaTiles. Asegúrate que la firma coincida (ruta como QString).
    if (!m_map->cargarDesdeTiledJson(path)) {
        qWarning() << "No se pudo cargar mapa Tiled JSON:" << path;
        return false;
    }

    // Limpiar NPCs existentes (se cargarán nuevos spawns al spawnInitialNPCs)
    for (NPC* p : m_npcs) delete p;
    m_npcs.clear();

    return true;
}

MapaTiles* LevelManager::map() { return m_map; }

void LevelManager::spawnInitialNPCs() {
    if (!m_map) return;

    // limpiar NPCs viejos (si hubo reinicio)
    for (NPC* p : m_npcs) {
        if (m_scene) m_scene->removeItem(p);
        delete p;
    }
    m_npcs.clear();

    // Intentamos usar object layer "Spawn1"/"Spawn2" desde m_capasObjetos
    QVector<QPoint> s1 = m_map->puntosSpawn("Spawn1");
    QVector<QPoint> s2 = m_map->puntosSpawn("Spawn2");

    auto asignarSpriteSegunFaccion = [this](NPC* npc){
        QString spritePath;
        if (npc->faccion() == Faccion::CRISTIANO) spritePath = "C:/Users/USUARIO/DesafioIII/sprites/cristiano.png";
        else if (npc->faccion() == Faccion::ENDEMONIADO || npc->faccion() == Faccion::POSEIDO) spritePath = "C:/Users/USUARIO/DesafioIII/sprites/demonio.png";
        else spritePath = "C:/Users/USUARIO/DesafioIII/sprites/npc_default.png";

        QPixmap px(spritePath);
        if (!px.isNull()) {
            npc->setPixmap(px);
            // ajustar offset para que los pies queden en la base del tile (si px más alto que tile)
            npc->setOffset(-px.width()/2.0, - (px.height() - m_map->altoTile()));
        }
    };

    for (const QPoint& t : s1) {
        Faccion f = (QRandomGenerator::global()->bounded(2) == 0) ? Faccion::ENDEMONIADO : Faccion::POSEIDO;
        NPC* npc = new NPC(f);
        npc->setPos(t.x() * m_map->anchoTile(), t.y() * m_map->altoTile());
        asignarSpriteSegunFaccion(npc);
        m_npcs.append(npc);
        if (m_scene) m_scene->addItem(npc);
    }
    for (const QPoint& t : s2) {
        NPC* npc = new NPC(Faccion::CRISTIANO);
        npc->setPos(t.x() * m_map->anchoTile(), t.y() * m_map->altoTile());
        asignarSpriteSegunFaccion(npc);
        m_npcs.append(npc);
        if (m_scene) m_scene->addItem(npc);
    }

    // fallback si no hay spawns detectados
    if (m_npcs.isEmpty()) {
        for (int i = 0; i < 3; ++i) {
            NPC* n = new NPC(Faccion::CRISTIANO);
            n->setPos((i+1) * m_map->anchoTile(), 2 * m_map->altoTile());
            asignarSpriteSegunFaccion(n);
            m_npcs.append(n);
            if (m_scene) m_scene->addItem(n);
        }
    }
}

void LevelManager::updateAI() {
    if (!m_map) return;
    // Movimiento aleatorio simple para NPCs
    for (NPC* n : m_npcs) {
        if (!n) continue;
        int r = QRandomGenerator::global()->bounded(100);
        if (r < 30) {
            QPointF p = n->pos();
            int dx = (QRandomGenerator::global()->bounded(3) - 1);
            int dy = (QRandomGenerator::global()->bounded(3) - 1);
            // mover 1 tile (en pixeles)
            n->setPos(p.x() + dx * m_map->anchoTile(), p.y() + dy * m_map->altoTile());
        }
    }
}
