#ifndef VENTANASANACION_H
#define VENTANASANACION_H

#pragma once
#include <QDialog>

class Entidad;

class VentanaSanacion : public QDialog {
    Q_OBJECT
public:
    explicit VentanaSanacion(QWidget* parent = nullptr);
    ~VentanaSanacion() override;

    void iniciar(Entidad* objetivo);

signals:
    void sanacionCompletada(Entidad* objetivo, bool exitoso);

private slots:
    void onAceptar();
    void onCancelar();

private:
    Entidad* m_objetivo;
};

#endif // VENTANASANACION_H
