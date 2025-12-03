#include "Level.h"
#include "Player.h"
#include "MapaTiles.h"
#include "Camara2D.h"
#include "GestorInteracciones.h"
#include "GestorSonido.h"
#include "GestorFisicas.h"
#include "GestorEventos.h"

Level::Level(QObject* parent)
    : QObject(parent),
    m_scene(nullptr),
    m_player(nullptr),
    m_mapa(nullptr),
    m_camara(nullptr),
    m_interacciones(nullptr),
    m_sonido(nullptr),
    m_fisicas(nullptr),
    m_eventos(nullptr),
    m_completado(false)
{
}

Level::~Level() = default;

bool Level::cargar(const QString& rutaMapa) {
    Q_UNUSED(rutaMapa);
    return true;
}

void Level::setScene(QGraphicsScene* scene) { m_scene = scene; }
void Level::setPlayer(Player* player) { m_player = player; }

void Level::setManagers(GestorInteracciones* interacciones,
                        GestorSonido* sonido,
                        GestorFisicas* fisicas,
                        GestorEventos* eventos)
{
    m_interacciones = interacciones;
    m_sonido = sonido;
    m_fisicas = fisicas;
    m_eventos = eventos;
}

void Level::setMapa(MapaTiles* mapa) { m_mapa = mapa; }
void Level::setCamara(Camara2D* cam) { m_camara = cam; }

void Level::update(float dt) {
    if (!m_player) return;

    m_fisicas->actualizar(dt);
    m_interacciones->procesarInteracciones(dt);
    m_eventos->actualizar(dt);

    m_camara->centrarEn(m_player->x(), m_player->y());
    m_mapa->renderizar(m_camara->camX(), m_camara->camY(),
                       m_camara->tilesX(), m_camara->tilesY());

    if (!m_player->estaVivo())
        emit jugadorMurio();
}
