#include "VentanaExorcismo.h"
#include "Entidad.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>

VentanaExorcismo::VentanaExorcismo(QWidget* parent)
    : QDialog(parent), m_objetivo(nullptr)
{
    setWindowTitle("Exorcismo");
    auto* layout = new QVBoxLayout(this);

    layout->addWidget(new QLabel("Intenta expulsar al espíritu inmundo…"));

    auto* btnA = new QPushButton("Realizar Exorcismo");
    auto* btnB = new QPushButton("Cancelar");

    connect(btnA, &QPushButton::clicked, this, &VentanaExorcismo::onAceptar);
    connect(btnB, &QPushButton::clicked, this, &VentanaExorcismo::onCancelar);

    layout->addWidget(btnA);
    layout->addWidget(btnB);
}

VentanaExorcismo::~VentanaExorcismo() = default;

void VentanaExorcismo::iniciar(Entidad* objetivo) {
    m_objetivo = objetivo;
    show();
}

void VentanaExorcismo::onAceptar() {
    emit exorcismoCompletado(m_objetivo, true);
    close();
}

void VentanaExorcismo::onCancelar() {
    emit exorcismoCompletado(m_objetivo, false);
    close();
}
