#ifndef VENTANAEXORCISMO_H
#define VENTANAEXORCISMO_H

#pragma once
#include <QWidget>
#include <QVector>
#include <QChar>

class VentanaExorcismo : public QWidget {
    Q_OBJECT
public:
    explicit VentanaExorcismo(QWidget* parent = nullptr);
    ~VentanaExorcismo() override;

    void iniciarSecuencia();

protected:
    void keyPressEvent(QKeyEvent* event) override;

signals:
    void exorcismoCompletado();
    void exorcismoFallado();

private:
    QVector<QChar> secuencia; // letras (K, O, L, N -> se interpreta N como Ñ)
    int indice;
    void generarSecuencia(int longitud);
};

#endif // VENTANAEXORCISMO_H
