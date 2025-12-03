#include "LevelManager.h"
#include "Level.h"

LevelManager::LevelManager(QObject* parent)
    : QObject(parent),
    m_indiceActual(0)
{
}

LevelManager::~LevelManager() = default;

void LevelManager::agregarNivel(Level* nivel) {
    m_niveles.push_back(nivel);
}

Level* LevelManager::nivelActual() const {
    if (m_indiceActual < 0 || m_indiceActual >= (int)m_niveles.size())
        return nullptr;
    return m_niveles[m_indiceActual];
}

void LevelManager::avanzarNivel() {
    m_indiceActual++;

    if (m_indiceActual >= (int)m_niveles.size())
        emit todosLosNivelesCompletados();
    else
        emit nivelCambio(nivelActual());
}

void LevelManager::reiniciarNivel() {
    emit nivelCambio(nivelActual());
}
