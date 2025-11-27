#include "VentanaOratoria.h"
#include "Entidad.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>

VentanaOratoria::VentanaOratoria(QWidget* parent)
    : QDialog(parent), m_audiencia(nullptr)
{
    setWindowTitle("Oratoria");
    auto* layout = new QVBoxLayout(this);

    layout->addWidget(new QLabel("Pablo habla movido por el Espíritu Santo…"));

    auto* btnA = new QPushButton("Proclamar mensaje");
    auto* btnB = new QPushButton("Cancelar");

    connect(btnA, &QPushButton::clicked, this, &VentanaOratoria::onAceptar);
    connect(btnB, &QPushButton::clicked, this, &VentanaOratoria::onCancelar);

    layout->addWidget(btnA);
    layout->addWidget(btnB);
}

VentanaOratoria::~VentanaOratoria() = default;

void VentanaOratoria::iniciar(Entidad* audiencia) {
    m_audiencia = audiencia;
    show();
}

void VentanaOratoria::onAceptar() {
    emit oratoriaCompletada(m_audiencia, true);
    close();
}

void VentanaOratoria::onCancelar() {
    emit oratoriaCompletada(m_audiencia, false);
    close();
}
