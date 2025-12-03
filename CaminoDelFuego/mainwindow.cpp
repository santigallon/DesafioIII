#include "MainWindow.h"
#include "Game.h"
#include "HudManager.h"
#include "inputview.h"
#include "habilidadwidget.h"
#include "registro.h"
#include "Player.h"

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTimer>
#include <QStatusBar>
#include <QDebug>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
    m_game(new Game(this)),
    m_view(new InputView(this)),
    m_scene(new QGraphicsScene(this)),
    m_hud(new HudManager(this)),
    m_timer(new QTimer(this)),
    m_habilidad(new HabilidadWidget(this)),
    m_registro(nullptr)
{
    // pasar la escena al juego antes de inicializar (tu código original hacía esto)
    m_game->setScene(m_scene);

    setCentralWidget(m_view);
    m_view->setScene(m_scene);

    // inicializar juego (crea player, managers...)
    m_game->inicializar();

    // obtener el registro desde Game (lo creamos en Game::inicializar)
    m_registro = m_game->registro();

    // Conectar log al statusBar y a qDebug
    if (m_registro) {
        connect(m_registro, &Registro::nuevoMensaje, this, [this](const QString& msg){
            statusBar()->showMessage(msg, 4000); // mostrar 4s
            qDebug() << "[LOG]" << msg;
        });
    }

    // Conectar mover (WASD)
    connect(m_view, &InputView::mover, this, [this](bool up, bool down, bool left, bool right){
        if (m_game && m_game->jugador()) {
            m_game->jugador()->moverPorInput(up, down, left, right, 0.016f);
        }
    });

    // Cuando se presione K/O/L/Ñ desde el view, lanzamos los comportamientos:
    connect(m_view, &InputView::teclaHabilidad, this, [this](const QString& t){
        if (!m_game || !m_game->jugador()) return;

        // Registrar el intento
        if (m_registro) m_registro->agregar(QString("Tecla habilidad presionada: %1").arg(t));

        // K: iniciar mini-juego exorcismo
        if (t == "K") {
            m_habilidad->iniciar("Exorcismo", 4); // dificultad 4 por defecto
            // Cuando termine:
            connect(m_habilidad, &HabilidadWidget::resultado, this, [this](bool exito){
                if (exito) {
                    if (m_registro) m_registro->agregar("Exorcismo: Éxito");
                    // recompensa simple: curar o marcar exorcismo
                    m_game->jugador()->recibirProvidencia();
                } else {
                    if (m_registro) m_registro->agregar("Exorcismo: Falló");
                    m_game->jugador()->aplicarDanio(8, "Falla en exorcismo");
                }
            }, Qt::UniqueConnection);
        }
        // O: oratoria -> mini-juego (menor dificultad)
        else if (t == "O") {
            m_habilidad->iniciar("Oratoria", 3);
            connect(m_habilidad, &HabilidadWidget::resultado, this, [this](bool exito){
                if (exito) {
                    if (m_registro) m_registro->agregar("Oratoria: Éxito");
                    m_game->jugador()->aumentarFe(2.0);
                } else {
                    if (m_registro) m_registro->agregar("Oratoria: Falló");
                    m_game->jugador()->disminuirFe(1.5);
                }
            }, Qt::UniqueConnection);
        }
        // L: providencia (acción directa)
        else if (t == "L") {
            if (m_registro) m_registro->agregar("Providencia invocada (L)");
            m_game->jugador()->recibirProvidencia();
        }
        // Ñ: abrir escritura (ponemos un log y emitimos carta completa)
        else if (t == "Ñ") {
            if (m_registro) m_registro->agregar("Inicia escritura de carta (Ñ)");
            m_game->jugador()->iniciarEscritura("Carta breve: Señor, ten piedad...");
            // también registramos la carta (simulación)
            if (m_registro) m_registro->agregar("Carta escrita: 'Señor, ten piedad...'");
        }
    });

    connect(m_timer, &QTimer::timeout, this, &MainWindow::actualizarJuego);
    m_timer->start(16); // ~60 FPS
}

MainWindow::~MainWindow() = default;

void MainWindow::actualizarJuego() {
    m_game->update(0.016f);
}
