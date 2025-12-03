#ifndef INPUTVIEW_H
#define INPUTVIEW_H

#include <QGraphicsView>
#include <QKeyEvent>

class InputView : public QGraphicsView {
    Q_OBJECT

public:
    explicit InputView(QWidget* parent = nullptr);

signals:
    // Emite estado de movimiento continuo
    void mover(bool up, bool down, bool left, bool right);
    // Emite cuando se presiona una tecla de habilidad (K/O/L/Ñ)
    void teclaHabilidad(const QString& accion);

protected:
    void keyPressEvent(QKeyEvent* e) override;
    void keyReleaseEvent(QKeyEvent* e) override;

private:
    bool up, down, left, right;
};

#endif // INPUTVIEW_H
