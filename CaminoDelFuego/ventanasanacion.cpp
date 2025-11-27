#include "VentanaSanacion.h"
#include "Entidad.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>

VentanaSanacion::VentanaSanacion(QWidget* parent)
    : QDialog(parent), m_objetivo(nullptr)
{
    setWindowTitle("Sanación");
    auto* layout = new QVBoxLayout(this);

    layout->addWidget(new QLabel("Pablo ora por la persona necesitada…"));

    auto* btnA = new QPushButton("Sanar");
    auto* btnB = new QPushButton("Cancelar");

    connect(btnA, &QPushButton::clicked, this, &VentanaSanacion::onAceptar);
    connect(btnB, &QPushButton::clicked, this, &VentanaSanacion::onCancelar);

    layout->addWidget(btnA);
    layout->addWidget(btnB);
}

VentanaSanacion::~VentanaSanacion() = default;

void VentanaSanacion::iniciar(Entidad* objetivo) {
    m_objetivo = objetivo;
    show();
}

void VentanaSanacion::onAceptar() {
    emit sanacionCompletada(m_objetivo, true);
    close();
}

void VentanaSanacion::onCancelar() {
    emit sanacionCompletada(m_objetivo, false);
    close();
}
