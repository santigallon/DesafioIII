#ifndef PLAYER_H
#define PLAYER_H

#include "Entidad.h"
#include <QPointF>
#include <QString>
#include <QTimer>


class Player : public Entidad {
    Q_OBJECT
public:
    explicit Player(QGraphicsItem* parent = nullptr);
    ~Player() override;

    // Atributos físicos / espirituales
    float energia() const;
    void setEnergia(float e);

    double fe() const;
    double pecado() const;
    double ego() const;
    double gracia() const;

    int proyectiles() const;
    int pergaminos() const;

    QString estadoActual() const;
    bool esControlable() const;

    // Acciones del jugador
    void actualizar(float dt) override;
    void moverPorInput(bool up, bool down, bool left, bool right, float dt);
    void orar(int intensidad = 1);
    void descansar(float segundos);
    void interactuarCon(Entidad* e, bool ayudar);
    void lanzarProyectil(const QPointF& dir);
    void iniciarExorcismo(Entidad* objetivo);
    void iniciarEscritura(const QString& texto);
    void iniciarOratoria(const QString& tema);

    // Providencia y efectos
    void recibirProvidencia();
    void aplicarDanio(int cantidad, const QString& razon = QString());
    void curarJugador(int cantidad);

    // Modificadores espirituales
    void aumentarFe(double cantidad);
    void disminuirFe(double cantidad);
    void agregarPecado(double cantidad);
    void reducirPecado(double cantidad);

    bool feAlta(double umbral = 75.0) const;
    bool estaCorrupto(double umbral = 60.0) const;

signals:
    void energiaCambiada(float nuevaEnergia);
    void feCambiada(double nuevaFe);
    void pecadoCambiado(double nuevoPecado);

    void exorcismoExitoso(Entidad* objetivo);
    void exorcismoFallido(Entidad* objetivo);
    void cartaCompleta(const QString& contenido);
    void oratoriaExitosa();
    void oratoriaFallida();
    void murioJugador();

private slots:
    void onRegenTick();

private:
    // Físicos
    float m_energia;
    QPointF m_velocidad;

    // Espirituales
    double m_fe;
    double m_pecado;
    double m_ego;
    double m_gracia;

    // Recursos
    int m_proyectiles;
    int m_pergaminos;
    int m_oracion; // contador de oraciones o intensidad acumulada

    QString m_estadoActual;
    bool m_controlable;

    // Temporizadores / regeneración
    QTimer m_regenTimer;
    void clampValores();
};
#endif // PLAYER_H



