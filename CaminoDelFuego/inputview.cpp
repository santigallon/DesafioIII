#include "inputview.h"
#include <QString>

InputView::InputView(QWidget* parent)
    : QGraphicsView(parent),
    up(false), down(false), left(false), right(false)
{
    setFocusPolicy(Qt::StrongFocus);
    setInteractive(true);
}

void InputView::keyPressEvent(QKeyEvent* e) {
    if (e->isAutoRepeat()) return;

    // Movimiento WASD
    bool changed = false;
    switch (e->key()) {
    case Qt::Key_W: if (!up) { up = true; changed = true; } break;
    case Qt::Key_S: if (!down) { down = true; changed = true; } break;
    case Qt::Key_A: if (!left) { left = true; changed = true; } break;
    case Qt::Key_D: if (!right) { right = true; changed = true; } break;
    default: break;
    }

    // Habilidades K O L Ñ -> la tecla puede venir como letra en text()
    QString texto = e->text().toUpper();
    if (!texto.isEmpty()) {
        if (texto == "K" || texto == "O" || texto == "L" || texto == "Ñ" || texto == "N") {
            // Nota: algunos teclados pueden enviar 'N' en lugar de 'Ñ'; permitimos ambos
            QString tecla = (texto == "N") ? QString("Ñ") : texto;
            emit teclaHabilidad(tecla);
        }
    }

    if (changed) emit mover(up, down, left, right);
    // mantener comportamiento por defecto para otras teclas
    QGraphicsView::keyPressEvent(e);
}

void InputView::keyReleaseEvent(QKeyEvent* e) {
    if (e->isAutoRepeat()) return;

    bool changed = false;
    switch (e->key()) {
    case Qt::Key_W: if (up) { up = false; changed = true; } break;
    case Qt::Key_S: if (down) { down = false; changed = true; } break;
    case Qt::Key_A: if (left) { left = false; changed = true; } break;
    case Qt::Key_D: if (right) { right = false; changed = true; } break;
    default: break;
    }

    if (changed) emit mover(up, down, left, right);
    QGraphicsView::keyReleaseEvent(e);
}
