#include "HudManager.h"
#include "GestorBarrasEstado.h"

#include <QVBoxLayout>
#include <QDebug>

HudManager::HudManager(QWidget* parent)
    : QWidget(parent),
    m_barras(nullptr),
    m_barVida(nullptr),
    m_barEnergia(nullptr),
    m_barFe(nullptr),
    m_barPecado(nullptr)
{
    // Si quieres que HudManager cree sus propias barras internas, puedes hacerlo aquí.
    // Por ahora dejamos que las barras las cree el UI y se conecten vía conectarWidgets().
    setContentsMargins(0,0,0,0);
}

HudManager::~HudManager() = default;

void HudManager::setGestorBarras(GestorBarrasEstado* barras) {
    if (m_barras == barras) return;

    // Si ya había uno anterior, desconectamos para evitar duplicados o dangling pointers
    if (m_barras) {
        // desconectar señales antiguas (seguro)
        disconnect(m_barras, nullptr, nullptr, nullptr);
    }

    m_barras = barras;
    establecerConexiones();

    // Opcional: sincronizar valores iniciales si GestorBarrasEstado expone getters:
    // if (m_barras && m_barVida) m_barVida->setValue(m_barras->vidaActual());
    // (Descomenta y adapta si tu Gestor tiene getters)
}

void HudManager::conectarWidgets(QProgressBar* vida,
                                 QProgressBar* energia,
                                 QProgressBar* fe,
                                 QProgressBar* pecado)
{
    m_barVida    = vida;
    m_barEnergia = energia;
    m_barFe      = fe;
    m_barPecado  = pecado;

    establecerConexiones();
}

void HudManager::establecerConexiones() {
    if (!m_barras) return;

    // Conectar solo si el widget destino existe
    if (m_barVida) {
        // desconectar previos para evitar múltiples conexiones
        disconnect(m_barras, &GestorBarrasEstado::vidaCambiada, nullptr, nullptr);
        connect(m_barras, &GestorBarrasEstado::vidaCambiada, m_barVida, &QProgressBar::setValue, Qt::UniqueConnection);
    }
    if (m_barEnergia) {
        disconnect(m_barras, &GestorBarrasEstado::energiaCambiada, nullptr, nullptr);
        connect(m_barras, &GestorBarrasEstado::energiaCambiada, m_barEnergia, &QProgressBar::setValue, Qt::UniqueConnection);
    }
    if (m_barFe) {
        disconnect(m_barras, &GestorBarrasEstado::feCambiada, nullptr, nullptr);
        connect(m_barras, &GestorBarrasEstado::feCambiada, m_barFe, &QProgressBar::setValue, Qt::UniqueConnection);
    }
    if (m_barPecado) {
        disconnect(m_barras, &GestorBarrasEstado::pecadoCambiado, nullptr, nullptr);
        connect(m_barras, &GestorBarrasEstado::pecadoCambiado, m_barPecado, &QProgressBar::setValue, Qt::UniqueConnection);
    }
}
