#include "habilidadwidget.h"
#include <QVBoxLayout>
#include <QKeyEvent>
#include <QRandomGenerator>

HabilidadWidget::HabilidadWidget(QWidget* parent)
    : QDialog(parent), indiceActual(0)
{
    setWindowTitle("Prueba de habilidad");
    setModal(true);
    label = new QLabel(this);
    label->setAlignment(Qt::AlignCenter);

    auto* layout = new QVBoxLayout(this);
    layout->addWidget(label);

    setFixedSize(360, 120);
    setFocusPolicy(Qt::StrongFocus);
}

void HabilidadWidget::iniciar(const QString& tipo, int dificultad) {
    secuencia.clear();
    indiceActual = 0;
    tipoActual = tipo;

    const QString keys = "KOLÑ";
    // generar secuencia aleatoria (usar QRandomGenerator)
    for (int i = 0; i < qMax(1, dificultad); ++i) {
        int idx = QRandomGenerator::global()->bounded(keys.size());
        secuencia.append(QString(keys.at(idx)));
    }

    mostrarProgreso();
    show();
    activateWindow();
    setFocus();
}

void HabilidadWidget::mostrarProgreso() {
    QString display;
    for (int i = 0; i < secuencia.size(); ++i) {
        if (i < indiceActual) display += "[✓] ";
        else if (i == indiceActual) display += "[" + secuencia[i] + "] ";
        else display += "[ ] ";
    }
    label->setText(QString("Secuencia (%1):\n%2").arg(tipoActual, display));
}

void HabilidadWidget::keyPressEvent(QKeyEvent* e) {
    if (e->isAutoRepeat()) return;

    QString texto = e->text().toUpper();
    QString tecla;
    if (texto == "K" || texto == "O" || texto == "L" || texto == "Ñ" || texto == "N")
        tecla = (texto == "N") ? QString("Ñ") : texto;
    else {
        // ignorar otras teclas
        QDialog::keyPressEvent(e);
        return;
    }

    if (secuencia.isEmpty()) {
        emit resultado(false);
        close();
        return;
    }

    if (tecla == secuencia[indiceActual]) {
        indiceActual++;
        if (indiceActual >= secuencia.size()) {
            emit resultado(true);
            close();
            return;
        } else {
            mostrarProgreso();
        }
    } else {
        emit resultado(false);
        close();
    }
}
