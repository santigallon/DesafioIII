#ifndef LEVELMANAGER_H
#define LEVELMANAGER_H

#pragma once
#include <QObject>
#include <vector>

class Level;

class LevelManager : public QObject {
    Q_OBJECT
public:
    explicit LevelManager(QObject* parent = nullptr);
    ~LevelManager() override;

    void agregarNivel(Level* nivel);
    Level* nivelActual() const;

    void avanzarNivel();
    void reiniciarNivel();

signals:
    void todosLosNivelesCompletados();
    void nivelCambio(Level* nuevoNivel);

private:
    std::vector<Level*> m_niveles;
    int m_indiceActual;
};

#endif // LEVELMANAGER_H
