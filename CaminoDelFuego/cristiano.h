#ifndef CRISTIANO_H
#define CRISTIANO_H
#pragma once
#include "NPC.h"

class Cristiano : public NPC {
    Q_OBJECT
public:
    explicit
    Cristiano(QGraphicsItem* parent = nullptr);
    ~Cristiano() override;

    void actualizar(float dt) override;
    void interactuar(Entidad* otro) override;

    void orarPor(Entidad* objetivo);
    void curarAliado(Entidad* objetivo, int cantidad);
private:
    bool m_convertidoRecientemente;
    int m_fuerzaOracion;
};

#endif // CRISTIANO_H
