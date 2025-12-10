#include "VentanaCartasPulinas.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include <QPushButton>
#include <QLabel>

VentanaCartasPulinas::VentanaCartasPulinas(QWidget* parent)
    : QDialog(parent),
    m_lista(new QListWidget(this)),
    m_btnSeleccionar(new QPushButton("Seleccionar", this)),
    m_btnCancelar(new QPushButton("Cancelar", this))
{
    setWindowTitle("Cartas Pulinas");
    setModal(true);
    setMinimumSize(320, 240);

    auto* mainLay = new QVBoxLayout(this);
    mainLay->addWidget(new QLabel("Selecciona una carta para continuar:"));
    mainLay->addWidget(m_lista);

    auto* botonesLay = new QHBoxLayout();
    botonesLay->addStretch();
    botonesLay->addWidget(m_btnSeleccionar);
    botonesLay->addWidget(m_btnCancelar);

    mainLay->addLayout(botonesLay);

    connect(m_btnSeleccionar, &QPushButton::clicked, this, &VentanaCartasPulinas::onSeleccionar);
    connect(m_btnCancelar,   &QPushButton::clicked, this, &VentanaCartasPulinas::onCancelar);
    connect(m_lista, &QListWidget::itemDoubleClicked, this, &VentanaCartasPulinas::onDobleClickLista);
}

VentanaCartasPulinas::~VentanaCartasPulinas() = default;

void VentanaCartasPulinas::iniciar(const QStringList& cartas) {
    m_lista->clear();
    m_lista->addItems(cartas);
    if (!cartas.isEmpty())
        m_lista->setCurrentRow(0);
    show();
}

void VentanaCartasPulinas::onSeleccionar() {
    auto* item = m_lista->currentItem();
    if (!item) return;
    emit cartaSeleccionada(item->text());
    close();
}

void VentanaCartasPulinas::onCancelar() {
    emit ventanaCerrada();
    close();
}

void VentanaCartasPulinas::onDobleClickLista() {
    onSeleccionar();
}
