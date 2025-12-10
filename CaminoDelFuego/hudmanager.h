#ifndef HUDMANAGER_H
#define HUDMANAGER_H

#pragma once

#include <QWidget>
#include <QProgressBar>

class GestorBarrasEstado;

class HudManager : public QWidget {
    Q_OBJECT
public:
    explicit HudManager(QWidget* parent = nullptr);
    ~HudManager() override;

    void setGestorBarras(GestorBarrasEstado* barras);

    void conectarWidgets(QProgressBar* vida,
                         QProgressBar* energia,
                         QProgressBar* fe,
                         QProgressBar* pecado);

private:
    void establecerConexiones(); // intenta conectar si todos los punteros necesarios están disponibles

    GestorBarrasEstado* m_barras = nullptr;

    QProgressBar* m_barVida = nullptr;
    QProgressBar* m_barEnergia = nullptr;
    QProgressBar* m_barFe = nullptr;
    QProgressBar* m_barPecado = nullptr;
};

#endif // HUDMANAGER_H
