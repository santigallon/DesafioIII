#pragma once
#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QWidget>
#include <QPoint>
#include <QTimer>
#include <QPixmap>
#include <QMap>
#include "LevelManager.h"
class VentanaCartasPulinas;
class HouseWindow;

class GameWidget : public QWidget {
    Q_OBJECT
public:
    explicit GameWidget(QWidget* parent = nullptr);
    ~GameWidget() override;

public slots:
    void resetJuego();
    void cargarNivel(const QString& path);

protected:
    void paintEvent(QPaintEvent* ev) override;
    void keyPressEvent(QKeyEvent* ev) override;
    void timerEvent(QTimerEvent* ev) override;

private slots:
    void usarCartaSeleccionada(const QString& carta);
    void ventanaCartasCerrada();
    void onHouseAction(const QString& tipo, bool exito);

private:
    QPoint m_player;
    int m_playerVida;
    int m_tileSize;
    int m_timerId;
    VentanaCartasPulinas* m_cartasWnd;
    LevelManager m_level;
    int m_cameraX, m_cameraY;

    // Sprites
    QPixmap m_playerSprite;
    QMap<Faccion, QPixmap> m_npcSprites;

    void cargarSpritesPorDefecto();
    QPixmap spriteForFaction(Faccion f) const;

    void moverJugador(int dx, int dy);
    void comprobarEncuentro();
    void actualizarCamara();
    void dibujarHUD(QPainter& p);
};

#endif // GAMEWIDGET_H
