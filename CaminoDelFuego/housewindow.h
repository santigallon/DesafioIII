#pragma once
#ifndef HOUSEWINDOW_H
#define HOUSEWINDOW_H

#include <QDialog>
#include <QVector>
#include <QString>
#include <QKeyEvent>

struct HouseNPC {
    QString faction;
    QPoint pos; // tile coords inside the house-area grid
    QString name;
    int vida;
};

class LevelManager;
class PatternWindow;

class HouseWindow : public QDialog {
    Q_OBJECT
public:
    // areaWidth/areaHeight: tamaño en tiles de cada cuarto (ej: 4x4)
    explicit HouseWindow(LevelManager* level, const QRect& worldDoorTileRect, QWidget* parent = nullptr);
    ~HouseWindow() override;

    // genera NPCs aleatorios dentro de la casa
    void generarNPCs();

signals:
    void accionRealizada(const QString& tipoAccion, bool exito);

protected:
    bool eventFilter(QObject* obj, QEvent* ev) override;

private:
    LevelManager* m_level;
    QRect m_worldDoor; // tile rect (x,y,width=1,height=1) en coordenadas del mapa
    QVector<HouseNPC> m_npcs;
    void construirUI();
    void abrirInteraccionConNPC(const HouseNPC& npc); // lanza PatternWindow con patrón según acción
    int m_areaW, m_areaH; // tamaño en tiles por cuarto
};

#endif // HOUSEWINDOW_H
