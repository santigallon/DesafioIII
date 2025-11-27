#ifndef VENTANAEXORCISMO_H
#define VENTANAEXORCISMO_H

#pragma once
#include <QDialog>

class Entidad;

class VentanaExorcismo : public QDialog {
    Q_OBJECT
public:
    explicit VentanaExorcismo(QWidget* parent = nullptr);
    ~VentanaExorcismo() override;

    void iniciar(Entidad* objetivo);

signals:
    void exorcismoCompletado(Entidad* objetivo, bool exitoso);

private slots:
    void onAceptar();
    void onCancelar();

private:
    Entidad* m_objetivo;
};
#endif // VENTANAEXORCISMO_H
