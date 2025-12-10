#include "Game.h"
#include "Player.h"
#include "LevelManager.h"
#include "Level.h"
#include "GestorInteracciones.h"
#include "GestorSonido.h"
#include "GestorFisicas.h"
#include "GestorEventos.h"
#include "MapaTiles.h"
#include "Camara2D.h"
#include "registro.h"

#include <QDebug>

void Game::setScene(QGraphicsScene* scene)
{
    m_scene = scene;
}

Game::Game(QObject* parent)
    : QObject(parent),
    m_player(nullptr),
    m_levels(nullptr),
    m_interacciones(nullptr),
    m_sonido(nullptr),
    m_fisicas(nullptr),
    m_eventos(nullptr),
    m_mapa(nullptr),
    m_camara(nullptr)
{
}

Game::~Game() = default;

void Game::inicializar() {
    // Verificar que la escena ya fue seteada por MainWindow
    if (!m_scene) {
        qWarning() << "Game::inicializar: m_scene es nullptr. Llama a setScene() antes de inicializar.";
        return;
    }

    // Crear componentes principales
    m_player = new Player();                       // jugador (Entidad QGraphicsPixmapItem)
    m_camara = new Camara2D();                     // cámara lógica (si la usas)
    m_interacciones = new GestorInteracciones(this);
    m_sonido = new GestorSonido(this);
    m_eventos = new GestorEventos(this);
    m_fisicas = new GestorFisicas(m_scene, this);
    m_registro = new Registro(this);

    // LevelManager (cada Level se configurará con m_mapa)
    m_levels = new LevelManager(this);

    // Crear y cargar el mapa principal (se guarda en m_mapa, no en una variable local)
    if (m_mapa) { delete m_mapa; m_mapa = nullptr; } // asegurar limpieza si ya existía
    m_mapa = new MapaTiles(m_scene, this);

    const QString rutaJson = QStringLiteral("C:/Users/USUARIO/DesafioIII/sprites/mapaEfeso.json");
    if (!m_mapa->cargarDesdeTiledJson(rutaJson)) {
        qWarning() << "Game::inicializar - Error cargando mapa Tiled JSON:" << rutaJson;
        // Aunque el mapa falló, seguimos creando el jugador y niveles para facilitar debugging.
    } else {
        qDebug() << "Game::inicializar - Mapa cargado correctamente desde:" << rutaJson;
        // Render inicial: esto añade QGraphicsPixmapItem para cada tile en la escena
        // Usamos una porción amplia; MapaTiles::renderizar se encarga de poblar la escena.
        int tilesX = (int)(m_scene->sceneRect().width() / (m_mapa->anchoTile() > 0 ? m_mapa->anchoTile() : 32)) + 2;
        int tilesY = (int)(m_scene->sceneRect().height() / (m_mapa->altoTile() > 0 ? m_mapa->altoTile() : 32)) + 2;
        // Si sceneRect es nulo o pequeño, renderizamos con valores por defecto
        if (tilesX <= 0) tilesX = 40;
        if (tilesY <= 0) tilesY = 30;
        m_mapa->renderizar(0, 0, tilesX, tilesY);

        // Asegurar que la escena cubra el mapa (para que QGraphicsView muestre correctamente)
        QSize mapPx = m_mapa->mapPixelSize();
        if (!mapPx.isEmpty()) {
            m_scene->setSceneRect(0, 0, mapPx.width(), mapPx.height());
            qDebug() << "Game::inicializar - sceneRect ajustado a tamaño del mapa:" << mapPx;
        }
    }

    // Añadir jugador a la escena y posicionarlo en spawn si existe
    if (m_player) {
        // Asegurarnos de que el jugador sea un QGraphicsItem válido
        if (m_scene) {
            m_scene->addItem(m_player);
            // Intentar ubicar en Spawn2 (si existe), sino (1,1)
            QPoint initialTile(1, 1);
            if (m_mapa) {
                QVector<QPoint> spawns = m_mapa->puntosSpawn(QStringLiteral("Spawn2"));
                if (!spawns.isEmpty()) initialTile = spawns.first();
                // calcular en pixeles
                int px = initialTile.x() * m_mapa->anchoTile();
                int py = initialTile.y() * m_mapa->altoTile();
                m_player->setPos(px, py);
                qDebug() << "[Game] jugador posicionado en tile" << initialTile << "pixeles:" << px << py;
            } else {
                m_player->setPos(initialTile.x() * 32, initialTile.y() * 32);
                qDebug() << "[Game] m_mapa es nullptr, jugador posicionado en (1,1) por defecto.";
            }
        } else {
            qWarning() << "Game::inicializar - no se pudo agregar jugador a escena: m_scene es nullptr";
        }
    }

    // Crear niveles y configurarlos (comparten referencias a managers y al mapa)
    Level* damasco = new Level(this);
    Level* efeso   = new Level(this);
    Level* prision = new Level(this);

    // setear managers y referencias útiles en niveles
    damasco->setScene(m_scene);
    damasco->setPlayer(m_player);
    damasco->setManagers(m_interacciones, m_sonido, m_fisicas, m_eventos);
    damasco->setMapa(m_mapa);
    damasco->setCamara(m_camara);

    efeso->setScene(m_scene);
    efeso->setPlayer(m_player);
    efeso->setManagers(m_interacciones, m_sonido, m_fisicas, m_eventos);
    efeso->setMapa(m_mapa);
    efeso->setCamara(m_camara);

    prision->setScene(m_scene);
    prision->setPlayer(m_player);
    prision->setManagers(m_interacciones, m_sonido, m_fisicas, m_eventos);
    prision->setMapa(m_mapa);
    prision->setCamara(m_camara);

    m_levels->agregarNivel(damasco);
    m_levels->agregarNivel(efeso);
    m_levels->agregarNivel(prision);

    // Conexiones de registro para eventos del jugador
    connect(m_player, &Player::exorcismoExitoso, this, [this](Entidad* e){
        if (m_registro) m_registro->agregar("Exorcismo exitoso sobre " + e->nombre());
    });
    connect(m_player, &Player::exorcismoFallido, this, [this](Entidad* e){
        if (m_registro) m_registro->agregar("Exorcismo fallido contra " + e->nombre());
    });

    qDebug() << "Game::inicializar - inicialización completada.";
}

void Game::update(float dt) {
    if (!m_levels || !m_levels->nivelActual())
        return;

    m_levels->nivelActual()->update(dt);
}
