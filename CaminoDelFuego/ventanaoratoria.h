#pragma once
#ifndef VENTANAORATORIA_H
#define VENTANAORATORIA_H

#include <QDialog>

class Entidad;

class VentanaOratoria : public QDialog {
    Q_OBJECT
public:
    explicit VentanaOratoria(QWidget* parent = nullptr);
    ~VentanaOratoria() override;

    void iniciar(Entidad* audiencia);

signals:
    void oratoriaCompletada(Entidad* audiencia, bool exitoso);

private slots:
    void onAceptar();
    void onCancelar();

private:
    Entidad* m_audiencia;
};

#endif // VENTANAORATORIA_H
