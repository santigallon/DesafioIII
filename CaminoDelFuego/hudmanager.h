#ifndef HUDMANAGER_H
#define HUDMANAGER_H

#pragma once
#include <QObject>
#include <QProgressBar>

class GestorBarrasEstado;

class HudManager : public QObject {
    Q_OBJECT
public:
    explicit HudManager(QObject* parent = nullptr);
    ~HudManager() override;

    void setGestorBarras(GestorBarrasEstado* barras);

    void conectarWidgets(QProgressBar* vida,
                         QProgressBar* energia,
                         QProgressBar* fe,
                         QProgressBar* pecado);

private:
    GestorBarrasEstado* m_barras;

    QProgressBar* m_barVida;
    QProgressBar* m_barEnergia;
    QProgressBar* m_barFe;
    QProgressBar* m_barPecado;
};

#endif // HUDMANAGER_H
