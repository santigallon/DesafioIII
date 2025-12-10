#pragma once
#ifndef LEVELMANAGER_H
#define LEVELMANAGER_H

#include <QObject>
#include <QVector>
#include <QString>
#include <QGraphicsScene>
#include <QPoint>
#include "MapaTiles.h"
#include "NPC.h"

    class Level;

/**
 * LevelManager
 * - Gestiona niveles, el mapa y los NPCs del nivel actual.
 * - Provee una QGraphicsScene para ser renderizada por la UI.
 */
class LevelManager : public QObject {
    Q_OBJECT
public:
    explicit LevelManager(QObject* parent = nullptr);
    ~LevelManager() override;

    // Gestión de niveles
    void agregarNivel(Level* nivel);
    Level* nivelActual() const;
    void avanzarNivel();
    void reiniciarNivel();

    // Cargar nivel desde archivo (JSON Tiled)
    bool loadLevelFile(const QString& path);

    // Acceso al mapa y escena
    MapaTiles* map(); // devuelve puntero al mapa interno (o nullptr)
    QGraphicsScene* scene() { return m_scene; }

    // NPCs
    const QVector<NPC*>& npcs() const { return m_npcs; }
    QVector<NPC*>& editableNpcs() { return m_npcs; }

    // Spawns y lógica simple de IA
    void spawnInitialNPCs();
    void updateAI();

signals:
    void todosLosNivelesCompletados();
    void nivelCambio(Level* nuevoNivel);

private:
    QVector<Level*> m_niveles;
    int m_indiceActual = -1;

    // NPCs almacenados como punteros
    QVector<NPC*> m_npcs;

    // mapa interno como puntero (coherente con MapaTiles API)
    MapaTiles* m_map = nullptr;
    QGraphicsScene* m_scene = nullptr;
};

#endif // LEVELMANAGER_H
