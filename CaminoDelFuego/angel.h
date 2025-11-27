#ifndef ANGEL_H
#define ANGEL_H

#pragma once
#include "NPC.h"
#include <QString>

/**
 * Angel - mensajero espiritual. No es humano; puede activar eventos.
 */
class Angel : public NPC {
    Q_OBJECT
public:
    explicit Angel(QGraphicsItem* parent = nullptr);
    ~Angel() override;

    void actualizar(float dt) override;
    void interactuar(Entidad* otro) override;

    void revelarAlJugador();
    void enviarMensaje(const QString& mensaje);

private:
    QString m_mensaje;
    bool m_visibleParaJugador;
};
#endif // ANGEL_H
