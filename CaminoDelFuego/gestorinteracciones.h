#ifndef GESTORINTERACCIONES_H
#define GESTORINTERACCIONES_H

#pragma once
#include <QObject>
#include <QString>
#include <QTimer>

class Player;
class Entidad;
class GestorSonido;
class GestorEventos;
class GestorBarrasEstado;

/**
 * GestorInteracciones
 * - Centraliza las interacciones entre Player y Entidades (NPCs, Endemoniados, etc.)
 * - Inicia ventanas de exorcismo/oratoria/sanación mediante señales.
 * - Consulta GestorFisicas (externo) si es necesario.
 *
 * NOTA: las ventanas (UI) pueden estar a cargo de GestorPantallas o Game.
 */

class GestorInteracciones : public QObject {
    Q_OBJECT
public:
    explicit GestorInteracciones(QObject* parent = nullptr);
    ~GestorInteracciones() override;

    void setGestorSonido(GestorSonido* s);
    void setGestorEventos(GestorEventos* e);
    void setGestorBarras(GestorBarrasEstado* b);

    // Acciones invocadas por el juego (ej. al presionar una tecla cerca de un NPC)
    void intentarExorcismo(Player* jugador, Entidad* objetivo);
    void intentarOratoria(Player* jugador, Entidad* audiencia);
    void intentarSanacion(Player* jugador, Entidad* objetivo);

signals:
    // señales para que la UI / GestorPantallas abra las ventanas correspondientes
    void empezarExorcismo(Entidad* objetivo);
    void empezarOratoria(Entidad* audiencia);
    void empezarSanacion(Entidad* objetivo);

    // resultado final de interacción (éxito/fracaso)
    void exorcismoResultado(Entidad* objetivo, bool exitoso);
    void oratoriaResultado(Entidad* audiencia, bool exitoso);
    void sanacionResultado(Entidad* objetivo, bool exitoso);

private slots:
    void onTimeoutAccion();

private:
    GestorSonido* m_sonido;
    GestorEventos* m_eventos;
    GestorBarrasEstado* m_barras;

    QTimer m_timerAccion;
};

#endif // GESTORINTERACCIONES_H
