#include "HudManager.h"
#include "GestorBarrasEstado.h"

HudManager::HudManager(QObject* parent)
    : QObject(parent),
    m_barras(nullptr),
    m_barVida(nullptr),
    m_barEnergia(nullptr),
    m_barFe(nullptr),
    m_barPecado(nullptr)
{
}

HudManager::~HudManager() = default;

void HudManager::setGestorBarras(GestorBarrasEstado* barras) {
    m_barras = barras;

    // conectar señales
    connect(barras, &GestorBarrasEstado::vidaCambiada, m_barVida, &QProgressBar::setValue);
    connect(barras, &GestorBarrasEstado::energiaCambiada, m_barEnergia, &QProgressBar::setValue);
    connect(barras, &GestorBarrasEstado::feCambiada, m_barFe, &QProgressBar::setValue);
    connect(barras, &GestorBarrasEstado::pecadoCambiado, m_barPecado, &QProgressBar::setValue);
}

void HudManager::conectarWidgets(QProgressBar* vida,
                                 QProgressBar* energia,
                                 QProgressBar* fe,
                                 QProgressBar* pecado)
{
    m_barVida = vida;
    m_barEnergia = energia;
    m_barFe = fe;
    m_barPecado = pecado;
}
