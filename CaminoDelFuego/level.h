#ifndef LEVEL_H
#define LEVEL_H

#pragma once
#include <QObject>
#include <QGraphicsScene>

class Player;
class MapaTiles;
class Camara2D;
class GestorInteracciones;
class GestorSonido;
class GestorFisicas;
class GestorEventos;

class Level : public QObject {
    Q_OBJECT
public:
    explicit Level(QObject* parent = nullptr);
    ~Level() override;

    bool cargar(const QString& rutaMapa);

    void setScene(QGraphicsScene* scene);
    void setPlayer(Player* player);

    void setManagers(GestorInteracciones* interacciones,
                     GestorSonido* sonido,
                     GestorFisicas* fisicas,
                     GestorEventos* eventos);

    void setMapa(MapaTiles* mapa);
    void setCamara(Camara2D* cam);

    void update(float dt);

signals:
    void nivelCompletado();
    void jugadorMurio();

private:
    QGraphicsScene* m_scene;
    Player* m_player;
    MapaTiles* m_mapa;
    Camara2D* m_camara;

    GestorInteracciones* m_interacciones;
    GestorSonido* m_sonido;
    GestorFisicas* m_fisicas;
    GestorEventos* m_eventos;

    bool m_completado;
};

#endif // LEVEL_H
