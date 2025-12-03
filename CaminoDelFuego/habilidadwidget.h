#ifndef HABILIDADWIDGET_H
#define HABILIDADWIDGET_H

#include <QDialog>
#include <QLabel>
#include <QVector>

class HabilidadWidget : public QDialog {
    Q_OBJECT

public:
    explicit HabilidadWidget(QWidget* parent = nullptr);
    // tipo: "exorcismo" / "sanacion" / "oratoria" (solo para info)
    // dificultad: longitud de la secuencia (3..8)
    void iniciar(const QString& tipo, int dificultad);

signals:
    // Emite true si el jugador completó la secuencia
    void resultado(bool exito);

protected:
    void keyPressEvent(QKeyEvent* e) override;

private:
    QVector<QString> secuencia;
    int indiceActual;
    QLabel* label;
    QString tipoActual;
    void mostrarProgreso();
};

#endif // HABILIDADWIDGET_H
