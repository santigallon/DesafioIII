#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class Game;
class HudManager;
class InputView;
class QGraphicsScene;
class QTimer;
class HabilidadWidget;
class Registro;
class QKeyEvent; // forward declaration

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;

    bool keyW, keyA, keyS, keyD;

private slots:
    void actualizarJuego();

private:
    Game* m_game;
    InputView* m_view;
    QGraphicsScene* m_scene;
    HudManager* m_hud;
    QTimer* m_timer;

    // Mini-juego y log
    HabilidadWidget* m_habilidad;
    Registro* m_registro;
};

#endif // MAINWINDOW_H
