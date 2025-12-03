#ifndef ANGEL_H
#define ANGEL_H

#include "Entidad.h"
#include <QString>

class GestorSonido;

class Angel : public Entidad {
    Q_OBJECT
public:
    Angel(QGraphicsItem* parent = nullptr);

    void actualizar(float dt) override;
    void interactuar(Entidad* otro) override;

    void revelarAlJugador();
    void enviarMensaje(const QString& mensaje);

signals:
    void mensajeDivino(const QString& mensaje);

private:
    bool m_visibleParaJugador;
    QString m_mensaje;
    QPointF m_posInicial;
    float m_tiempo;
    GestorSonido* m_sonido;
};

#endif
