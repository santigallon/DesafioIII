#ifndef GAME_H
#define GAME_H
#pragma once
#include <QObject>

class Player;
class LevelManager;
class GestorInteracciones;
class GestorSonido;
class GestorFisicas;
class GestorEventos;
class MapaTiles;
class Camara2D;
class QGraphicsScene;
class Registro;

class Game : public QObject {
    Q_OBJECT
public:
    explicit Game(QObject* parent = nullptr);
    ~Game() override;

    void inicializar();
    void update(float dt);
    void setScene(QGraphicsScene* scene);

    Player* jugador() const { return m_player; }
    LevelManager* niveles() const { return m_levels; }
    Registro* registro() const { return m_registro; }

signals:
    void juegoTerminado();
    void mostrarMensaje(const QString& msg);

private:
    Player* m_player;
    LevelManager* m_levels;
    QGraphicsScene* m_scene = nullptr;

    GestorInteracciones* m_interacciones;
    GestorSonido* m_sonido;
    GestorFisicas* m_fisicas;
    GestorEventos* m_eventos;

    MapaTiles* m_mapa;
    Camara2D* m_camara;
    Registro* m_registro = nullptr;
};

#endif // GAME_H
