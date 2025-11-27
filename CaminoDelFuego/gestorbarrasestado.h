#ifndef GESTORBARRASESTADO_H
#define GESTORBARRASESTADO_H

#pragma once
#include <QObject>

/**
 * GestorBarrasEstado
 * - Mantiene valores visibles (vida, energía, fe, pecado) para un jugador o entidad.
 * - Expone API para modificar y consultar; emite señales para el HUD.
 *
 * NOTA: Este gestor puede tener instancias por HUD/ventana o una instancia global que
 * actualice widgets según la entidad objetivo.
 */

class Entidad;
class Player;

class GestorBarrasEstado : public QObject {
    Q_OBJECT
public:
    explicit GestorBarrasEstado(QObject* parent = nullptr);
    ~GestorBarrasEstado() override;

    // Asociar con una entidad objetivo (puede ser Player u otra Entidad)
    void setObjetivo(Entidad* objetivo);

    // Actualizar valores (invocado desde la entidad o managers)
    void establecerVida(int v);
    void establecerEnergia(float e);
    void establecerFe(double f);
    void establecerPecado(double p);

    // Consultas
    int vida() const;
    float energia() const;
    double fe() const;
    double pecado() const;

signals:
    void vidaCambiada(int nuevaVida);
    void energiaCambiada(float nuevaEnergia);
    void feCambiada(double nuevaFe);
    void pecadoCambiado(double nuevoPecado);

private:
    Entidad* m_objetivo;
    int m_vida;
    float m_energia;
    double m_fe;
    double m_pecado;
};

#endif // GESTORBARRASESTADO_H
