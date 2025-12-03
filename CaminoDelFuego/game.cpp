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
    // Crea componentes (no se añaden a la escena aún)
    m_player = new Player();
    m_mapa = new MapaTiles(m_scene);
    m_camara = new Camara2D();

    m_interacciones = new GestorInteracciones(this);
    m_sonido = new GestorSonido(this);

    m_eventos = new GestorEventos(this);
    m_fisicas = new GestorFisicas(m_scene, this);

    // Registro de sucesos
    m_registro = new Registro(this);

    // LevelManager y niveles
    m_levels = new LevelManager(this);

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
}

void Game::update(float dt) {
    if (!m_levels || !m_levels->nivelActual())
        return;

    m_levels->nivelActual()->update(dt);
}
